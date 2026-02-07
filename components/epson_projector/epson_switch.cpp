#include "epson_switch.h"

#include "esphome/core/log.h"

namespace esphome::epson_projector {

static const char *const TAG = "epson_projector.switch";

void EpsonSwitch::setup() {
  if (!setup_entity(this, TAG)) {
    return;
  }
}

void EpsonSwitch::dump_config() {
  LOG_SWITCH("", "Epson Projector Switch", this);
  const char *type_str = "Unknown";
  switch (this->switch_type_) {
    case SwitchType::POWER:
      type_str = "Power";
      break;
    case SwitchType::MUTE:
      type_str = "Mute";
      break;
  }
  ESP_LOGCONFIG(TAG, "  Type: %s", type_str);
}

void EpsonSwitch::write_state(bool state) {
  switch (this->switch_type_) {
    case SwitchType::POWER:
      this->parent_->set_power(state);
      break;
    case SwitchType::MUTE:
      this->parent_->set_mute(state);
      break;
  }
}

void EpsonSwitch::on_state_change() {
  bool state = false;
  switch (this->switch_type_) {
    case SwitchType::POWER:
      state = this->parent_->power_state() == PowerState::ON || this->parent_->power_state() == PowerState::WARMUP;
      break;
    case SwitchType::MUTE:
      state = this->parent_->is_muted();
      break;
  }
  this->publish_state(state);
}

}  // namespace esphome::epson_projector
