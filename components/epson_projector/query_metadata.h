#pragma once

#include "protocol_constants.h"

#include <cstddef>
#include <cstdint>

namespace esphome::epson_projector {

enum class QueryType : uint8_t {
  POWER,
  LAMP_HOURS,
  ERROR_CODE,
  SOURCE,
  MUTE,
  VOLUME,
  BRIGHTNESS,
  CONTRAST,
  COLOR_MODE,
  ASPECT_RATIO,
  SHARPNESS,
  DENSITY,
  TINT,
  COLOR_TEMP,
  V_KEYSTONE,
  H_KEYSTONE,
  H_REVERSE,
  V_REVERSE,
  LUMINANCE,
  GAMMA,
  FREEZE,
  SERIAL_NUMBER,
};

struct QueryInfo {
  QueryType type;
  const char *cmd;
  bool requires_power_on;
};

inline constexpr QueryInfo QUERY_TABLE[] = {
    {QueryType::POWER, CMD_POWER, false},
    {QueryType::LAMP_HOURS, CMD_LAMP, true},
    {QueryType::ERROR_CODE, CMD_ERROR, true},
    {QueryType::SOURCE, CMD_SOURCE, true},
    {QueryType::MUTE, CMD_MUTE, true},
    {QueryType::VOLUME, CMD_VOLUME, true},
    {QueryType::BRIGHTNESS, CMD_BRIGHTNESS, true},
    {QueryType::CONTRAST, CMD_CONTRAST, true},
    {QueryType::COLOR_MODE, CMD_COLOR_MODE, true},
    {QueryType::ASPECT_RATIO, CMD_ASPECT, true},
    {QueryType::SHARPNESS, CMD_SHARPNESS, true},
    {QueryType::DENSITY, CMD_DENSITY, true},
    {QueryType::TINT, CMD_TINT, true},
    {QueryType::COLOR_TEMP, CMD_COLOR_TEMP, true},
    {QueryType::V_KEYSTONE, CMD_VKEYSTONE, true},
    {QueryType::H_KEYSTONE, CMD_HKEYSTONE, true},
    {QueryType::H_REVERSE, CMD_HREVERSE, true},
    {QueryType::V_REVERSE, CMD_VREVERSE, true},
    {QueryType::LUMINANCE, CMD_LUMINANCE, true},
    {QueryType::GAMMA, CMD_GAMMA, true},
    {QueryType::FREEZE, CMD_FREEZE, true},
    {QueryType::SERIAL_NUMBER, CMD_SERIAL, true},
};

inline constexpr size_t QUERY_TABLE_SIZE = sizeof(QUERY_TABLE) / sizeof(QUERY_TABLE[0]);

constexpr const QueryInfo *find_query_info(QueryType type) {
  for (const auto &info : QUERY_TABLE) {
    if (info.type == type) {
      return &info;
    }
  }
  return nullptr;
}

}  // namespace esphome::epson_projector
