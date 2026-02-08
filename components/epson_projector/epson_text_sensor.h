#pragma once

#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/core/component.h"

#include "entity_base.h"
#include "epson_projector.h"

namespace esphome::epson_projector {

enum class TextSensorType : uint8_t {
  SERIAL_NUMBER,
};

class EpsonTextSensor : public text_sensor::TextSensor, public Component, public Parented<EpsonProjector> {
 public:
  void setup() override;
  void dump_config() override;

  void set_text_sensor_type(TextSensorType type) { this->sensor_type_ = type; }

  void on_state_change();

 protected:
  TextSensorType sensor_type_{TextSensorType::SERIAL_NUMBER};
};

}  // namespace esphome::epson_projector
