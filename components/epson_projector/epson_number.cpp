#include "epson_number.h"

#include "esphome/core/log.h"

namespace esphome::epson_projector {

static const char *const TAG = "epson_projector.number";

void EpsonNumber::setup() {
  if (!setup_entity(this, TAG)) {
    return;
  }
  const auto *info = find_number_type_info(this->number_type_);
  if (info != nullptr) {
    this->parent_->register_query(info->query_type);
  }
}

void EpsonNumber::dump_config() {
  LOG_NUMBER("", "Epson Projector Number", this);
  const auto *info = find_number_type_info(this->number_type_);
  ESP_LOGCONFIG(TAG, "  Type: %s", info != nullptr ? info->name : "Unknown");
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
    case NumberType::SHARPNESS:
      this->parent_->set_sharpness(int_value);
      break;
    case NumberType::DENSITY:
      this->parent_->set_density(int_value);
      break;
    case NumberType::TINT:
      this->parent_->set_tint(int_value);
      break;
    case NumberType::COLOR_TEMPERATURE:
      this->parent_->set_color_temp(int_value);
      break;
    case NumberType::V_KEYSTONE:
      this->parent_->set_v_keystone(int_value);
      break;
    case NumberType::H_KEYSTONE:
      this->parent_->set_h_keystone(int_value);
      break;
  }
  this->publish_state(value);
}

void EpsonNumber::on_state_change() {
  const auto *info = find_number_type_info(this->number_type_);
  if (info == nullptr || !this->parent_->has_received(info->query_type)) {
    return;
  }
  float value = 0;
  switch (this->number_type_) {
    case NumberType::BRIGHTNESS:
      value = static_cast<float>(this->parent_->brightness());
      break;
    case NumberType::CONTRAST:
      value = static_cast<float>(this->parent_->contrast());
      break;
    case NumberType::VOLUME:
      value = static_cast<float>(this->parent_->volume());
      break;
    case NumberType::SHARPNESS:
      value = static_cast<float>(this->parent_->sharpness());
      break;
    case NumberType::DENSITY:
      value = static_cast<float>(this->parent_->density());
      break;
    case NumberType::TINT:
      value = static_cast<float>(this->parent_->tint());
      break;
    case NumberType::COLOR_TEMPERATURE:
      value = static_cast<float>(this->parent_->color_temp());
      break;
    case NumberType::V_KEYSTONE:
      value = static_cast<float>(this->parent_->v_keystone());
      break;
    case NumberType::H_KEYSTONE:
      value = static_cast<float>(this->parent_->h_keystone());
      break;
  }
  this->publish_state(value);
}

}  // namespace esphome::epson_projector
