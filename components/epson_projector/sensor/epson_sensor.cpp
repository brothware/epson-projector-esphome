#include "epson_sensor.h"

#include "esphome/core/log.h"

namespace esphome::epson_projector {

static const char *const TAG = "epson_projector.sensor";

void EpsonSensor::setup() {
  if (this->parent_ == nullptr) {
    ESP_LOGE(TAG, "Parent not set");
    this->mark_failed();
    return;
  }

  this->parent_->add_on_state_callback([this]() { this->on_state_change(); });
}

void EpsonSensor::dump_config() {
  LOG_SENSOR("", "Epson Projector Sensor", this);
  const char *type_str = "Unknown";
  switch (this->sensor_type_) {
    case SensorType::LAMP_HOURS:
      type_str = "Lamp Hours";
      break;
    case SensorType::ERROR_CODE:
      type_str = "Error Code";
      break;
  }
  ESP_LOGCONFIG(TAG, "  Type: %s", type_str);
}

void EpsonSensor::on_state_change() {
  float value = NAN;
  switch (this->sensor_type_) {
    case SensorType::LAMP_HOURS:
      value = static_cast<float>(this->parent_->lamp_hours());
      break;
    case SensorType::ERROR_CODE:
      value = static_cast<float>(this->parent_->error_code());
      break;
  }
  this->publish_state(value);
}

}  // namespace esphome::epson_projector
