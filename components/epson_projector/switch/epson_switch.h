#pragma once

#include "esphome/components/switch/switch.h"
#include "esphome/core/component.h"

#include "../epson_projector.h"

namespace esphome::epson_projector {

enum class SwitchType : uint8_t {
  POWER,
  MUTE,
};

class EpsonSwitch : public switch_::Switch, public Component {
 public:
  void setup() override;
  void dump_config() override;

  void set_parent(EpsonProjector *parent) { this->parent_ = parent; }
  void set_switch_type(SwitchType type) { this->switch_type_ = type; }

 protected:
  void write_state(bool state) override;
  void on_state_change();

  EpsonProjector *parent_{nullptr};
  SwitchType switch_type_{SwitchType::POWER};
};

}  // namespace esphome::epson_projector
