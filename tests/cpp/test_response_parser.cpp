#include "response_parser.h"

#include <gtest/gtest.h>

namespace esphome::epson_projector {

class ResponseParserTest : public ::testing::Test {
 protected:
  ResponseParser parser;
};

TEST_F(ResponseParserTest, IsCompleteResponseWithPrompt) {
  EXPECT_TRUE(parser.is_complete_response("PWR=01\r:"));
  EXPECT_TRUE(parser.is_complete_response(":"));
}

TEST_F(ResponseParserTest, IsCompleteResponseWithoutPrompt) {
  EXPECT_FALSE(parser.is_complete_response("PWR=01\r"));
  EXPECT_FALSE(parser.is_complete_response("PWR=01"));
  EXPECT_FALSE(parser.is_complete_response(""));
}

TEST_F(ResponseParserTest, ParsesPowerStateStandby) {
  auto result = parser.parse("PWR=00\r:");
  ASSERT_TRUE(result.has_value());
  auto *power = std::get_if<PowerResponse>(&*result);
  ASSERT_NE(power, nullptr);
  EXPECT_EQ(power->state, PowerState::STANDBY);
}

TEST_F(ResponseParserTest, ParsesPowerStateOn) {
  auto result = parser.parse("PWR=01\r:");
  ASSERT_TRUE(result.has_value());
  auto *power = std::get_if<PowerResponse>(&*result);
  ASSERT_NE(power, nullptr);
  EXPECT_EQ(power->state, PowerState::ON);
}

TEST_F(ResponseParserTest, ParsesPowerStateWarmup) {
  auto result = parser.parse("PWR=02\r:");
  ASSERT_TRUE(result.has_value());
  auto *power = std::get_if<PowerResponse>(&*result);
  ASSERT_NE(power, nullptr);
  EXPECT_EQ(power->state, PowerState::WARMUP);
}

TEST_F(ResponseParserTest, ParsesPowerStateCooldown) {
  auto result = parser.parse("PWR=03\r:");
  ASSERT_TRUE(result.has_value());
  auto *power = std::get_if<PowerResponse>(&*result);
  ASSERT_NE(power, nullptr);
  EXPECT_EQ(power->state, PowerState::COOLDOWN);
}

TEST_F(ResponseParserTest, ParsesLampHours) {
  auto result = parser.parse("LAMP=1234\r:");
  ASSERT_TRUE(result.has_value());
  auto *lamp = std::get_if<LampResponse>(&*result);
  ASSERT_NE(lamp, nullptr);
  EXPECT_EQ(lamp->hours, 1234u);
}

TEST_F(ResponseParserTest, ParsesLampHoursZero) {
  auto result = parser.parse("LAMP=0\r:");
  ASSERT_TRUE(result.has_value());
  auto *lamp = std::get_if<LampResponse>(&*result);
  ASSERT_NE(lamp, nullptr);
  EXPECT_EQ(lamp->hours, 0u);
}

TEST_F(ResponseParserTest, ParsesErrorCode) {
  auto result = parser.parse("ERR=03\r:");
  ASSERT_TRUE(result.has_value());
  auto *error = std::get_if<ErrorResponse>(&*result);
  ASSERT_NE(error, nullptr);
  EXPECT_EQ(error->code, 3);
}

TEST_F(ResponseParserTest, ParsesSourceResponse) {
  auto result = parser.parse("SOURCE=30\r:");
  ASSERT_TRUE(result.has_value());
  auto *source = std::get_if<SourceResponse>(&*result);
  ASSERT_NE(source, nullptr);
  EXPECT_EQ(source->source_code, "30");
}

TEST_F(ResponseParserTest, ParsesMuteOn) {
  auto result = parser.parse("MUTE=ON\r:");
  ASSERT_TRUE(result.has_value());
  auto *mute = std::get_if<MuteResponse>(&*result);
  ASSERT_NE(mute, nullptr);
  EXPECT_TRUE(mute->muted);
}

TEST_F(ResponseParserTest, ParsesMuteOff) {
  auto result = parser.parse("MUTE=OFF\r:");
  ASSERT_TRUE(result.has_value());
  auto *mute = std::get_if<MuteResponse>(&*result);
  ASSERT_NE(mute, nullptr);
  EXPECT_FALSE(mute->muted);
}

TEST_F(ResponseParserTest, ParsesVolume) {
  auto result = parser.parse("VOL=15\r:");
  ASSERT_TRUE(result.has_value());
  auto *vol = std::get_if<VolumeResponse>(&*result);
  ASSERT_NE(vol, nullptr);
  EXPECT_EQ(vol->value, 15);
}

TEST_F(ResponseParserTest, ParsesAckResponse) {
  auto result = parser.parse(":");
  ASSERT_TRUE(result.has_value());
  auto *ack = std::get_if<AckResponse>(&*result);
  EXPECT_NE(ack, nullptr);
}

TEST_F(ResponseParserTest, ParsesErrorResponseErr) {
  auto result = parser.parse("ERR\r:");
  ASSERT_TRUE(result.has_value());
  auto *error = std::get_if<ErrorResult>(&*result);
  ASSERT_NE(error, nullptr);
  EXPECT_EQ(error->message, "Command error");
}

TEST_F(ResponseParserTest, HandlesInvalidFormat) {
  auto result = parser.parse("INVALID\r:");
  ASSERT_TRUE(result.has_value());
  auto *error = std::get_if<ErrorResult>(&*result);
  EXPECT_NE(error, nullptr);
}

}  // namespace esphome::epson_projector
