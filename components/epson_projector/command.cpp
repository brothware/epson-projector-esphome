#include "command.h"

#include "protocol_constants.h"

namespace esphome::epson_projector {

std::string build_query_command(const char *cmd) {
  std::string result(cmd);
  result += QUERY_SUFFIX;
  result += CMD_TERMINATOR;
  return result;
}

std::string build_set_command(const char *cmd, const char *value) {
  std::string result(cmd);
  result += ' ';
  result += value;
  result += CMD_TERMINATOR;
  return result;
}

std::string build_set_command(const char *cmd, int value) {
  return build_set_command(cmd, std::to_string(value).c_str());
}

std::string build_power_on_command() { return build_set_command(CMD_POWER, ARG_ON); }

std::string build_power_off_command() { return build_set_command(CMD_POWER, ARG_OFF); }

std::string build_mute_command(bool mute) { return build_set_command(CMD_MUTE, mute ? ARG_ON : ARG_OFF); }

}  // namespace esphome::epson_projector
