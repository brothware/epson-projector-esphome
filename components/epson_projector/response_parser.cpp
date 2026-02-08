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

  if (key == CMD_SOURCE) {
    return SourceResponse{value};
  }

  if (key == CMD_MUTE) {
    return MuteResponse{is_bool_true(value)};
  }

  if (key == CMD_VOLUME) {
    auto vol = safe_stoi(value);
    if (!vol) {
      return compat::unexpected("Invalid volume value: " + value);
    }
    return VolumeResponse{*vol};
  }

  if (key == CMD_BRIGHTNESS) {
    auto raw_value = safe_stoi(value);
    if (!raw_value) {
      return compat::unexpected("Invalid brightness value: " + value);
    }
    int scaled = (*raw_value * BRIGHTNESS_MAX) / PROJECTOR_BRIGHTNESS_MAX;
    return BrightnessResponse{scaled};
  }

  if (key == CMD_CONTRAST) {
    auto raw_value = safe_stoi(value);
    if (!raw_value) {
      return compat::unexpected("Invalid contrast value: " + value);
    }
    int scaled = (*raw_value * CONTRAST_MAX) / PROJECTOR_CONTRAST_MAX;
    return ContrastResponse{scaled};
  }

  if (key == CMD_COLOR_MODE) {
    return ColorModeResponse{value};
  }

  if (key == CMD_ASPECT) {
    return AspectRatioResponse{value};
  }

  if (key == CMD_SHARPNESS) {
    auto val = safe_stoi(value);
    if (!val) {
      return compat::unexpected("Invalid sharpness value: " + value);
    }
    return SharpnessResponse{*val};
  }

  if (key == CMD_DENSITY) {
    auto val = safe_stoi(value);
    if (!val) {
      return compat::unexpected("Invalid density value: " + value);
    }
    return DensityResponse{*val};
  }

  if (key == CMD_TINT) {
    auto val = safe_stoi(value);
    if (!val) {
      return compat::unexpected("Invalid tint value: " + value);
    }
    return TintResponse{*val};
  }

  if (key == CMD_COLOR_TEMP) {
    auto val = safe_stoi(value);
    if (!val) {
      return compat::unexpected("Invalid color temperature value: " + value);
    }
    return ColorTempResponse{*val};
  }

  if (key == CMD_VKEYSTONE) {
    auto val = safe_stoi(value);
    if (!val) {
      return compat::unexpected("Invalid vertical keystone value: " + value);
    }
    return VKeystoneResponse{*val};
  }

  if (key == CMD_HKEYSTONE) {
    auto val = safe_stoi(value);
    if (!val) {
      return compat::unexpected("Invalid horizontal keystone value: " + value);
    }
    return HKeystoneResponse{*val};
  }

  if (key == CMD_HREVERSE) {
    return HReverseResponse{is_bool_true(value)};
  }

  if (key == CMD_VREVERSE) {
    return VReverseResponse{is_bool_true(value)};
  }

  if (key == CMD_LUMINANCE) {
    return LuminanceResponse{value};
  }

  if (key == CMD_GAMMA) {
    return GammaResponse{value};
  }

  if (key == CMD_FREEZE) {
    return FreezeResponse{is_bool_true(value)};
  }

  if (key == CMD_SERIAL) {
    return SerialNumberResponse{value};
  }

  return StringResponse{value};
}

}  // namespace esphome::epson_projector
