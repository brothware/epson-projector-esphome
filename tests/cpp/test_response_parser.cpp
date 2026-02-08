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
  ASSERT_FALSE(result.has_value());
  EXPECT_TRUE(result.error().contains("Command error"));
}

TEST_F(ResponseParserTest, HandlesInvalidFormat) {
  auto result = parser.parse("INVALID\r:");
  ASSERT_FALSE(result.has_value());
  EXPECT_TRUE(result.error().contains("Invalid response format"));
}

TEST_F(ResponseParserTest, ParsesSharpness) {
  auto result = parser.parse("SHARP=10\r:");
  ASSERT_TRUE(result.has_value());
  auto *sharp = std::get_if<SharpnessResponse>(&*result);
  ASSERT_NE(sharp, nullptr);
  EXPECT_EQ(sharp->value, 10);
}

TEST_F(ResponseParserTest, ParsesDensity) {
  auto result = parser.parse("DENSITY=5\r:");
  ASSERT_TRUE(result.has_value());
  auto *density = std::get_if<DensityResponse>(&*result);
  ASSERT_NE(density, nullptr);
  EXPECT_EQ(density->value, 5);
}

TEST_F(ResponseParserTest, ParsesDensityNegative) {
  auto result = parser.parse("DENSITY=-15\r:");
  ASSERT_TRUE(result.has_value());
  auto *density = std::get_if<DensityResponse>(&*result);
  ASSERT_NE(density, nullptr);
  EXPECT_EQ(density->value, -15);
}

TEST_F(ResponseParserTest, ParsesTint) {
  auto result = parser.parse("TINT=12\r:");
  ASSERT_TRUE(result.has_value());
  auto *tint = std::get_if<TintResponse>(&*result);
  ASSERT_NE(tint, nullptr);
  EXPECT_EQ(tint->value, 12);
}

TEST_F(ResponseParserTest, ParsesTintNegative) {
  auto result = parser.parse("TINT=-20\r:");
  ASSERT_TRUE(result.has_value());
  auto *tint = std::get_if<TintResponse>(&*result);
  ASSERT_NE(tint, nullptr);
  EXPECT_EQ(tint->value, -20);
}

TEST_F(ResponseParserTest, ParsesColorTemperature) {
  auto result = parser.parse("CTEMP=5\r:");
  ASSERT_TRUE(result.has_value());
  auto *ctemp = std::get_if<ColorTempResponse>(&*result);
  ASSERT_NE(ctemp, nullptr);
  EXPECT_EQ(ctemp->value, 5);
}

TEST_F(ResponseParserTest, ParsesVKeystone) {
  auto result = parser.parse("VKEYSTONE=15\r:");
  ASSERT_TRUE(result.has_value());
  auto *vk = std::get_if<VKeystoneResponse>(&*result);
  ASSERT_NE(vk, nullptr);
  EXPECT_EQ(vk->value, 15);
}

TEST_F(ResponseParserTest, ParsesVKeystoneNegative) {
  auto result = parser.parse("VKEYSTONE=-20\r:");
  ASSERT_TRUE(result.has_value());
  auto *vk = std::get_if<VKeystoneResponse>(&*result);
  ASSERT_NE(vk, nullptr);
  EXPECT_EQ(vk->value, -20);
}

TEST_F(ResponseParserTest, ParsesHKeystone) {
  auto result = parser.parse("HKEYSTONE=10\r:");
  ASSERT_TRUE(result.has_value());
  auto *hk = std::get_if<HKeystoneResponse>(&*result);
  ASSERT_NE(hk, nullptr);
  EXPECT_EQ(hk->value, 10);
}

TEST_F(ResponseParserTest, ParsesHReverseOn) {
  auto result = parser.parse("HREVERSE=ON\r:");
  ASSERT_TRUE(result.has_value());
  auto *hr = std::get_if<HReverseResponse>(&*result);
  ASSERT_NE(hr, nullptr);
  EXPECT_TRUE(hr->reversed);
}

TEST_F(ResponseParserTest, ParsesHReverseOff) {
  auto result = parser.parse("HREVERSE=OFF\r:");
  ASSERT_TRUE(result.has_value());
  auto *hr = std::get_if<HReverseResponse>(&*result);
  ASSERT_NE(hr, nullptr);
  EXPECT_FALSE(hr->reversed);
}

TEST_F(ResponseParserTest, ParsesVReverseOn) {
  auto result = parser.parse("VREVERSE=ON\r:");
  ASSERT_TRUE(result.has_value());
  auto *vr = std::get_if<VReverseResponse>(&*result);
  ASSERT_NE(vr, nullptr);
  EXPECT_TRUE(vr->reversed);
}

