#include "command.h"

#include <gtest/gtest.h>

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

TEST(SanitizeTest, RemovesCarriageReturn) {
  std::string result = sanitize_value("test\rvalue");
  EXPECT_EQ(result, "testvalue");
}

TEST(SanitizeTest, RemovesNewline) {
  std::string result = sanitize_value("test\nvalue");
  EXPECT_EQ(result, "testvalue");
}

TEST(SanitizeTest, RemovesColon) {
  std::string result = sanitize_value("test:value");
  EXPECT_EQ(result, "testvalue");
}

TEST(SanitizeTest, RemovesControlCharacters) {
  std::string result = sanitize_value("test\x01\x02value");
  EXPECT_EQ(result, "testvalue");
}

TEST(SanitizeTest, PreservesNormalCharacters) {
  std::string result = sanitize_value("ON");
  EXPECT_EQ(result, "ON");
}

TEST(SanitizeTest, PreservesNumbers) {
  std::string result = sanitize_value("30");
  EXPECT_EQ(result, "30");
}

TEST(SanitizeTest, CommandInjectionPrevented) {
  std::string result = sanitize_value("30\rPWR OFF\r");
  EXPECT_EQ(result, "30PWR OFF");
}

TEST(ValidSourceTest, ValidHexCodes) {
  EXPECT_TRUE(is_valid_source_code("30"));
  EXPECT_TRUE(is_valid_source_code("A0"));
  EXPECT_TRUE(is_valid_source_code("10"));
  EXPECT_TRUE(is_valid_source_code("53"));
}

TEST(ValidSourceTest, InvalidEmpty) {
  EXPECT_FALSE(is_valid_source_code(""));
}

TEST(ValidSourceTest, InvalidTooLong) {
  EXPECT_FALSE(is_valid_source_code("12345"));
}

TEST(ValidSourceTest, InvalidNonHex) {
  EXPECT_FALSE(is_valid_source_code("GH"));
  EXPECT_FALSE(is_valid_source_code("XX"));
}

TEST(ValidSourceTest, InvalidWithSpecialChars) {
  EXPECT_FALSE(is_valid_source_code("3\r"));
  EXPECT_FALSE(is_valid_source_code("A:"));
}

TEST(ClampTest, ClampsToMin) {
  EXPECT_EQ(clamp_value(-50, 0, 255), 0);
  EXPECT_EQ(clamp_value(-100, -32, 32), -32);
}

TEST(ClampTest, ClampsToMax) {
  EXPECT_EQ(clamp_value(300, 0, 255), 255);
  EXPECT_EQ(clamp_value(100, -32, 32), 32);
}

TEST(ClampTest, PreservesValidValues) {
  EXPECT_EQ(clamp_value(128, 0, 255), 128);
  EXPECT_EQ(clamp_value(0, -32, 32), 0);
  EXPECT_EQ(clamp_value(-10, -32, 32), -10);
}

TEST(CommandTest, EmptyValueReturnsEmpty) {
  std::string cmd = build_set_command("SOURCE", "\r\n");
  EXPECT_EQ(cmd, "");
}

}  // namespace esphome::epson_projector
