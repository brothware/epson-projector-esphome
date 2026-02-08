#pragma once

#include "esphome/components/uart/uart.h"
#include "esphome/core/component.h"

#include "command.h"
#include "command_queue.h"
#include "protocol_constants.h"
#include "response_parser.h"

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace esphome::epson_projector {

class EpsonProjector : public uart::UARTDevice, public PollingComponent {
 public:
  void setup() override;
  void loop() override;
  void update() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  void set_power(bool on);
  void set_mute(bool mute);
  void set_source(const std::string &source_code);
  void set_volume(int volume);
  void set_brightness(int brightness);
  void set_contrast(int contrast);

  void query_power();
  void query_lamp_hours();
  void query_error();
  void query_source();
  void query_mute();
  void query_volume();

  [[nodiscard]] PowerState power_state() const { return power_state_; }
  [[nodiscard]] bool is_muted() const { return muted_; }
  [[nodiscard]] uint32_t lamp_hours() const { return lamp_hours_; }
  [[nodiscard]] uint8_t error_code() const { return error_code_; }
  [[nodiscard]] const std::string &current_source() const { return current_source_; }
  [[nodiscard]] int volume() const { return volume_; }

  using StateCallback = std::function<void()>;
  void add_on_state_callback(StateCallback callback) { state_callbacks_.push_back(std::move(callback)); }

 protected:
  void send_command(const std::string &cmd, CommandType type,
                    std::function<void(bool, const std::string &)> callback = nullptr);
  void process_queue();
  void handle_response(const std::string &response);
  void notify_state_change();
  std::string format_response_for_log(const std::string &response);
  bool is_busy_state() const;

  CommandQueue command_queue_;
  ResponseParser response_parser_;
  std::string rx_buffer_;

  PowerState power_state_{PowerState::UNKNOWN};
  bool muted_{false};
  uint32_t lamp_hours_{0};
  uint8_t error_code_{0};
  std::string current_source_;
  int volume_{0};

  uint32_t last_command_time_{0};
  static constexpr uint32_t COMMAND_DELAY_MS = 500;
  static constexpr uint32_t RESPONSE_TIMEOUT_MS = 3000;
  static constexpr uint32_t BUSY_TIMEOUT_MS = 10000;

  std::vector<StateCallback> state_callbacks_;
};

}  // namespace esphome::epson_projector
