#include "epson_number.h"

#include "esphome/core/log.h"

namespace esphome::epson_projector {

static const char *const TAG = "epson_projector.number";

void EpsonNumber::setup() {
  if (this->parent_ == nullptr) {
    ESP_LOGE(TAG, "Parent not set");
    this->mark_failed();
    return;
  }

  this->parent_->add_on_state_callback([this]() { this->on_state_change(); });
}

void EpsonNumber::dump_config() {
  LOG_NUMBER("", "Epson Projector Number", this);
  const char *type_str = "Unknown";
  switch (this->number_type_) {
    case NumberType::BRIGHTNESS:
      type_str = "Brightness";
      break;
    case NumberType::CONTRAST:
      type_str = "Contrast";
      break;
    case NumberType::VOLUME:
      type_str = "Volume";
      break;
  }
  ESP_LOGCONFIG(TAG, "  Type: %s", type_str);
}

void EpsonNumber::control(float value) {
  int int_value = static_cast<int>(value);
  switch (this->number_type_) {
    case NumberType::BRIGHTNESS:
      this->parent_->set_brightness(int_value);
      break;
    case NumberType::CONTRAST:
      this->parent_->set_contrast(int_value);
      break;
    case NumberType::VOLUME:
      this->parent_->set_volume(int_value);
      break;
  }
  this->publish_state(value);
}

void EpsonNumber::on_state_change() {
  if (this->number_type_ == NumberType::VOLUME) {
    this->publish_state(static_cast<float>(this->parent_->volume()));
  }
}

}  // namespace esphome::epson_projector
