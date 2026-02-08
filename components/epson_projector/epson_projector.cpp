#include "epson_projector.h"

#include "esphome/core/log.h"

#include <algorithm>
#include <ranges>

namespace esphome::epson_projector {

static const char *const TAG = "epson_projector";

void EpsonProjector::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Epson Projector...");
  this->rx_buffer_.reserve(64);
}

void EpsonProjector::loop() {
  while (this->available()) {
    char c = this->read();
    this->rx_buffer_ += c;

    if (this->response_parser_.is_complete_response(this->rx_buffer_)) {
      ESP_LOGD(TAG, "Raw response: '%s' (len=%d)", this->format_response_for_log(this->rx_buffer_).c_str(),
               this->rx_buffer_.length());
      this->handle_response(this->rx_buffer_);
      this->rx_buffer_.clear();
    }
  }

  uint32_t now = millis();
  if (this->command_queue_.has_pending_command()) {
    uint32_t timeout = this->is_busy_state() ? BUSY_TIMEOUT_MS : RESPONSE_TIMEOUT_MS;
    if (now - this->last_command_time_ > timeout) {
      if (!this->is_busy_state()) {
        ESP_LOGW(TAG, "Command timeout");
      }
      auto &pending = this->command_queue_.pending_command();
      if (pending && pending->callback) {
        pending->callback(false, "");
      }
      this->command_queue_.retry_pending();
      this->last_command_time_ = now;
    }
  } else if (!this->command_queue_.empty()) {
    if (now - this->last_command_time_ > COMMAND_DELAY_MS) {
      this->process_queue();
    }
  }
}

std::string EpsonProjector::format_response_for_log(const std::string &response) {
  std::string result;
  for (char c : response) {
    if (c == '\r') {
      result += "\\r";
    } else if (c == '\n') {
      result += "\\n";
    } else if (c < 32 || c > 126) {
      char hex[8];
      snprintf(hex, sizeof(hex), "\\x%02X", static_cast<unsigned char>(c));
      result += hex;
    } else {
      result += c;
    }
  }
  return result;
}

bool EpsonProjector::is_busy_state() const {
  return this->power_state_ == PowerState::WARMUP || this->power_state_ == PowerState::COOLDOWN;
}

void EpsonProjector::update() {
  bool is_on = (this->power_state_ == PowerState::ON);
  auto should_query = [this, is_on](const QueryInfo &info) {
    return this->has_query(info.type) && (!info.requires_power_on || is_on);
  };
  for (const auto &info : QUERY_TABLE | std::views::filter(should_query)) {
    this->query(info.type);
  }
}

void EpsonProjector::dump_config() {
  ESP_LOGCONFIG(TAG, "Epson Projector:");
  ESP_LOGCONFIG(TAG, "  Power State: %d", compat::to_underlying(this->power_state_));
  ESP_LOGCONFIG(TAG, "  Lamp Hours: %u", this->lamp_hours_);
}

void EpsonProjector::send_int_command(const char *cmd, int min_val, int max_val, int value,
                                      int EpsonProjector::*member) {
  int clamped = clamp_value(value, min_val, max_val);
  std::string cmd_str = build_set_command(cmd, clamped);
  this->send_command(cmd_str, CommandType::SET, [this, member, clamped](bool success, const std::string &) {
    if (success) {
      this->*member = clamped;
      this->notify_state_change();
    }
  });
}

void EpsonProjector::send_bool_command(const char *cmd, bool value, bool EpsonProjector::*member) {
  std::string cmd_str = build_set_command(cmd, value ? ARG_ON : ARG_OFF);
  this->send_command(cmd_str, CommandType::SET, [this, member, value](bool success, const std::string &) {
    if (success) {
      this->*member = value;
      this->notify_state_change();
    }
  });
}

void EpsonProjector::send_string_command(const char *cmd, const std::string &value,
                                         std::string EpsonProjector::*member) {
  std::string cmd_str = build_set_command(cmd, value.c_str());
  this->send_command(cmd_str, CommandType::SET, [this, member, value](bool success, const std::string &) {
    if (success) {
      this->*member = value;
      this->notify_state_change();
    }
  });
}

void EpsonProjector::set_power(bool on) {
  std::string cmd = on ? build_power_on_command() : build_power_off_command();
  this->send_command(cmd, CommandType::SET, [this, on](bool success, const std::string &) {
    if (success) {
      this->power_state_ = on ? PowerState::WARMUP : PowerState::COOLDOWN;
      this->notify_state_change();
    }
  });
}

