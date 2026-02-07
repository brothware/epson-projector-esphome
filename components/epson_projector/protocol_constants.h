#pragma once

#include <cstdint>

namespace esphome::epson_projector {

static constexpr char CMD_TERMINATOR = '\r';
static constexpr char RESPONSE_PROMPT = ':';
static constexpr char QUERY_SUFFIX = '?';
static constexpr char RESPONSE_SEPARATOR = '=';

static constexpr const char *CMD_POWER = "PWR";
static constexpr const char *CMD_SOURCE = "SOURCE";
static constexpr const char *CMD_MUTE = "MUTE";
static constexpr const char *CMD_VOLUME = "VOL";
static constexpr const char *CMD_LAMP = "LAMP";
static constexpr const char *CMD_ERROR = "ERR";
static constexpr const char *CMD_SERIAL = "SNO";

static constexpr const char *CMD_BRIGHTNESS = "BRIGHT";
static constexpr const char *CMD_CONTRAST = "CONTRAST";
static constexpr const char *CMD_DENSITY = "DENSITY";
static constexpr const char *CMD_TINT = "TINT";
static constexpr const char *CMD_SHARPNESS = "SHARP";
static constexpr const char *CMD_COLOR_TEMP = "CTEMP";
static constexpr const char *CMD_COLOR_MODE = "CMODE";
static constexpr const char *CMD_GAMMA = "GAMMA";
static constexpr const char *CMD_LUMINANCE = "LUMINANCE";

static constexpr const char *CMD_VKEYSTONE = "VKEYSTONE";
static constexpr const char *CMD_HKEYSTONE = "HKEYSTONE";
static constexpr const char *CMD_ASPECT = "ASPECT";
static constexpr const char *CMD_HREVERSE = "HREVERSE";
static constexpr const char *CMD_VREVERSE = "VREVERSE";

static constexpr const char *ARG_ON = "ON";
static constexpr const char *ARG_OFF = "OFF";

static constexpr const char *RESPONSE_OK = ":";
static constexpr const char *RESPONSE_ERR = "ERR";

enum class PowerState : uint8_t {
  STANDBY = 0,
  ON = 1,
  WARMUP = 2,
  COOLDOWN = 3,
  UNKNOWN = 255,
};

}  // namespace esphome::epson_projector
