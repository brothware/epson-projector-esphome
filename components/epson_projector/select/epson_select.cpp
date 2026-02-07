#include "epson_select.h"

#include "esphome/core/log.h"

namespace esphome::epson_projector {

static const char *const TAG = "epson_projector.select";

void EpsonSelect::set_options_map(const std::map<std::string, std::string> &options_map) {
  this->options_map_ = options_map;
  this->reverse_map_.clear();

  std::vector<std::string> options;
  for (const auto &[name, code] : options_map) {
    options.push_back(name);
    this->reverse_map_[code] = name;
  }
  this->traits.set_options(options);
}

void EpsonSelect::setup() {
  if (this->parent_ == nullptr) {
    ESP_LOGE(TAG, "Parent not set");
    this->mark_failed();
    return;
  }

  this->parent_->add_on_state_callback([this]() { this->on_state_change(); });
}

void EpsonSelect::dump_config() {
  LOG_SELECT("", "Epson Projector Select", this);
  const char *type_str = "Unknown";
  switch (this->select_type_) {
    case SelectType::SOURCE:
      type_str = "Source";
      break;
    case SelectType::COLOR_MODE:
      type_str = "Color Mode";
      break;
    case SelectType::ASPECT_RATIO:
      type_str = "Aspect Ratio";
      break;
  }
  ESP_LOGCONFIG(TAG, "  Type: %s", type_str);
}

void EpsonSelect::control(const std::string &value) {
  auto it = this->options_map_.find(value);
  if (it == this->options_map_.end()) {
    ESP_LOGW(TAG, "Unknown option: %s", value.c_str());
    return;
  }

  const std::string &code = it->second;
  switch (this->select_type_) {
    case SelectType::SOURCE:
      this->parent_->set_source(code);
      break;
    case SelectType::COLOR_MODE:
    case SelectType::ASPECT_RATIO:
      ESP_LOGW(TAG, "Not implemented yet");
      break;
  }
  this->publish_state(value);
}

void EpsonSelect::on_state_change() {
  if (this->select_type_ != SelectType::SOURCE) {
    return;
  }

  const std::string &current = this->parent_->current_source();
  auto it = this->reverse_map_.find(current);
  if (it != this->reverse_map_.end()) {
    this->publish_state(it->second);
  }
}

}  // namespace esphome::epson_projector
