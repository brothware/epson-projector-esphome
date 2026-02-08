#include "response_parser.h"

#include <algorithm>
#include <cctype>
#include <format>
#include <stdexcept>

namespace esphome::epson_projector {

namespace {

std::expected<int, std::string> safe_stoi(const std::string &str) {
  try {
    return std::stoi(str);
  } catch (const std::exception &) {
    return std::unexpected(std::format("Invalid integer: '{}'", str));
  }
}

std::expected<uint32_t, std::string> safe_stoul(const std::string &str) {
  try {
    return static_cast<uint32_t>(std::stoul(str));
  } catch (const std::exception &) {
    return std::unexpected(std::format("Invalid unsigned integer: '{}'", str));
  }
}

bool is_bool_true(const std::string &value) {
  return value == ARG_ON || value == ARG_ON_NUMERIC;
}

}  // namespace

bool ResponseParser::is_complete_response(const std::string &buffer) const {
  return !buffer.empty() && buffer.back() == RESPONSE_PROMPT;
}

std::expected<ParseResult, std::string> ResponseParser::parse(const std::string &response) {
  if (response.empty()) {
    return std::unexpected("Empty response");
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
    return std::unexpected("Command error");
  }

  if (!trimmed.contains(RESPONSE_SEPARATOR)) {
    return std::unexpected(std::format("Invalid response format: '{}'", trimmed));
  }

  auto sep_pos = trimmed.find(RESPONSE_SEPARATOR);
  std::string key = trimmed.substr(0, sep_pos);
  std::string value = trimmed.substr(sep_pos + 1);

  return parse_key_value(key, value);
}

std::expected<ParseResult, std::string> ResponseParser::parse_key_value(const std::string &key,
                                                                        const std::string &value) {
  if (key == CMD_POWER) {
    auto state_val = safe_stoi(value);
    if (!state_val) {
      return std::unexpected(std::format("Invalid power state value: '{}'", value));
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
    return PowerResponse{.state = state};
  }

  if (key == CMD_LAMP) {
    auto hours = safe_stoul(value);
    if (!hours) {
      return std::unexpected(std::format("Invalid lamp hours value: '{}'", value));
    }
    return LampResponse{.hours = *hours};
  }

  if (key == CMD_ERROR) {
    auto code = safe_stoi(value);
    if (!code) {
      return std::unexpected(std::format("Invalid error code value: '{}'", value));
    }
    return ErrorResponse{.code = static_cast<uint8_t>(*code)};
  }

  if (key == CMD_SOURCE) {
    return SourceResponse{.source_code = value};
  }

  if (key == CMD_MUTE) {
    return MuteResponse{.muted = is_bool_true(value)};
  }

  if (key == CMD_VOLUME) {
    auto vol = safe_stoi(value);
    if (!vol) {
      return std::unexpected(std::format("Invalid volume value: '{}'", value));
    }
    return VolumeResponse{.value = *vol};
  }

  if (key == CMD_BRIGHTNESS) {
    auto raw_value = safe_stoi(value);
    if (!raw_value) {
      return std::unexpected(std::format("Invalid brightness value: '{}'", value));
    }
    int scaled = (*raw_value * BRIGHTNESS_MAX) / PROJECTOR_BRIGHTNESS_MAX;
    return BrightnessResponse{.value = scaled};
  }

  if (key == CMD_CONTRAST) {
    auto raw_value = safe_stoi(value);
    if (!raw_value) {
      return std::unexpected(std::format("Invalid contrast value: '{}'", value));
    }
    int scaled = (*raw_value * CONTRAST_MAX) / PROJECTOR_CONTRAST_MAX;
    return ContrastResponse{.value = scaled};
  }

  if (key == CMD_COLOR_MODE) {
    return ColorModeResponse{.mode_code = value};
  }

  if (key == CMD_ASPECT) {
    return AspectRatioResponse{.ratio_code = value};
  }

  if (key == CMD_SHARPNESS) {
    auto val = safe_stoi(value);
    if (!val) {
      return std::unexpected(std::format("Invalid sharpness value: '{}'", value));
    }
    return SharpnessResponse{.value = *val};
  }

  if (key == CMD_DENSITY) {
    auto val = safe_stoi(value);
    if (!val) {
      return std::unexpected(std::format("Invalid density value: '{}'", value));
    }
    return DensityResponse{.value = *val};
  }

  if (key == CMD_TINT) {
    auto val = safe_stoi(value);
    if (!val) {
      return std::unexpected(std::format("Invalid tint value: '{}'", value));
    }
    return TintResponse{.value = *val};
  }

  if (key == CMD_COLOR_TEMP) {
    auto val = safe_stoi(value);
    if (!val) {
      return std::unexpected(std::format("Invalid color temperature value: '{}'", value));
    }
    return ColorTempResponse{.value = *val};
  }

  if (key == CMD_VKEYSTONE) {
    auto val = safe_stoi(value);
    if (!val) {
      return std::unexpected(std::format("Invalid vertical keystone value: '{}'", value));
    }
    return VKeystoneResponse{.value = *val};
  }

  if (key == CMD_HKEYSTONE) {
    auto val = safe_stoi(value);
    if (!val) {
      return std::unexpected(std::format("Invalid horizontal keystone value: '{}'", value));
    }
    return HKeystoneResponse{.value = *val};
  }

  if (key == CMD_HREVERSE) {
    return HReverseResponse{.reversed = is_bool_true(value)};
  }

  if (key == CMD_VREVERSE) {
    return VReverseResponse{.reversed = is_bool_true(value)};
  }

  if (key == CMD_LUMINANCE) {
    return LuminanceResponse{.mode_code = value};
  }

  if (key == CMD_GAMMA) {
    return GammaResponse{.mode_code = value};
  }

  if (key == CMD_FREEZE) {
    return FreezeResponse{.frozen = is_bool_true(value)};
  }

  if (key == CMD_SERIAL) {
    return SerialNumberResponse{.serial = value};
  }

  return StringResponse{.value = value};
}

}  // namespace esphome::epson_projector
