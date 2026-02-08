#include "epson_select.h"

#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome::epson_projector {

static const char *const TAG = "epson_projector.select";

void EpsonSelect::set_options_map(const std::map<std::string, std::string> &options_map) {
  for (const auto &[name, code] : options_map) {
    this->add_option(name, code);
  }
}

void EpsonSelect::add_option(const std::string &name, const std::string &code) {
  this->options_map_[name] = code;
  this->reverse_map_[code] = name;
  this->option_names_.push_back(name);
}

void EpsonSelect::setup() {
  if (!setup_entity(this, TAG)) {
    return;
  }

  switch (this->select_type_) {
    case SelectType::SOURCE:
      this->parent_->register_query(EpsonProjector::QueryType::SOURCE);
      break;
    case SelectType::COLOR_MODE:
      this->parent_->register_query(EpsonProjector::QueryType::COLOR_MODE);
      break;
    case SelectType::ASPECT_RATIO:
      this->parent_->register_query(EpsonProjector::QueryType::ASPECT_RATIO);
      break;
  }

  if (!this->option_names_.empty()) {
    FixedVector<const char *> option_ptrs;
    option_ptrs.init(this->option_names_.size());
    for (const auto &name : this->option_names_) {
      option_ptrs.push_back(name.c_str());
    }
    this->traits.set_options(option_ptrs);
  }
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
      this->parent_->set_color_mode(code);
      break;
    case SelectType::ASPECT_RATIO:
      this->parent_->set_aspect_ratio(code);
      break;
  }
  this->publish_state(value);
}

void EpsonSelect::on_state_change() {
  std::string current;
  switch (this->select_type_) {
    case SelectType::SOURCE:
      current = this->parent_->current_source();
      break;
    case SelectType::COLOR_MODE:
      current = this->parent_->current_color_mode();
      break;
    case SelectType::ASPECT_RATIO:
      current = this->parent_->current_aspect_ratio();
      break;
  }

  auto it = this->reverse_map_.find(current);
  if (it != this->reverse_map_.end()) {
    this->publish_state(it->second);
  }
}

}  // namespace esphome::epson_projector
