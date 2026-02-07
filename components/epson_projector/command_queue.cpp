#include "command_queue.h"

namespace esphome::epson_projector {

void CommandQueue::enqueue(Command cmd) { queue_.push_back(std::move(cmd)); }

void CommandQueue::enqueue_priority(Command cmd) { queue_.push_front(std::move(cmd)); }

std::optional<Command> CommandQueue::dequeue() {
  if (queue_.empty()) {
    return std::nullopt;
  }
  Command cmd = std::move(queue_.front());
  queue_.pop_front();
  return cmd;
}

bool CommandQueue::empty() const { return queue_.empty(); }

size_t CommandQueue::size() const { return queue_.size(); }

void CommandQueue::clear() {
  queue_.clear();
  pending_command_.reset();
}

void CommandQueue::set_pending(Command cmd) { pending_command_ = std::move(cmd); }

void CommandQueue::clear_pending() { pending_command_.reset(); }

void CommandQueue::retry_pending() {
  if (pending_command_.has_value() && pending_command_->retry_count < Command::MAX_RETRIES) {
    pending_command_->retry_count++;
    queue_.push_front(std::move(*pending_command_));
    pending_command_.reset();
  }
}

}  // namespace esphome::epson_projector
