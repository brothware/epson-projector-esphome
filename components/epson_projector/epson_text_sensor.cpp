#include "epson_text_sensor.h"

#include "esphome/core/log.h"

namespace esphome::epson_projector {

static const char *const TAG = "epson_projector.text_sensor";

void EpsonTextSensor::setup() {
  if (!setup_entity(this, TAG)) {
    return;
  }
  const auto *info = find_text_sensor_type_info(this->sensor_type_);
  if (info != nullptr) {
    this->parent_->register_query(info->query_type);
  }
}

void EpsonTextSensor::dump_config() {
  LOG_TEXT_SENSOR("", "Epson Projector Text Sensor", this);
  const auto *info = find_text_sensor_type_info(this->sensor_type_);
  ESP_LOGCONFIG(TAG, "  Type: %s", info != nullptr ? info->name : "Unknown");
}

void EpsonTextSensor::on_state_change() {
  const auto *info = find_text_sensor_type_info(this->sensor_type_);
  if (info == nullptr || !this->parent_->has_received(info->query_type)) {
    return;
  }
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
