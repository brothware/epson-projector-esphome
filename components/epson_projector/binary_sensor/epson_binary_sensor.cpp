#include "epson_binary_sensor.h"

#include "esphome/core/log.h"

namespace esphome::epson_projector {

static const char *const TAG = "epson_projector.binary_sensor";

void EpsonBinarySensor::setup() {
  if (this->parent_ == nullptr) {
    ESP_LOGE(TAG, "Parent not set");
    this->mark_failed();
    return;
  }

  this->parent_->add_on_state_callback([this]() { this->on_state_change(); });
}

void EpsonBinarySensor::dump_config() {
  LOG_BINARY_SENSOR("", "Epson Projector Binary Sensor", this);
  const char *type_str = "Unknown";
  switch (this->sensor_type_) {
    case BinarySensorType::POWER_STATE:
      type_str = "Power State";
      break;
    case BinarySensorType::MUTE_STATE:
      type_str = "Mute State";
      break;
  }
  ESP_LOGCONFIG(TAG, "  Type: %s", type_str);
}

void EpsonBinarySensor::on_state_change() {
  bool state = false;
  switch (this->sensor_type_) {
    case BinarySensorType::POWER_STATE:
      state = this->parent_->power_state() == PowerState::ON;
      break;
    case BinarySensorType::MUTE_STATE:
      state = this->parent_->is_muted();
      break;
  }
  this->publish_state(state);
}

}  // namespace esphome::epson_projector
