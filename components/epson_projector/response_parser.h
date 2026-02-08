#pragma once

#include "protocol_constants.h"

#include <cstdint>
#include <optional>
#include <string>
#include <variant>

namespace esphome::epson_projector {

struct PowerResponse {
  PowerState state;
};

struct LampResponse {
  uint32_t hours;
};

struct ErrorResponse {
  uint8_t code;
};

struct SourceResponse {
  std::string source_code;
};

struct MuteResponse {
  bool muted;
};

struct VolumeResponse {
  int value;
};

struct BrightnessResponse {
  int value;
};

struct ContrastResponse {
  int value;
};

struct ColorModeResponse {
  std::string mode_code;
};

struct AspectRatioResponse {
  std::string ratio_code;
};

struct SharpnessResponse {
  int value;
};

struct DensityResponse {
  int value;
};

struct TintResponse {
  int value;
};

struct ColorTempResponse {
  int value;
};

struct NumericResponse {
  int value;
};

struct StringResponse {
  std::string value;
};

struct AckResponse {};

struct ErrorResult {
  std::string message;
};

using ParseResult = std::variant<PowerResponse, LampResponse, ErrorResponse, SourceResponse, MuteResponse,
                                 VolumeResponse, BrightnessResponse, ContrastResponse, ColorModeResponse,
                                 AspectRatioResponse, SharpnessResponse, DensityResponse, TintResponse,
                                 ColorTempResponse, NumericResponse, StringResponse, AckResponse, ErrorResult>;

class ResponseParser {
 public:
  [[nodiscard]] std::optional<ParseResult> parse(const std::string &response);
  [[nodiscard]] bool is_complete_response(const std::string &buffer) const;

 private:
  std::optional<ParseResult> parse_key_value(const std::string &key, const std::string &value);
};

}  // namespace esphome::epson_projector
