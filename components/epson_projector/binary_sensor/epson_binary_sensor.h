#pragma once

#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/core/component.h"

#include "../entity_base.h"
#include "../epson_projector.h"

namespace esphome::epson_projector {

enum class BinarySensorType : uint8_t {
  POWER_STATE,
  MUTE_STATE,
};

class EpsonBinarySensor : public binary_sensor::BinarySensor, public Component, public Parented<EpsonProjector> {
 public:
  void setup() override;
  void dump_config() override;

  void set_sensor_type(BinarySensorType type) { this->sensor_type_ = type; }

  void on_state_change();

 protected:
  BinarySensorType sensor_type_{BinarySensorType::POWER_STATE};
};

}  // namespace esphome::epson_projector