void EpsonProjector::set_mute(bool mute) {
  std::string cmd = build_mute_command(mute);
  this->send_command(cmd, CommandType::SET, [this, mute](bool success, const std::string &) {
    if (success) {
      this->muted_ = mute;
      this->notify_state_change();
    }
  });
}

void EpsonProjector::set_source(const std::string &source_code) {
  if (!is_valid_source_code(source_code)) {
    ESP_LOGW(TAG, "Invalid source code: %s", source_code.c_str());
    return;
  }
  std::string cmd = build_set_command(CMD_SOURCE, source_code.c_str());
  if (cmd.empty()) {
    return;
  }
  this->send_command(cmd, CommandType::SET, [this, source_code](bool success, const std::string &) {
    if (success) {
      this->current_source_ = source_code;
      this->notify_state_change();
    }
  });
}

void EpsonProjector::set_volume(int volume) {
  int clamped = clamp_value(volume, 0, VOLUME_MAX);
  int projector_value = (clamped * PROJECTOR_RAW_MAX) / VOLUME_MAX;
  std::string cmd = build_set_command(CMD_VOLUME, projector_value);
  this->send_command(cmd, CommandType::SET, [this, clamped](bool success, const std::string &) {
    if (success) {
      this->volume_ = clamped;
      this->notify_state_change();
    }
  });
}

void EpsonProjector::set_brightness(int brightness) {
  int clamped = clamp_value(brightness, 0, BRIGHTNESS_MAX);
  int projector_value = (clamped * PROJECTOR_RAW_MAX) / BRIGHTNESS_MAX;
  std::string cmd = build_set_command(CMD_BRIGHTNESS, projector_value);
  this->send_command(cmd, CommandType::SET, [this, clamped](bool success, const std::string &) {
    if (success) {
      this->brightness_ = clamped;
      this->notify_state_change();
    }
  });
}

void EpsonProjector::set_contrast(int contrast) {
  int clamped = clamp_value(contrast, 0, CONTRAST_MAX);
  int projector_value = (clamped * PROJECTOR_RAW_MAX) / CONTRAST_MAX;
  std::string cmd = build_set_command(CMD_CONTRAST, projector_value);
  this->send_command(cmd, CommandType::SET, [this, clamped](bool success, const std::string &) {
    if (success) {
      this->contrast_ = clamped;
      this->notify_state_change();
    }
  });
}

void EpsonProjector::set_color_mode(const std::string &mode_code) {
  this->send_string_command(CMD_COLOR_MODE, mode_code, &EpsonProjector::current_color_mode_);
}

void EpsonProjector::set_aspect_ratio(const std::string &ratio_code) {
  this->send_string_command(CMD_ASPECT, ratio_code, &EpsonProjector::current_aspect_ratio_);
}

void EpsonProjector::set_sharpness(int value) {
  int clamped = clamp_value(value, 0, SHARPNESS_MAX);
  int projector_value = (clamped * PROJECTOR_RAW_MAX) / SHARPNESS_MAX;
  std::string cmd = build_set_command(CMD_SHARPNESS, projector_value);
  this->send_command(cmd, CommandType::SET, [this, clamped](bool success, const std::string &) {
    if (success) {
      this->sharpness_ = clamped;
      this->notify_state_change();
    }
  });
}

void EpsonProjector::set_density(int value) {
  int clamped = clamp_value(value, 0, DENSITY_MAX);
  int projector_value = (clamped * PROJECTOR_RAW_MAX) / DENSITY_MAX;
  std::string cmd = build_set_command(CMD_DENSITY, projector_value);
  this->send_command(cmd, CommandType::SET, [this, clamped](bool success, const std::string &) {
    if (success) {
      this->density_ = clamped;
      this->notify_state_change();
    }
  });
}

void EpsonProjector::set_tint(int value) {
  int clamped = clamp_value(value, 0, TINT_MAX);
  int projector_value = (clamped * PROJECTOR_RAW_MAX) / TINT_MAX;
  std::string cmd = build_set_command(CMD_TINT, projector_value);
  this->send_command(cmd, CommandType::SET, [this, clamped](bool success, const std::string &) {
    if (success) {
      this->tint_ = clamped;
      this->notify_state_change();
    }
  });
}

void EpsonProjector::set_color_temp(int value) {
  int clamped = clamp_value(value, 0, COLOR_TEMP_MAX);
  int projector_value = (clamped * PROJECTOR_RAW_MAX) / COLOR_TEMP_MAX;
  std::string cmd = build_set_command(CMD_COLOR_TEMP, projector_value);
  this->send_command(cmd, CommandType::SET, [this, clamped](bool success, const std::string &) {
    if (success) {
      this->color_temp_ = clamped;
      this->notify_state_change();
    }
  });
}

