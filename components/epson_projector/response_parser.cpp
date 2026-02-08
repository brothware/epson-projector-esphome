#include "response_parser.h"

#include <algorithm>
#include <cctype>
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <optional>

namespace esphome::epson_projector {

namespace {

std::optional<int> safe_stoi(const std::string &str) {
  if (str.empty()) {
    return std::nullopt;
  }
  char *end = nullptr;
  errno = 0;
  long value = std::strtol(str.c_str(), &end, 10);
  if (errno != 0 || end == str.c_str() || *end != '\0') {
    return std::nullopt;
  }
  if (value < INT_MIN || value > INT_MAX) {
    return std::nullopt;
  }
  return static_cast<int>(value);
}

std::optional<uint32_t> safe_stoul(const std::string &str) {
  if (str.empty()) {
    return std::nullopt;
  }
  char *end = nullptr;
  errno = 0;
  unsigned long value = std::strtoul(str.c_str(), &end, 10);
  if (errno != 0 || end == str.c_str() || *end != '\0') {
    return std::nullopt;
  }
  if (value > UINT32_MAX) {
    return std::nullopt;
  }
  return static_cast<uint32_t>(value);
}

bool is_bool_true(const std::string &value) {
  return value == ARG_ON || value == ARG_ON_NUMERIC;
}

ParseResult make_volume(int v) {
  return VolumeResponse{v};
}
ParseResult make_brightness(int v) {
  return BrightnessResponse{v};
}
ParseResult make_contrast(int v) {
  return ContrastResponse{v};
}
ParseResult make_sharpness(int v) {
  return SharpnessResponse{v};
}
ParseResult make_density(int v) {
  return DensityResponse{v};
}
ParseResult make_tint(int v) {
  return TintResponse{v};
}
ParseResult make_color_temp(int v) {
  return ColorTempResponse{v};
}
ParseResult make_v_keystone(int v) {
  return VKeystoneResponse{v};
}
ParseResult make_h_keystone(int v) {
  return HKeystoneResponse{v};
}

ParseResult make_mute(bool v) {
  return MuteResponse{v};
}
ParseResult make_h_reverse(bool v) {
  return HReverseResponse{v};
}
ParseResult make_v_reverse(bool v) {
  return VReverseResponse{v};
}
ParseResult make_freeze(bool v) {
  return FreezeResponse{v};
}

ParseResult make_source(const std::string &v) {
  return SourceResponse{v};
}
ParseResult make_color_mode(const std::string &v) {
  return ColorModeResponse{v};
}
ParseResult make_aspect_ratio(const std::string &v) {
  return AspectRatioResponse{v};
}
ParseResult make_luminance(const std::string &v) {
  return LuminanceResponse{v};
}
ParseResult make_gamma(const std::string &v) {
  return GammaResponse{v};
}
ParseResult make_serial(const std::string &v) {
  return SerialNumberResponse{v};
}

struct ScaledIntEntry {
  const char *cmd;
  int ui_max;
  ParseResult (*make)(int);
};

struct BoolEntry {
  const char *cmd;
  ParseResult (*make)(bool);
};

struct StringEntry {
  const char *cmd;
  ParseResult (*make)(const std::string &);
};

constexpr ScaledIntEntry SCALED_INT_PARSERS[] = {
    {CMD_VOLUME, VOLUME_MAX, make_volume},
    {CMD_BRIGHTNESS, BRIGHTNESS_MAX, make_brightness},
    {CMD_CONTRAST, CONTRAST_MAX, make_contrast},
    {CMD_SHARPNESS, SHARPNESS_MAX, make_sharpness},
    {CMD_DENSITY, DENSITY_MAX, make_density},
    {CMD_TINT, TINT_MAX, make_tint},
    {CMD_COLOR_TEMP, COLOR_TEMP_MAX, make_color_temp},
    {CMD_VKEYSTONE, KEYSTONE_MAX, make_v_keystone},
    {CMD_HKEYSTONE, KEYSTONE_MAX, make_h_keystone},
};

constexpr BoolEntry BOOL_PARSERS[] = {
    {CMD_MUTE, make_mute},
    {CMD_HREVERSE, make_h_reverse},
    {CMD_VREVERSE, make_v_reverse},
    {CMD_FREEZE, make_freeze},
};

constexpr StringEntry STRING_PARSERS[] = {
    {CMD_SOURCE, make_source},       {CMD_COLOR_MODE, make_color_mode}, {CMD_ASPECT, make_aspect_ratio},
    {CMD_LUMINANCE, make_luminance}, {CMD_GAMMA, make_gamma},           {CMD_SERIAL, make_serial},
};

}  // namespace

bool ResponseParser::is_complete_response(const std::string &buffer) const {
  return !buffer.empty() && buffer.back() == RESPONSE_PROMPT;
}

compat::expected<ParseResult, std::string> ResponseParser::parse(const std::string &response) {
  if (response.empty()) {
    return compat::unexpected("Empty response");
  }

  std::string trimmed = response;
  while (!trimmed.empty() && (trimmed.back() == RESPONSE_PROMPT || trimmed.back() == CMD_TERMINATOR ||
                              std::isspace(static_cast<unsigned char>(trimmed.back())))) {
    trimmed.pop_back();
  }

  if (trimmed.empty()) {
    return AckResponse{};
  }

  if (trimmed == RESPONSE_ERR) {
    return compat::unexpected("Command error");
  }

  auto sep_pos = trimmed.find(RESPONSE_SEPARATOR);
  if (sep_pos == std::string::npos) {
    return compat::unexpected("Invalid response format: " + trimmed);
  }

  std::string key = trimmed.substr(0, sep_pos);
  std::string value = trimmed.substr(sep_pos + 1);

  return parse_key_value(key, value);
}

compat::expected<ParseResult, std::string> ResponseParser::parse_key_value(const std::string &key,
                                                                           const std::string &value) {
  if (key == CMD_POWER) {
    auto state_val = safe_stoi(value);
    if (!state_val) {
      return compat::unexpected("Invalid power state value: " + value);
    }
    PowerState state;
    switch (*state_val) {
      case 0:
        state = PowerState::STANDBY;
        break;
      case 1:
        state = PowerState::ON;
        break;
      case 2:
        state = PowerState::WARMUP;
        break;
      case 3:
        state = PowerState::COOLDOWN;
        break;
      default:
        state = PowerState::UNKNOWN;
        break;
    }
    return PowerResponse{state};
  }

  if (key == CMD_LAMP) {
    auto hours = safe_stoul(value);
    if (!hours) {
      return compat::unexpected("Invalid lamp hours value: " + value);
    }
    return LampResponse{*hours};
  }

  if (key == CMD_ERROR) {
    auto code = safe_stoi(value);
    if (!code) {
      return compat::unexpected("Invalid error code value: " + value);
    }
    return ErrorResponse{static_cast<uint8_t>(*code)};
  }

  for (const auto &entry : SCALED_INT_PARSERS) {
    if (key == entry.cmd) {
      auto raw_value = safe_stoi(value);
      if (!raw_value) {
        return compat::unexpected("Invalid " + std::string(entry.cmd) + " value: " + value);
      }
      int scaled = (*raw_value * entry.ui_max) / PROJECTOR_RAW_MAX;
      return entry.make(scaled);
    }
  }

  for (const auto &entry : BOOL_PARSERS) {
    if (key == entry.cmd) {
      return entry.make(is_bool_true(value));
    }
  }

  for (const auto &entry : STRING_PARSERS) {
    if (key == entry.cmd) {
      return entry.make(value);
    }
  }

  return StringResponse{value};
}

}  // namespace esphome::epson_projector
