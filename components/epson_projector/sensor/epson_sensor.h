#pragma once

#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"

#include "../epson_projector.h"

namespace esphome::epson_projector {

enum class SensorType : uint8_t {
  LAMP_HOURS,
  ERROR_CODE,
};

class EpsonSensor : public sensor::Sensor, public Component {
 public:
  void setup() override;
  void dump_config() override;

  void set_parent(EpsonProjector *parent) { this->parent_ = parent; }
  void set_sensor_type(SensorType type) { this->sensor_type_ = type; }

 protected:
  void on_state_change();

  EpsonProjector *parent_{nullptr};
  SensorType sensor_type_{SensorType::LAMP_HOURS};
};

}  // namespace esphome::epson_projector