void EpsonProjector::set_v_keystone(int value) {
  int clamped = clamp_value(value, 0, KEYSTONE_MAX);
  int projector_value = (clamped * PROJECTOR_RAW_MAX) / KEYSTONE_MAX;
  std::string cmd = build_set_command(CMD_VKEYSTONE, projector_value);
  this->send_command(cmd, CommandType::SET, [this, clamped](bool success, const std::string &) {
    if (success) {
      this->v_keystone_ = clamped;
      this->notify_state_change();
    }
  });
}

void EpsonProjector::set_h_keystone(int value) {
  int clamped = clamp_value(value, 0, KEYSTONE_MAX);
  int projector_value = (clamped * PROJECTOR_RAW_MAX) / KEYSTONE_MAX;
  std::string cmd = build_set_command(CMD_HKEYSTONE, projector_value);
  this->send_command(cmd, CommandType::SET, [this, clamped](bool success, const std::string &) {
    if (success) {
      this->h_keystone_ = clamped;
      this->notify_state_change();
    }
  });
}

void EpsonProjector::set_h_reverse(bool reverse) {
  this->send_bool_command(CMD_HREVERSE, reverse, &EpsonProjector::h_reverse_);
}

void EpsonProjector::set_v_reverse(bool reverse) {
  this->send_bool_command(CMD_VREVERSE, reverse, &EpsonProjector::v_reverse_);
}

void EpsonProjector::set_luminance(const std::string &mode_code) {
  this->send_string_command(CMD_LUMINANCE, mode_code, &EpsonProjector::current_luminance_);
}

void EpsonProjector::set_gamma(const std::string &mode_code) {
  this->send_string_command(CMD_GAMMA, mode_code, &EpsonProjector::current_gamma_);
}

void EpsonProjector::set_freeze(bool freeze) {
  this->send_bool_command(CMD_FREEZE, freeze, &EpsonProjector::frozen_);
}

void EpsonProjector::query(QueryType type) {
  const QueryInfo *info = find_query_info(type);
  if (info == nullptr) {
    ESP_LOGW(TAG, "Unknown query type: %d", compat::to_underlying(type));
    return;
  }
  std::string cmd = build_query_command(info->cmd);
  this->send_command(cmd, CommandType::QUERY);
}

void EpsonProjector::send_command(const std::string &cmd, CommandType type,
                                  std::function<void(bool, const std::string &)> callback) {
  Command command{cmd, type, std::move(callback), 0};
  this->command_queue_.enqueue(std::move(command));
}

void EpsonProjector::process_queue() {
  auto cmd_opt = this->command_queue_.dequeue();
  if (!cmd_opt.has_value()) {
    return;
  }

  Command cmd = std::move(*cmd_opt);
  ESP_LOGV(TAG, "Sending: %s", cmd.command_str.c_str());
  this->write_str(cmd.command_str.c_str());
  this->command_queue_.set_pending(std::move(cmd));
  this->last_command_time_ = millis();
}

