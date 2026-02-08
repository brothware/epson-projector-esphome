#pragma once

#include "query_metadata.h"

#include <cstddef>

namespace esphome::epson_projector {

class EpsonProjector;

enum class NumberType : uint8_t {
  BRIGHTNESS,
  CONTRAST,
  VOLUME,
  SHARPNESS,
  DENSITY,
  TINT,
  COLOR_TEMPERATURE,
  V_KEYSTONE,
  H_KEYSTONE,
};

struct NumberTypeInfo {
  NumberType type;
  QueryType query_type;
  const char *name;
};

inline constexpr NumberTypeInfo NUMBER_TYPE_INFO[] = {
    {NumberType::BRIGHTNESS, QueryType::BRIGHTNESS, "Brightness"},
    {NumberType::CONTRAST, QueryType::CONTRAST, "Contrast"},
    {NumberType::VOLUME, QueryType::VOLUME, "Volume"},
    {NumberType::SHARPNESS, QueryType::SHARPNESS, "Sharpness"},
    {NumberType::DENSITY, QueryType::DENSITY, "Density"},
    {NumberType::TINT, QueryType::TINT, "Tint"},
    {NumberType::COLOR_TEMPERATURE, QueryType::COLOR_TEMP, "Color Temperature"},
    {NumberType::V_KEYSTONE, QueryType::V_KEYSTONE, "V Keystone"},
    {NumberType::H_KEYSTONE, QueryType::H_KEYSTONE, "H Keystone"},
};

inline constexpr std::size_t NUMBER_TYPE_INFO_SIZE = sizeof(NUMBER_TYPE_INFO) / sizeof(NUMBER_TYPE_INFO[0]);

constexpr const NumberTypeInfo *find_number_type_info(NumberType type) {
  for (const auto &info : NUMBER_TYPE_INFO) {
    if (info.type == type) {
      return &info;
    }
  }
  return nullptr;
}

enum class SwitchType : uint8_t {
  POWER,
  MUTE,
  H_REVERSE,
  V_REVERSE,
  FREEZE,
};

struct SwitchTypeInfo {
  SwitchType type;
  QueryType query_type;
  const char *name;
};

inline constexpr SwitchTypeInfo SWITCH_TYPE_INFO[] = {
    {SwitchType::POWER, QueryType::POWER, "Power"},
    {SwitchType::MUTE, QueryType::MUTE, "Mute"},
    {SwitchType::H_REVERSE, QueryType::H_REVERSE, "H Reverse"},
    {SwitchType::V_REVERSE, QueryType::V_REVERSE, "V Reverse"},
    {SwitchType::FREEZE, QueryType::FREEZE, "Freeze"},
};

inline constexpr std::size_t SWITCH_TYPE_INFO_SIZE = sizeof(SWITCH_TYPE_INFO) / sizeof(SWITCH_TYPE_INFO[0]);

constexpr const SwitchTypeInfo *find_switch_type_info(SwitchType type) {
  for (const auto &info : SWITCH_TYPE_INFO) {
    if (info.type == type) {
      return &info;
    }
  }
  return nullptr;
}

enum class SelectType : uint8_t {
  SOURCE,
  COLOR_MODE,
  ASPECT_RATIO,
  LUMINANCE,
  GAMMA,
};

struct SelectTypeInfo {
  SelectType type;
  QueryType query_type;
  const char *name;
};

inline constexpr SelectTypeInfo SELECT_TYPE_INFO[] = {
    {SelectType::SOURCE, QueryType::SOURCE, "Source"},
    {SelectType::COLOR_MODE, QueryType::COLOR_MODE, "Color Mode"},
    {SelectType::ASPECT_RATIO, QueryType::ASPECT_RATIO, "Aspect Ratio"},
    {SelectType::LUMINANCE, QueryType::LUMINANCE, "Luminance"},
    {SelectType::GAMMA, QueryType::GAMMA, "Gamma"},
};

inline constexpr std::size_t SELECT_TYPE_INFO_SIZE = sizeof(SELECT_TYPE_INFO) / sizeof(SELECT_TYPE_INFO[0]);

constexpr const SelectTypeInfo *find_select_type_info(SelectType type) {
  for (const auto &info : SELECT_TYPE_INFO) {
    if (info.type == type) {
      return &info;
    }
  }
  return nullptr;
}

enum class SensorType : uint8_t {
  LAMP_HOURS,
  ERROR_CODE,
};

struct SensorTypeInfo {
  SensorType type;
  QueryType query_type;
  const char *name;
};

inline constexpr SensorTypeInfo SENSOR_TYPE_INFO[] = {
    {SensorType::LAMP_HOURS, QueryType::LAMP_HOURS, "Lamp Hours"},
    {SensorType::ERROR_CODE, QueryType::ERROR_CODE, "Error Code"},
};

inline constexpr std::size_t SENSOR_TYPE_INFO_SIZE = sizeof(SENSOR_TYPE_INFO) / sizeof(SENSOR_TYPE_INFO[0]);

constexpr const SensorTypeInfo *find_sensor_type_info(SensorType type) {
  for (const auto &info : SENSOR_TYPE_INFO) {
    if (info.type == type) {
      return &info;
    }
  }
  return nullptr;
}

enum class BinarySensorType : uint8_t {
  POWER_STATE,
  MUTE_STATE,
};

struct BinarySensorTypeInfo {
  BinarySensorType type;
  QueryType query_type;
  const char *name;
};

inline constexpr BinarySensorTypeInfo BINARY_SENSOR_TYPE_INFO[] = {
    {BinarySensorType::POWER_STATE, QueryType::POWER, "Power State"},
    {BinarySensorType::MUTE_STATE, QueryType::MUTE, "Mute State"},
};

inline constexpr std::size_t BINARY_SENSOR_TYPE_INFO_SIZE =
    sizeof(BINARY_SENSOR_TYPE_INFO) / sizeof(BINARY_SENSOR_TYPE_INFO[0]);

constexpr const BinarySensorTypeInfo *find_binary_sensor_type_info(BinarySensorType type) {
  for (const auto &info : BINARY_SENSOR_TYPE_INFO) {
    if (info.type == type) {
      return &info;
    }
  }
  return nullptr;
}

enum class TextSensorType : uint8_t {
  SERIAL_NUMBER,
};

struct TextSensorTypeInfo {
  TextSensorType type;
  QueryType query_type;
  const char *name;
};

inline constexpr TextSensorTypeInfo TEXT_SENSOR_TYPE_INFO[] = {
    {TextSensorType::SERIAL_NUMBER, QueryType::SERIAL_NUMBER, "Serial Number"},
};

inline constexpr std::size_t TEXT_SENSOR_TYPE_INFO_SIZE =
    sizeof(TEXT_SENSOR_TYPE_INFO) / sizeof(TEXT_SENSOR_TYPE_INFO[0]);

constexpr const TextSensorTypeInfo *find_text_sensor_type_info(TextSensorType type) {
  for (const auto &info : TEXT_SENSOR_TYPE_INFO) {
    if (info.type == type) {
      return &info;
    }
  }
  return nullptr;
}

}  // namespace esphome::epson_projector
