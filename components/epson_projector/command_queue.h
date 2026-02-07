#pragma once

#include <cstdint>
#include <deque>
#include <functional>
#include <optional>

#include "command.h"

namespace esphome::epson_projector {

class CommandQueue {
 public:
  void enqueue(Command cmd);
  void enqueue_priority(Command cmd);
  [[nodiscard]] std::optional<Command> dequeue();
  [[nodiscard]] bool empty() const;
  [[nodiscard]] size_t size() const;
  void clear();

  [[nodiscard]] bool has_pending_command() const { return pending_command_.has_value(); }
  [[nodiscard]] const std::optional<Command> &pending_command() const { return pending_command_; }
  void set_pending(Command cmd);
  void clear_pending();
  void retry_pending();

 private:
  std::deque<Command> queue_;
  std::optional<Command> pending_command_;
};

}  // namespace esphome::epson_projector
