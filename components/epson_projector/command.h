#pragma once

#include <cstdint>
#include <functional>
#include <string>

namespace esphome::epson_projector {

enum class CommandType : uint8_t {
  QUERY,
  SET,
};

struct Command {
  std::string command_str;
  CommandType type;
  std::function<void(bool success, const std::string &response)> callback;
  uint8_t retry_count{0};
  static constexpr uint8_t MAX_RETRIES = 3;
};

std::string build_query_command(const char *cmd);
std::string build_set_command(const char *cmd, const char *value);
std::string build_set_command(const char *cmd, int value);
std::string build_power_on_command();
std::string build_power_off_command();
std::string build_mute_command(bool mute);

}  // namespace esphome::epson_projector
