#include "epson_sensor.h"

#include "esphome/core/log.h"

namespace esphome::epson_projector {

static const char *const TAG = "epson_projector.sensor";

void EpsonSensor::setup() {
  ESP_LOGD(TAG, "Setting up sensor type %d", static_cast<int>(this->sensor_type_));
  if (!setup_entity(this, TAG)) {
    return;
  }
  const auto *info = find_sensor_type_info(this->sensor_type_);
  if (info != nullptr) {
    this->parent_->register_query(info->query_type);
  }
  ESP_LOGD(TAG, "Sensor setup complete, callback registered");
}

void EpsonSensor::dump_config() {
  LOG_SENSOR("", "Epson Projector Sensor", this);
  const auto *info = find_sensor_type_info(this->sensor_type_);
  ESP_LOGCONFIG(TAG, "  Type: %s", info != nullptr ? info->name : "Unknown");
}

void EpsonSensor::on_state_change() {
  const auto *info = find_sensor_type_info(this->sensor_type_);
  if (info == nullptr || !this->parent_->has_received(info->query_type)) {
    return;
  }
  float value = NAN;
  switch (this->sensor_type_) {
    case SensorType::LAMP_HOURS:
      value = static_cast<float>(this->parent_->lamp_hours());
      ESP_LOGD(TAG, "Publishing lamp hours: %.0f", value);
      break;
    case SensorType::ERROR_CODE:
      value = static_cast<float>(this->parent_->error_code());
      break;
  }
  this->publish_state(value);
}

}  // namespace esphome::epson_projector
