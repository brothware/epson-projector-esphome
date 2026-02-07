#include "command_queue.h"

#include <gtest/gtest.h>

namespace esphome::epson_projector {

class CommandQueueTest : public ::testing::Test {
 protected:
  CommandQueue queue;

  Command make_command(const std::string &cmd_str) { return Command{cmd_str, CommandType::QUERY, nullptr, 0}; }
};

TEST_F(CommandQueueTest, StartsEmpty) {
  EXPECT_TRUE(queue.empty());
  EXPECT_EQ(queue.size(), 0u);
}

TEST_F(CommandQueueTest, EnqueueAddsCommand) {
  queue.enqueue(make_command("PWR?\r"));
  EXPECT_FALSE(queue.empty());
  EXPECT_EQ(queue.size(), 1u);
}

TEST_F(CommandQueueTest, DequeueFIFO) {
  queue.enqueue(make_command("PWR?\r"));
  queue.enqueue(make_command("LAMP?\r"));

  auto cmd1 = queue.dequeue();
  ASSERT_TRUE(cmd1.has_value());
  EXPECT_EQ(cmd1->command_str, "PWR?\r");

  auto cmd2 = queue.dequeue();
  ASSERT_TRUE(cmd2.has_value());
  EXPECT_EQ(cmd2->command_str, "LAMP?\r");
}

TEST_F(CommandQueueTest, DequeueEmptyReturnsNullopt) {
  auto cmd = queue.dequeue();
  EXPECT_FALSE(cmd.has_value());
}

TEST_F(CommandQueueTest, EnqueuePriorityInsertsAtFront) {
  queue.enqueue(make_command("PWR?\r"));
  queue.enqueue_priority(make_command("ERR?\r"));

  auto cmd = queue.dequeue();
  ASSERT_TRUE(cmd.has_value());
  EXPECT_EQ(cmd->command_str, "ERR?\r");
}

TEST_F(CommandQueueTest, ClearEmptiesQueue) {
  queue.enqueue(make_command("PWR?\r"));
  queue.enqueue(make_command("LAMP?\r"));
  queue.clear();

  EXPECT_TRUE(queue.empty());
}

TEST_F(CommandQueueTest, PendingCommandTracking) {
  EXPECT_FALSE(queue.has_pending_command());

  Command cmd = make_command("PWR?\r");
  queue.set_pending(cmd);

  EXPECT_TRUE(queue.has_pending_command());
  ASSERT_TRUE(queue.pending_command().has_value());
  EXPECT_EQ(queue.pending_command()->command_str, "PWR?\r");

  queue.clear_pending();
  EXPECT_FALSE(queue.has_pending_command());
}

TEST_F(CommandQueueTest, RetryPendingRequeuesCommand) {
  Command cmd = make_command("PWR?\r");
  queue.set_pending(cmd);
  queue.retry_pending();

  EXPECT_FALSE(queue.has_pending_command());
  EXPECT_FALSE(queue.empty());

  auto requeued = queue.dequeue();
  ASSERT_TRUE(requeued.has_value());
  EXPECT_EQ(requeued->command_str, "PWR?\r");
  EXPECT_EQ(requeued->retry_count, 1);
}

TEST_F(CommandQueueTest, RetryIncreasesRetryCount) {
  Command cmd = make_command("PWR?\r");
  cmd.retry_count = 2;
  queue.set_pending(cmd);
  queue.retry_pending();

  auto requeued = queue.dequeue();
  ASSERT_TRUE(requeued.has_value());
  EXPECT_EQ(requeued->retry_count, 3);
}

TEST_F(CommandQueueTest, RetryStopsAtMaxRetries) {
  Command cmd = make_command("PWR?\r");
  cmd.retry_count = Command::MAX_RETRIES;
  queue.set_pending(cmd);
  queue.retry_pending();

  EXPECT_TRUE(queue.empty());
}

}  // namespace esphome::epson_projector
