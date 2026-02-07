#pragma once

#include "esphome/components/number/number.h"
#include "esphome/core/component.h"

#include "../epson_projector.h"

namespace esphome::epson_projector {

enum class NumberType : uint8_t {
  BRIGHTNESS,
  CONTRAST,
  VOLUME,
};

class EpsonNumber : public number::Number, public Component {
 public:
  void setup() override;
  void dump_config() override;

  void set_parent(EpsonProjector *parent) { this->parent_ = parent; }
  void set_number_type(NumberType type) { this->number_type_ = type; }

 protected:
  void control(float value) override;
  void on_state_change();

  EpsonProjector *parent_{nullptr};
  NumberType number_type_{NumberType::BRIGHTNESS};
};

}  // namespace esphome::epson_projector
