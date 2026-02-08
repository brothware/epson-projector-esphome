#include "epson_text_sensor.h"

#include "esphome/core/log.h"

namespace esphome::epson_projector {

static const char *const TAG = "epson_projector.text_sensor";

void EpsonTextSensor::setup() {
  if (!setup_entity(this, TAG)) {
    return;
  }
  switch (this->sensor_type_) {
    case TextSensorType::SERIAL_NUMBER:
      this->parent_->register_query(EpsonProjector::QueryType::SERIAL_NUMBER);
      break;
  }
}

void EpsonTextSensor::dump_config() {
  LOG_TEXT_SENSOR("", "Epson Projector Text Sensor", this);
  const char *type_str = "Unknown";
  switch (this->sensor_type_) {
    case TextSensorType::SERIAL_NUMBER:
      type_str = "Serial Number";
      break;
  }
  ESP_LOGCONFIG(TAG, "  Type: %s", type_str);
}

void EpsonTextSensor::on_state_change() {
  std::string value;
  switch (this->sensor_type_) {
    case TextSensorType::SERIAL_NUMBER:
      value = this->parent_->serial_number();
      break;
  }
  if (!value.empty()) {
    this->publish_state(value);
  }
}

}  // namespace esphome::epson_projector
