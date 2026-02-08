#include "epson_binary_sensor.h"

#include "esphome/core/log.h"

namespace esphome::epson_projector {

static const char *const TAG = "epson_projector.binary_sensor";

void EpsonBinarySensor::setup() {
  ESP_LOGD(TAG, "Setting up binary sensor type %d", static_cast<int>(this->sensor_type_));
  if (!setup_entity(this, TAG)) {
    return;
  }
  switch (this->sensor_type_) {
    case BinarySensorType::POWER_STATE:
      this->parent_->register_query(EpsonProjector::QueryType::POWER);
      break;
    case BinarySensorType::MUTE_STATE:
      this->parent_->register_query(EpsonProjector::QueryType::MUTE);
      break;
  }
  ESP_LOGD(TAG, "Binary sensor setup complete, callback registered");
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
