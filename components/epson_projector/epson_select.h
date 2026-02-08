#pragma once

#include "esphome/components/select/select.h"
#include "esphome/core/component.h"

#include "entity_base.h"
#include "entity_metadata.h"
#include "epson_projector.h"

#include <map>
#include <string>
#include <vector>

namespace esphome::epson_projector {

class EpsonSelect : public select::Select, public Component, public Parented<EpsonProjector> {
 public:
  void setup() override;
  void dump_config() override;

  void set_select_type(SelectType type) { this->select_type_ = type; }
  void set_options_map(const std::map<std::string, std::string> &options_map);
  void add_option(const std::string &name, const std::string &code);

  void on_state_change();

 protected:
  void control(const std::string &value) override;

  SelectType select_type_{SelectType::SOURCE};
  std::map<std::string, std::string> options_map_;
  std::map<std::string, std::string> reverse_map_;
  std::vector<std::string> option_names_;
};

}  // namespace esphome::epson_projector
