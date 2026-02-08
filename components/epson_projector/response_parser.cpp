#include "response_parser.h"

#include <algorithm>
#include <cctype>

namespace esphome::epson_projector {

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
    int state_val = std::stoi(value);
    PowerState state;
    switch (state_val) {
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
    return LampResponse{static_cast<uint32_t>(std::stoul(value))};
  }

  if (key == CMD_ERROR) {
    return ErrorResponse{static_cast<uint8_t>(std::stoi(value))};
  }

  if (key == CMD_SOURCE) {
    return SourceResponse{value};
  }

  if (key == CMD_MUTE) {
    return MuteResponse{value == ARG_ON || value == "01"};
  }

  if (key == CMD_VOLUME) {
    return VolumeResponse{std::stoi(value)};
  }

  if (key == CMD_BRIGHTNESS) {
    int raw_value = std::stoi(value);
    int scaled = (raw_value * BRIGHTNESS_MAX) / PROJECTOR_BRIGHTNESS_MAX;
    return BrightnessResponse{scaled};
  }

  if (key == CMD_CONTRAST) {
    int raw_value = std::stoi(value);
    int scaled = (raw_value * CONTRAST_MAX) / PROJECTOR_CONTRAST_MAX;
    return ContrastResponse{scaled};
  }

  if (key == CMD_COLOR_MODE) {
    return ColorModeResponse{value};
  }

  if (key == CMD_ASPECT) {
    return AspectRatioResponse{value};
  }

  if (key == CMD_SHARPNESS) {
    return SharpnessResponse{std::stoi(value)};
  }

  if (key == CMD_DENSITY) {
    return DensityResponse{std::stoi(value)};
  }

  if (key == CMD_TINT) {
    return TintResponse{std::stoi(value)};
  }

  if (key == CMD_COLOR_TEMP) {
    return ColorTempResponse{std::stoi(value)};
  }

  if (key == CMD_VKEYSTONE) {
    return VKeystoneResponse{std::stoi(value)};
  }

  if (key == CMD_HKEYSTONE) {
    return HKeystoneResponse{std::stoi(value)};
  }

  if (key == CMD_HREVERSE) {
    return HReverseResponse{value == ARG_ON || value == "01"};
  }

  if (key == CMD_VREVERSE) {
    return VReverseResponse{value == ARG_ON || value == "01"};
  }

  if (key == CMD_LUMINANCE) {
    return LuminanceResponse{value};
  }

  if (key == CMD_GAMMA) {
    return GammaResponse{value};
  }

  if (key == CMD_FREEZE) {
    return FreezeResponse{value == ARG_ON || value == "01"};
  }

  if (key == CMD_SERIAL) {
    return SerialNumberResponse{value};
  }

  return StringResponse{value};
}

}  // namespace esphome::epson_projector