TEST_F(ResponseParserTest, ParsesVReverseOff) {
  auto result = parser.parse("VREVERSE=OFF\r:");
  ASSERT_TRUE(result.has_value());
  auto *vr = std::get_if<VReverseResponse>(&*result);
  ASSERT_NE(vr, nullptr);
  EXPECT_FALSE(vr->reversed);
}

TEST_F(ResponseParserTest, ParsesLuminanceHigh) {
  auto result = parser.parse("LUMINANCE=00\r:");
  ASSERT_TRUE(result.has_value());
  auto *lum = std::get_if<LuminanceResponse>(&*result);
  ASSERT_NE(lum, nullptr);
  EXPECT_EQ(lum->mode_code, "00");
}

TEST_F(ResponseParserTest, ParsesLuminanceLow) {
  auto result = parser.parse("LUMINANCE=01\r:");
  ASSERT_TRUE(result.has_value());
  auto *lum = std::get_if<LuminanceResponse>(&*result);
  ASSERT_NE(lum, nullptr);
  EXPECT_EQ(lum->mode_code, "01");
}

TEST_F(ResponseParserTest, ParsesGamma) {
  auto result = parser.parse("GAMMA=22\r:");
  ASSERT_TRUE(result.has_value());
  auto *gamma = std::get_if<GammaResponse>(&*result);
  ASSERT_NE(gamma, nullptr);
  EXPECT_EQ(gamma->mode_code, "22");
}

TEST_F(ResponseParserTest, ParsesGammaCustom) {
  auto result = parser.parse("GAMMA=F0\r:");
  ASSERT_TRUE(result.has_value());
  auto *gamma = std::get_if<GammaResponse>(&*result);
  ASSERT_NE(gamma, nullptr);
  EXPECT_EQ(gamma->mode_code, "F0");
}

TEST_F(ResponseParserTest, ParsesFreezeOn) {
  auto result = parser.parse("FREEZE=ON\r:");
  ASSERT_TRUE(result.has_value());
  auto *freeze = std::get_if<FreezeResponse>(&*result);
  ASSERT_NE(freeze, nullptr);
  EXPECT_TRUE(freeze->frozen);
}

TEST_F(ResponseParserTest, ParsesFreezeOff) {
  auto result = parser.parse("FREEZE=OFF\r:");
  ASSERT_TRUE(result.has_value());
  auto *freeze = std::get_if<FreezeResponse>(&*result);
  ASSERT_NE(freeze, nullptr);
  EXPECT_FALSE(freeze->frozen);
}

TEST_F(ResponseParserTest, ParsesSerialNumber) {
  auto result = parser.parse("SNO=ABC123456\r:");
  ASSERT_TRUE(result.has_value());
  auto *sno = std::get_if<SerialNumberResponse>(&*result);
  ASSERT_NE(sno, nullptr);
  EXPECT_EQ(sno->serial, "ABC123456");
}

TEST_F(ResponseParserTest, HandlesMalformedPowerValue) {
  auto result = parser.parse("PWR=abc\r:");
  ASSERT_FALSE(result.has_value());
  EXPECT_TRUE(result.error().contains("Invalid power state"));
}

TEST_F(ResponseParserTest, HandlesMalformedLampHours) {
  auto result = parser.parse("LAMP=xyz\r:");
  ASSERT_FALSE(result.has_value());
  EXPECT_TRUE(result.error().contains("Invalid lamp hours"));
}

TEST_F(ResponseParserTest, HandlesMalformedVolume) {
  auto result = parser.parse("VOL=not_a_number\r:");
  ASSERT_FALSE(result.has_value());
  EXPECT_TRUE(result.error().contains("Invalid volume"));
}

TEST_F(ResponseParserTest, HandlesMalformedBrightness) {
  auto result = parser.parse("BRIGHT=invalid\r:");
  ASSERT_FALSE(result.has_value());
  EXPECT_TRUE(result.error().contains("Invalid brightness"));
}

TEST_F(ResponseParserTest, HandlesMalformedSharpness) {
  auto result = parser.parse("SHARP=bad\r:");
  ASSERT_FALSE(result.has_value());
  EXPECT_TRUE(result.error().contains("Invalid sharpness"));
}

TEST_F(ResponseParserTest, HandlesMalformedVKeystone) {
  auto result = parser.parse("VKEYSTONE=wrong\r:");
  ASSERT_FALSE(result.has_value());
  EXPECT_TRUE(result.error().contains("Invalid vertical keystone"));
}

TEST_F(ResponseParserTest, HandlesEmptyValue) {
  auto result = parser.parse("VOL=\r:");
  ASSERT_FALSE(result.has_value());
  EXPECT_TRUE(result.error().contains("Invalid volume"));
}