void EpsonProjector::handle_response(const std::string &response) {
  auto result = this->response_parser_.parse(response);
  if (!result) {
    ESP_LOGW(TAG, "Parse error: %s", result.error().c_str());
    auto &pending = this->command_queue_.pending_command();
    if (pending && pending->callback) {
      pending->callback(false, response);
    }
    this->command_queue_.clear_pending();
    return;
  }
  ESP_LOGD(TAG, "Parsed response successfully");

  auto &pending = this->command_queue_.pending_command();

  std::visit(
      [this](auto &&arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, PowerResponse>) {
          ESP_LOGD(TAG, "Power state: %d -> %d", compat::to_underlying(this->power_state_),
                   compat::to_underlying(arg.state));
          this->power_state_ = arg.state;
          this->notify_state_change();
        } else if constexpr (std::is_same_v<T, LampResponse>) {
          ESP_LOGD(TAG, "Lamp hours: %u", arg.hours);
          this->lamp_hours_ = arg.hours;
          this->notify_state_change();
        } else if constexpr (std::is_same_v<T, ErrorResponse>) {
          this->error_code_ = arg.code;
          this->notify_state_change();
        } else if constexpr (std::is_same_v<T, SourceResponse>) {
          ESP_LOGD(TAG, "Source: %s", arg.source_code.c_str());
          this->current_source_ = arg.source_code;
          this->notify_state_change();
        } else if constexpr (std::is_same_v<T, MuteResponse>) {
          ESP_LOGD(TAG, "Mute: %s", arg.muted ? "ON" : "OFF");
          this->muted_ = arg.muted;
          this->notify_state_change();
        } else if constexpr (std::is_same_v<T, VolumeResponse>) {
          ESP_LOGD(TAG, "Volume: %d", arg.value);
          this->volume_ = arg.value;
          this->notify_state_change();
        } else if constexpr (std::is_same_v<T, BrightnessResponse>) {
          ESP_LOGD(TAG, "Brightness: %d", arg.value);
          this->brightness_ = arg.value;
          this->notify_state_change();
        } else if constexpr (std::is_same_v<T, ContrastResponse>) {
          ESP_LOGD(TAG, "Contrast: %d", arg.value);
          this->contrast_ = arg.value;
          this->notify_state_change();
        } else if constexpr (std::is_same_v<T, ColorModeResponse>) {
          ESP_LOGD(TAG, "Color mode: %s", arg.mode_code.c_str());
          this->current_color_mode_ = arg.mode_code;
          this->notify_state_change();
        } else if constexpr (std::is_same_v<T, AspectRatioResponse>) {
          ESP_LOGD(TAG, "Aspect ratio: %s", arg.ratio_code.c_str());
          this->current_aspect_ratio_ = arg.ratio_code;
          this->notify_state_change();
        } else if constexpr (std::is_same_v<T, SharpnessResponse>) {
          ESP_LOGD(TAG, "Sharpness: %d", arg.value);
          this->sharpness_ = arg.value;
          this->notify_state_change();
        } else if constexpr (std::is_same_v<T, DensityResponse>) {
          ESP_LOGD(TAG, "Density: %d", arg.value);
          this->density_ = arg.value;
          this->notify_state_change();
        } else if constexpr (std::is_same_v<T, TintResponse>) {
          ESP_LOGD(TAG, "Tint: %d", arg.value);
          this->tint_ = arg.value;
          this->notify_state_change();
        } else if constexpr (std::is_same_v<T, ColorTempResponse>) {
          ESP_LOGD(TAG, "Color temperature: %d", arg.value);
          this->color_temp_ = arg.value;
          this->notify_state_change();
        } else if constexpr (std::is_same_v<T, VKeystoneResponse>) {
          ESP_LOGD(TAG, "V Keystone: %d", arg.value);
          this->v_keystone_ = arg.value;
          this->notify_state_change();
        } else if constexpr (std::is_same_v<T, HKeystoneResponse>) {
          ESP_LOGD(TAG, "H Keystone: %d", arg.value);
          this->h_keystone_ = arg.value;
          this->notify_state_change();
        } else if constexpr (std::is_same_v<T, HReverseResponse>) {
          ESP_LOGD(TAG, "H Reverse: %s", arg.reversed ? "ON" : "OFF");
          this->h_reverse_ = arg.reversed;
          this->notify_state_change();
        } else if constexpr (std::is_same_v<T, VReverseResponse>) {
          ESP_LOGD(TAG, "V Reverse: %s", arg.reversed ? "ON" : "OFF");
          this->v_reverse_ = arg.reversed;
          this->notify_state_change();
        } else if constexpr (std::is_same_v<T, LuminanceResponse>) {
          ESP_LOGD(TAG, "Luminance: %s", arg.mode_code.c_str());
          this->current_luminance_ = arg.mode_code;
          this->notify_state_change();
        } else if constexpr (std::is_same_v<T, GammaResponse>) {
          ESP_LOGD(TAG, "Gamma: %s", arg.mode_code.c_str());
          this->current_gamma_ = arg.mode_code;
          this->notify_state_change();
        } else if constexpr (std::is_same_v<T, FreezeResponse>) {
          ESP_LOGD(TAG, "Freeze: %s", arg.frozen ? "ON" : "OFF");
          this->frozen_ = arg.frozen;
          this->notify_state_change();
        } else if constexpr (std::is_same_v<T, SerialNumberResponse>) {
          ESP_LOGD(TAG, "Serial Number: %s", arg.serial.c_str());
          this->serial_number_ = arg.serial;
          this->notify_state_change();
        } else if constexpr (std::is_same_v<T, NumericResponse>) {
          ESP_LOGD(TAG, "Numeric response: %d", arg.value);
        } else if constexpr (std::is_same_v<T, AckResponse>) {
          ESP_LOGD(TAG, "Command acknowledged");
        }
      },
      *result);

  if (pending && pending->callback) {
    pending->callback(true, response);
  }
  this->command_queue_.clear_pending();
}

void EpsonProjector::notify_state_change() {
  ESP_LOGV(TAG, "Notifying %d callbacks", this->state_callbacks_.size());
  std::ranges::for_each(this->state_callbacks_, [](auto &cb) { cb(); });
}

}  // namespace esphome::epson_projector
