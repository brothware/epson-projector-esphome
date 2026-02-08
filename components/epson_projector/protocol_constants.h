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
static constexpr const char *CMD_FREEZE = "FREEZE";

static constexpr const char *ARG_ON = "ON";
static constexpr const char *ARG_OFF = "OFF";

static constexpr const char *RESPONSE_OK = ":";
static constexpr const char *RESPONSE_ERR = "ERR";

static constexpr int BRIGHTNESS_MIN = 0;
static constexpr int BRIGHTNESS_MAX = 100;
static constexpr int CONTRAST_MIN = 0;
static constexpr int CONTRAST_MAX = 100;
static constexpr int VOLUME_MIN = 0;
static constexpr int VOLUME_MAX = 20;

static constexpr int PROJECTOR_BRIGHTNESS_MAX = 255;
static constexpr int PROJECTOR_CONTRAST_MAX = 255;

static constexpr int SHARPNESS_MIN = 0;
static constexpr int SHARPNESS_MAX = 15;
static constexpr int DENSITY_MIN = -32;
static constexpr int DENSITY_MAX = 32;
static constexpr int TINT_MIN = -32;
static constexpr int TINT_MAX = 32;
static constexpr int COLOR_TEMP_MIN = 0;
static constexpr int COLOR_TEMP_MAX = 9;
static constexpr int VKEYSTONE_MIN = -30;
static constexpr int VKEYSTONE_MAX = 30;
static constexpr int HKEYSTONE_MIN = -30;
static constexpr int HKEYSTONE_MAX = 30;

enum class PowerState : uint8_t {
  STANDBY = 0,
  ON = 1,
  WARMUP = 2,
  COOLDOWN = 3,
  UNKNOWN = 255,
};

}  // namespace esphome::epson_projector
