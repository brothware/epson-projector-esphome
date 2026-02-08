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
  void set_color_mode(const std::string &mode_code);
  void set_aspect_ratio(const std::string &ratio_code);
  void set_sharpness(int value);
  void set_density(int value);
  void set_tint(int value);
  void set_color_temp(int value);
  void set_v_keystone(int value);
  void set_h_keystone(int value);
  void set_h_reverse(bool reverse);
  void set_v_reverse(bool reverse);
  void set_luminance(const std::string &mode_code);
  void set_gamma(const std::string &mode_code);

  void query_power();
  void query_lamp_hours();
  void query_error();
  void query_source();
  void query_mute();
  void query_volume();
  void query_brightness();
  void query_contrast();
  void query_color_mode();
  void query_aspect_ratio();
  void query_sharpness();
  void query_density();
  void query_tint();
  void query_color_temp();
  void query_v_keystone();
  void query_h_keystone();
  void query_h_reverse();
  void query_v_reverse();
  void query_luminance();
  void query_gamma();

  [[nodiscard]] PowerState power_state() const { return power_state_; }
  [[nodiscard]] bool is_muted() const { return muted_; }
  [[nodiscard]] uint32_t lamp_hours() const { return lamp_hours_; }
  [[nodiscard]] uint8_t error_code() const { return error_code_; }
  [[nodiscard]] const std::string &current_source() const { return current_source_; }
  [[nodiscard]] int volume() const { return volume_; }
  [[nodiscard]] int brightness() const { return brightness_; }
  [[nodiscard]] int contrast() const { return contrast_; }
  [[nodiscard]] const std::string &current_color_mode() const { return current_color_mode_; }
  [[nodiscard]] const std::string &current_aspect_ratio() const { return current_aspect_ratio_; }
  [[nodiscard]] int sharpness() const { return sharpness_; }
  [[nodiscard]] int density() const { return density_; }
  [[nodiscard]] int tint() const { return tint_; }
  [[nodiscard]] int color_temp() const { return color_temp_; }
  [[nodiscard]] int v_keystone() const { return v_keystone_; }
  [[nodiscard]] int h_keystone() const { return h_keystone_; }
  [[nodiscard]] bool h_reverse() const { return h_reverse_; }
  [[nodiscard]] bool v_reverse() const { return v_reverse_; }
  [[nodiscard]] const std::string &current_luminance() const { return current_luminance_; }
  [[nodiscard]] const std::string &current_gamma() const { return current_gamma_; }

  using StateCallback = std::function<void()>;
  void add_on_state_callback(StateCallback callback) { state_callbacks_.push_back(std::move(callback)); }

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
  };
  void register_query(QueryType type) { registered_queries_ |= (1 << static_cast<uint16_t>(type)); }
  bool has_query(QueryType type) const { return (registered_queries_ & (1 << static_cast<uint16_t>(type))) != 0; }

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
  std::string current_color_mode_;
  std::string current_aspect_ratio_;
  int volume_{0};
  int brightness_{0};
  int contrast_{0};
  int sharpness_{0};
  int density_{0};
  int tint_{0};
  int color_temp_{0};
  int v_keystone_{0};
  int h_keystone_{0};
  bool h_reverse_{false};
  bool v_reverse_{false};
  std::string current_luminance_;
  std::string current_gamma_;

  uint32_t last_command_time_{0};
  static constexpr uint32_t COMMAND_DELAY_MS = 500;
  static constexpr uint32_t RESPONSE_TIMEOUT_MS = 3000;
  static constexpr uint32_t BUSY_TIMEOUT_MS = 10000;

  std::vector<StateCallback> state_callbacks_;
  uint16_t registered_queries_{0};
};

}  // namespace esphome::epson_projector
