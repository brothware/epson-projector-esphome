#include <gtest/gtest.h>

#include "command.h"

namespace esphome::epson_projector {

TEST(CommandTest, BuildQueryCommand) {
  std::string cmd = build_query_command("PWR");
  EXPECT_EQ(cmd, "PWR?\r");
}

TEST(CommandTest, BuildQueryCommandLamp) {
  std::string cmd = build_query_command("LAMP");
  EXPECT_EQ(cmd, "LAMP?\r");
}

TEST(CommandTest, BuildSetCommandWithString) {
  std::string cmd = build_set_command("PWR", "ON");
  EXPECT_EQ(cmd, "PWR ON\r");
}

TEST(CommandTest, BuildSetCommandWithInt) {
  std::string cmd = build_set_command("VOL", 15);
  EXPECT_EQ(cmd, "VOL 15\r");
}

TEST(CommandTest, BuildPowerOnCommand) {
  std::string cmd = build_power_on_command();
  EXPECT_EQ(cmd, "PWR ON\r");
}

TEST(CommandTest, BuildPowerOffCommand) {
  std::string cmd = build_power_off_command();
  EXPECT_EQ(cmd, "PWR OFF\r");
}

TEST(CommandTest, BuildMuteOnCommand) {
  std::string cmd = build_mute_command(true);
  EXPECT_EQ(cmd, "MUTE ON\r");
}

TEST(CommandTest, BuildMuteOffCommand) {
  std::string cmd = build_mute_command(false);
  EXPECT_EQ(cmd, "MUTE OFF\r");
}

TEST(CommandTest, BuildSourceCommand) {
  std::string cmd = build_set_command("SOURCE", "30");
  EXPECT_EQ(cmd, "SOURCE 30\r");
}

TEST(CommandTest, BuildBrightnessCommand) {
  std::string cmd = build_set_command("BRIGHT", 128);
  EXPECT_EQ(cmd, "BRIGHT 128\r");
}

}  // namespace esphome::epson_projector
