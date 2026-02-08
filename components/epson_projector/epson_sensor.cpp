#include "epson_sensor.h"

#include "esphome/core/log.h"

namespace esphome::epson_projector {

static const char *const TAG = "epson_projector.sensor";

void EpsonSensor::setup() {
  ESP_LOGD(TAG, "Setting up sensor type %d", static_cast<int>(this->sensor_type_));
  if (!setup_entity(this, TAG)) {
    return;
  }
  switch (this->sensor_type_) {
    case SensorType::LAMP_HOURS:
      this->parent_->register_query(QueryType::LAMP_HOURS);
      break;
    case SensorType::ERROR_CODE:
      this->parent_->register_query(QueryType::ERROR_CODE);
      break;
  }
  ESP_LOGD(TAG, "Sensor setup complete, callback registered");
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
      ESP_LOGD(TAG, "Publishing lamp hours: %.0f", value);
      break;
    case SensorType::ERROR_CODE:
      value = static_cast<float>(this->parent_->error_code());
      break;
  }
  this->publish_state(value);
}

}  // namespace esphome::epson_projector
