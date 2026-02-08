#pragma once

#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"

#include "entity_base.h"
#include "entity_metadata.h"
#include "epson_projector.h"

namespace esphome::epson_projector {

class EpsonSensor : public sensor::Sensor, public Component, public Parented<EpsonProjector> {
 public:
  void setup() override;
  void dump_config() override;

  void set_sensor_type(SensorType type) { this->sensor_type_ = type; }

  void on_state_change();

 protected:
  SensorType sensor_type_{SensorType::LAMP_HOURS};
};

}  // namespace esphome::epson_projector
