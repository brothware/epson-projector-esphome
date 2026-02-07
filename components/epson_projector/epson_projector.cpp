#include "epson_projector.h"

#include "esphome/core/log.h"

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
      ESP_LOGV(TAG, "Received: %s", this->rx_buffer_.c_str());
      this->handle_response(this->rx_buffer_);
      this->rx_buffer_.clear();
    }
  }

  uint32_t now = millis();
  if (this->command_queue_.has_pending_command()) {
    if (now - this->last_command_time_ > RESPONSE_TIMEOUT_MS) {
      ESP_LOGW(TAG, "Command timeout");
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

void EpsonProjector::update() {
  this->query_power();
  if (this->power_state_ == PowerState::ON) {
    this->query_lamp_hours();
    this->query_source();
    this->query_mute();
  }
}

void EpsonProjector::dump_config() {
  ESP_LOGCONFIG(TAG, "Epson Projector:");
  ESP_LOGCONFIG(TAG, "  Power State: %d", static_cast<int>(this->power_state_));
  ESP_LOGCONFIG(TAG, "  Lamp Hours: %u", this->lamp_hours_);
}

void EpsonProjector::set_power(bool on) {
  std::string cmd = on ? build_power_on_command() : build_power_off_command();
  this->send_command(cmd, CommandType::SET, [this, on](bool success, const std::string & /*response*/) {
    if (success) {
      this->power_state_ = on ? PowerState::WARMUP : PowerState::COOLDOWN;
      this->notify_state_change();
    }
  });
}

void EpsonProjector::set_mute(bool mute) {
  std::string cmd = build_mute_command(mute);
  this->send_command(cmd, CommandType::SET, [this, mute](bool success, const std::string & /*response*/) {
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
  this->send_command(cmd, CommandType::SET, [this, source_code](bool success, const std::string & /*response*/) {
    if (success) {
      this->current_source_ = source_code;
      this->notify_state_change();
    }
  });
}

void EpsonProjector::set_volume(int volume) {
  int clamped = clamp_value(volume, 0, 20);
  std::string cmd = build_set_command(CMD_VOLUME, clamped);
  this->send_command(cmd, CommandType::SET, [this, clamped](bool success, const std::string & /*response*/) {
    if (success) {
      this->volume_ = clamped;
      this->notify_state_change();
    }
  });
}

void EpsonProjector::set_brightness(int brightness) {
  int clamped = clamp_value(brightness, 0, 255);
  std::string cmd = build_set_command(CMD_BRIGHTNESS, clamped);
  this->send_command(cmd, CommandType::SET);
}

void EpsonProjector::set_contrast(int contrast) {
  int clamped = clamp_value(contrast, -32, 32);
  std::string cmd = build_set_command(CMD_CONTRAST, clamped);
  this->send_command(cmd, CommandType::SET);
}

void EpsonProjector::query_power() {
  std::string cmd = build_query_command(CMD_POWER);
  this->send_command(cmd, CommandType::QUERY);
}

void EpsonProjector::query_lamp_hours() {
  std::string cmd = build_query_command(CMD_LAMP);
  this->send_command(cmd, CommandType::QUERY);
}

void EpsonProjector::query_error() {
  std::string cmd = build_query_command(CMD_ERROR);
  this->send_command(cmd, CommandType::QUERY);
}

void EpsonProjector::query_source() {
  std::string cmd = build_query_command(CMD_SOURCE);
  this->send_command(cmd, CommandType::QUERY);
}

void EpsonProjector::query_mute() {
  std::string cmd = build_query_command(CMD_MUTE);
  this->send_command(cmd, CommandType::QUERY);
}

void EpsonProjector::query_volume() {
  std::string cmd = build_query_command(CMD_VOLUME);
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
  if (!result.has_value()) {
    ESP_LOGW(TAG, "Failed to parse response");
    return;
  }

  bool success = true;
  auto &pending = this->command_queue_.pending_command();

  std::visit(
      [this, &success](auto &&arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, PowerResponse>) {
          this->power_state_ = arg.state;
          this->notify_state_change();
        } else if constexpr (std::is_same_v<T, LampResponse>) {
          this->lamp_hours_ = arg.hours;
          this->notify_state_change();
        } else if constexpr (std::is_same_v<T, ErrorResponse>) {
          this->error_code_ = arg.code;
          this->notify_state_change();
        } else if constexpr (std::is_same_v<T, SourceResponse>) {
          this->current_source_ = arg.source_code;
          this->notify_state_change();
        } else if constexpr (std::is_same_v<T, MuteResponse>) {
          this->muted_ = arg.muted;
          this->notify_state_change();
        } else if constexpr (std::is_same_v<T, NumericResponse>) {
          ESP_LOGD(TAG, "Numeric response: %d", arg.value);
        } else if constexpr (std::is_same_v<T, AckResponse>) {
          ESP_LOGD(TAG, "Command acknowledged");
        } else if constexpr (std::is_same_v<T, ErrorResult>) {
          ESP_LOGW(TAG, "Error: %s", arg.message.c_str());
          success = false;
        }
      },
      *result);

  if (pending && pending->callback) {
    pending->callback(success, response);
  }
  this->command_queue_.clear_pending();
}

void EpsonProjector::notify_state_change() {
  for (auto &callback : this->state_callbacks_) {
    callback();
  }
}

}  // namespace esphome::epson_projector