TEST_F(ResponseParserTest, HandlesLargeLampHours) {
  auto result = parser.parse("LAMP=65535\r:");
  ASSERT_TRUE(result.has_value());
  auto *lamp = std::get_if<LampResponse>(&*result);
  ASSERT_NE(lamp, nullptr);
  EXPECT_EQ(lamp->hours, 65535u);
}

TEST_F(ResponseParserTest, ParsesBrightnessScalingZero) {
  auto result = parser.parse("BRIGHT=0\r:");
  ASSERT_TRUE(result.has_value());
  auto *bright = std::get_if<BrightnessResponse>(&*result);
  ASSERT_NE(bright, nullptr);
  EXPECT_EQ(bright->value, 0);
}

TEST_F(ResponseParserTest, ParsesBrightnessScalingMax) {
  auto result = parser.parse("BRIGHT=255\r:");
  ASSERT_TRUE(result.has_value());
  auto *bright = std::get_if<BrightnessResponse>(&*result);
  ASSERT_NE(bright, nullptr);
  EXPECT_EQ(bright->value, 100);
}

TEST_F(ResponseParserTest, ParsesBrightnessScalingMid) {
  auto result = parser.parse("BRIGHT=127\r:");
  ASSERT_TRUE(result.has_value());
  auto *bright = std::get_if<BrightnessResponse>(&*result);
  ASSERT_NE(bright, nullptr);
  EXPECT_EQ(bright->value, 49);
}

TEST_F(ResponseParserTest, ParsesContrastScalingMax) {
  auto result = parser.parse("CONTRAST=255\r:");
  ASSERT_TRUE(result.has_value());
  auto *contrast = std::get_if<ContrastResponse>(&*result);
  ASSERT_NE(contrast, nullptr);
  EXPECT_EQ(contrast->value, 100);
}

TEST_F(ResponseParserTest, ParsesMuteNumericOn) {
  auto result = parser.parse("MUTE=01\r:");
  ASSERT_TRUE(result.has_value());
  auto *mute = std::get_if<MuteResponse>(&*result);
  ASSERT_NE(mute, nullptr);
  EXPECT_TRUE(mute->muted);
}

TEST_F(ResponseParserTest, ParsesMuteNumericOff) {
  auto result = parser.parse("MUTE=00\r:");
  ASSERT_TRUE(result.has_value());
  auto *mute = std::get_if<MuteResponse>(&*result);
  ASSERT_NE(mute, nullptr);
  EXPECT_FALSE(mute->muted);
}

TEST_F(ResponseParserTest, ParsesHReverseNumericOn) {
  auto result = parser.parse("HREVERSE=01\r:");
  ASSERT_TRUE(result.has_value());
  auto *hr = std::get_if<HReverseResponse>(&*result);
  ASSERT_NE(hr, nullptr);
  EXPECT_TRUE(hr->reversed);
}

TEST_F(ResponseParserTest, ParsesFreezeNumericOn) {
  auto result = parser.parse("FREEZE=01\r:");
  ASSERT_TRUE(result.has_value());
  auto *freeze = std::get_if<FreezeResponse>(&*result);
  ASSERT_NE(freeze, nullptr);
  EXPECT_TRUE(freeze->frozen);
}

TEST_F(ResponseParserTest, ParsesUnknownPowerState) {
  auto result = parser.parse("PWR=99\r:");
  ASSERT_TRUE(result.has_value());
  auto *power = std::get_if<PowerResponse>(&*result);
  ASSERT_NE(power, nullptr);
  EXPECT_EQ(power->state, PowerState::UNKNOWN);
}

TEST_F(ResponseParserTest, ParsesUnknownKey) {
  auto result = parser.parse("UNKNOWN=somevalue\r:");
  ASSERT_TRUE(result.has_value());
  auto *str = std::get_if<StringResponse>(&*result);
  ASSERT_NE(str, nullptr);
  EXPECT_EQ(str->value, "somevalue");
}

TEST_F(ResponseParserTest, HandlesWhitespaceInResponse) {
  auto result = parser.parse("PWR=01\r\n:");
  ASSERT_TRUE(result.has_value());
  auto *power = std::get_if<PowerResponse>(&*result);
  ASSERT_NE(power, nullptr);
  EXPECT_EQ(power->state, PowerState::ON);
}

TEST_F(ResponseParserTest, ParsesEmptyResponse) {
  auto result = parser.parse("");
  ASSERT_FALSE(result.has_value());
  EXPECT_TRUE(result.error().contains("Empty response"));
}

}  // namespace esphome::epson_projector
