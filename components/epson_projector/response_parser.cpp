#include "response_parser.h"

#include <algorithm>
#include <cctype>
#include <stdexcept>

namespace esphome::epson_projector {

namespace {

std::optional<int> safe_stoi(const std::string &str) {
  try {
    return std::stoi(str);
  } catch (const std::exception &) {
    return std::nullopt;
  }
}

std::optional<uint32_t> safe_stoul(const std::string &str) {
  try {
    return static_cast<uint32_t>(std::stoul(str));
  } catch (const std::exception &) {
    return std::nullopt;
  }
}

bool is_bool_true(const std::string &value) {
  return value == ARG_ON || value == ARG_ON_NUMERIC;
}

}  // namespace

bool ResponseParser::is_complete_response(const std::string &buffer) const {
  return !buffer.empty() && buffer.back() == RESPONSE_PROMPT;
}

std::optional<ParseResult> ResponseParser::parse(const std::string &response) {
  if (response.empty()) {
    return ErrorResult{"Empty response"};
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
    return ErrorResult{"Command error"};
  }

  auto sep_pos = trimmed.find(RESPONSE_SEPARATOR);
  if (sep_pos == std::string::npos) {
    return ErrorResult{"Invalid response format: " + trimmed};
  }

  std::string key = trimmed.substr(0, sep_pos);
  std::string value = trimmed.substr(sep_pos + 1);

  return parse_key_value(key, value);
}

std::optional<ParseResult> ResponseParser::parse_key_value(const std::string &key, const std::string &value) {
  if (key == CMD_POWER) {
    auto state_val = safe_stoi(value);
    if (!state_val) {
      return ErrorResult{"Invalid power state value: " + value};
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
      return ErrorResult{"Invalid lamp hours value: " + value};
    }
    return LampResponse{*hours};
  }

  if (key == CMD_ERROR) {
    auto code = safe_stoi(value);
    if (!code) {
      return ErrorResult{"Invalid error code value: " + value};
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
      return ErrorResult{"Invalid volume value: " + value};
    }
    return VolumeResponse{*vol};
  }

  if (key == CMD_BRIGHTNESS) {
    auto raw_value = safe_stoi(value);
    if (!raw_value) {
      return ErrorResult{"Invalid brightness value: " + value};
    }
    int scaled = (*raw_value * BRIGHTNESS_MAX) / PROJECTOR_BRIGHTNESS_MAX;
    return BrightnessResponse{scaled};
  }

  if (key == CMD_CONTRAST) {
    auto raw_value = safe_stoi(value);
    if (!raw_value) {
      return ErrorResult{"Invalid contrast value: " + value};
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
      return ErrorResult{"Invalid sharpness value: " + value};
    }
    return SharpnessResponse{*val};
  }

  if (key == CMD_DENSITY) {
    auto val = safe_stoi(value);
    if (!val) {
      return ErrorResult{"Invalid density value: " + value};
    }
    return DensityResponse{*val};
  }

  if (key == CMD_TINT) {
    auto val = safe_stoi(value);
    if (!val) {
      return ErrorResult{"Invalid tint value: " + value};
    }
    return TintResponse{*val};
  }

  if (key == CMD_COLOR_TEMP) {
    auto val = safe_stoi(value);
    if (!val) {
      return ErrorResult{"Invalid color temperature value: " + value};
    }
    return ColorTempResponse{*val};
  }

  if (key == CMD_VKEYSTONE) {
    auto val = safe_stoi(value);
    if (!val) {
      return ErrorResult{"Invalid vertical keystone value: " + value};
    }
    return VKeystoneResponse{*val};
  }

  if (key == CMD_HKEYSTONE) {
    auto val = safe_stoi(value);
    if (!val) {
      return ErrorResult{"Invalid horizontal keystone value: " + value};
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
