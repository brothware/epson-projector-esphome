#include "epson_switch.h"

#include "esphome/core/log.h"

namespace esphome::epson_projector {

static const char *const TAG = "epson_projector.switch";

void EpsonSwitch::setup() {
  if (!setup_entity(this, TAG)) {
    return;
  }
  const auto *info = find_switch_type_info(this->switch_type_);
  if (info != nullptr) {
    this->parent_->register_query(info->query_type);
  }
}

void EpsonSwitch::dump_config() {
  LOG_SWITCH("", "Epson Projector Switch", this);
  const auto *info = find_switch_type_info(this->switch_type_);
  ESP_LOGCONFIG(TAG, "  Type: %s", info != nullptr ? info->name : "Unknown");
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
    case SwitchType::FREEZE:
      this->parent_->set_freeze(state);
      break;
  }
}

void EpsonSwitch::on_state_change() {
  const auto *info = find_switch_type_info(this->switch_type_);
  if (info == nullptr || !this->parent_->has_received(info->query_type)) {
    return;
  }
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
    case SwitchType::FREEZE:
      state = this->parent_->is_frozen();
      break;
  }
  this->publish_state(state);
}

}  // namespace esphome::epson_projector
