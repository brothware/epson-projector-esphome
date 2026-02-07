#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"

namespace esphome::epson_projector {

class EpsonProjector;

template <typename T>
class Parented {
 public:
  void set_parent(T *parent) { this->parent_ = parent; }
  T *get_parent() const { return this->parent_; }

 protected:
  T *parent_{nullptr};
};

template <typename Entity>
bool setup_entity(Entity *entity, const char *tag) {
  auto *parent = entity->get_parent();
  if (parent == nullptr) {
    ESP_LOGE(tag, "Parent not set");
    entity->mark_failed();
    return false;
  }
  parent->add_on_state_callback([entity]() { entity->on_state_change(); });
  return true;
}

}  // namespace esphome::epson_projector
