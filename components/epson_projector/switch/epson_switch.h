#pragma once

#include "esphome/components/switch/switch.h"
#include "esphome/core/component.h"

#include "../entity_base.h"
#include "../epson_projector.h"

namespace esphome::epson_projector {

enum class SwitchType : uint8_t {
  POWER,
  MUTE,
};

class EpsonSwitch : public switch_::Switch, public Component, public Parented<EpsonProjector> {
 public:
  void setup() override;
  void dump_config() override;

  void set_switch_type(SwitchType type) { this->switch_type_ = type; }

  void on_state_change();

 protected:
  void write_state(bool state) override;

  SwitchType switch_type_{SwitchType::POWER};
};

}  // namespace esphome::epson_projector
