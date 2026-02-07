#pragma once

#include <map>
#include <string>
#include <vector>

#include "esphome/components/select/select.h"
#include "esphome/core/component.h"

#include "../epson_projector.h"

namespace esphome::epson_projector {

enum class SelectType : uint8_t {
  SOURCE,
  COLOR_MODE,
  ASPECT_RATIO,
};

class EpsonSelect : public select::Select, public Component {
 public:
  void setup() override;
  void dump_config() override;

  void set_parent(EpsonProjector *parent) { this->parent_ = parent; }
  void set_select_type(SelectType type) { this->select_type_ = type; }
  void set_options_map(const std::map<std::string, std::string> &options_map);

 protected:
  void control(const std::string &value) override;
  void on_state_change();

  EpsonProjector *parent_{nullptr};
  SelectType select_type_{SelectType::SOURCE};
  std::map<std::string, std::string> options_map_;
  std::map<std::string, std::string> reverse_map_;
};

}  // namespace esphome::epson_projector
