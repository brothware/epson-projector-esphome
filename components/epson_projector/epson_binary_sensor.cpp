#include "epson_binary_sensor.h"

#include "esphome/core/log.h"

namespace esphome::epson_projector {

static const char *const TAG = "epson_projector.binary_sensor";

void EpsonBinarySensor::setup() {
  ESP_LOGD(TAG, "Setting up binary sensor type %d", static_cast<int>(this->sensor_type_));
  if (!setup_entity(this, TAG)) {
    return;
  }
  const auto *info = find_binary_sensor_type_info(this->sensor_type_);
  if (info != nullptr) {
    this->parent_->register_query(info->query_type);
  }
  ESP_LOGD(TAG, "Binary sensor setup complete, callback registered");
}

void EpsonBinarySensor::dump_config() {
  LOG_BINARY_SENSOR("", "Epson Projector Binary Sensor", this);
  const auto *info = find_binary_sensor_type_info(this->sensor_type_);
  ESP_LOGCONFIG(TAG, "  Type: %s", info != nullptr ? info->name : "Unknown");
}

void EpsonBinarySensor::on_state_change() {
  const auto *info = find_binary_sensor_type_info(this->sensor_type_);
  if (info == nullptr || !this->parent_->has_received(info->query_type)) {
    return;
  }
  bool state = false;
  switch (this->sensor_type_) {
    case BinarySensorType::POWER_STATE:
      state = this->parent_->power_state() == PowerState::ON || this->parent_->power_state() == PowerState::WARMUP;
      break;
    case BinarySensorType::MUTE_STATE:
      state = this->parent_->is_muted();
      break;
  }
  this->publish_state(state);
}

}  // namespace esphome::epson_projector
