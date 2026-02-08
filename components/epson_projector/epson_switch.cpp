#include "epson_switch.h"

#include "esphome/core/log.h"

namespace esphome::epson_projector {

static const char *const TAG = "epson_projector.switch";

void EpsonSwitch::setup() {
  if (!setup_entity(this, TAG)) {
    return;
  }
  switch (this->switch_type_) {
    case SwitchType::POWER:
      this->parent_->register_query(EpsonProjector::QueryType::POWER);
      break;
    case SwitchType::MUTE:
      this->parent_->register_query(EpsonProjector::QueryType::MUTE);
      break;
    case SwitchType::H_REVERSE:
      this->parent_->register_query(EpsonProjector::QueryType::H_REVERSE);
      break;
    case SwitchType::V_REVERSE:
      this->parent_->register_query(EpsonProjector::QueryType::V_REVERSE);
      break;
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
    case SwitchType::H_REVERSE:
      type_str = "H Reverse";
      break;
    case SwitchType::V_REVERSE:
      type_str = "V Reverse";
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
    case SwitchType::H_REVERSE:
      this->parent_->set_h_reverse(state);
      break;
    case SwitchType::V_REVERSE:
      this->parent_->set_v_reverse(state);
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
    case SwitchType::H_REVERSE:
      state = this->parent_->h_reverse();
      break;
    case SwitchType::V_REVERSE:
      state = this->parent_->v_reverse();
      break;
  }
  this->publish_state(state);
}

}  // namespace esphome::epson_projector
