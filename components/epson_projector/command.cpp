#include "command.h"

#include "protocol_constants.h"

#include <algorithm>
#include <cctype>
#include <iterator>
#include <ranges>

namespace esphome::epson_projector {

std::string sanitize_value(const std::string &value) {
  std::string result;
  result.reserve(value.size());
  std::ranges::copy_if(value, std::back_inserter(result), [](char c) {
    return c != CMD_TERMINATOR && c != RESPONSE_PROMPT && c != '\n' && !std::iscntrl(static_cast<unsigned char>(c));
  });
  return result;
}

bool is_valid_source_code(const std::string &code) {
  if (code.empty() || code.size() > 4) {
    return false;
  }
  return std::ranges::all_of(code, [](char c) { return std::isxdigit(static_cast<unsigned char>(c)) != 0; });
}

int clamp_value(int value, int min_val, int max_val) {
  return std::clamp(value, min_val, max_val);
}

std::string build_query_command(const char *cmd) {
  std::string result(cmd);
  result += QUERY_SUFFIX;
  result += CMD_TERMINATOR;
  return result;
}

std::string build_set_command(const char *cmd, const char *value) {
  std::string sanitized = sanitize_value(value);
  if (sanitized.empty()) {
    return {};
  }
  std::string result(cmd);
  result += ' ';
  result += sanitized;
  result += CMD_TERMINATOR;
  return result;
}

std::string build_set_command(const char *cmd, int value) {
  return build_set_command(cmd, std::to_string(value).c_str());
}

std::string build_power_on_command() {
  return build_set_command(CMD_POWER, ARG_ON);
}

std::string build_power_off_command() {
  return build_set_command(CMD_POWER, ARG_OFF);
}

std::string build_mute_command(bool mute) {
  return build_set_command(CMD_MUTE, mute ? ARG_ON : ARG_OFF);
}

}  // namespace esphome::epson_projector
