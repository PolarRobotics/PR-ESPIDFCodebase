#pragma once

#include <MotorTypes.h>

/**
 * @brief Shared logic for motor control implementations (PWM + Packet Serial).
 *
 * Note: This class intentionally does NOT know how to output power to hardware.
 * Use `PWMMotor` or `SerialMotor` for the actual write/attach behavior.
 */
class MotorControlCommon
{
 protected:
  // Configuration
  MotorType motor_type = big_ampflow;
  float gear_ratio = 1.0F;
  bool has_encoder = false;
  int enc_a_pin = -1;
  int enc_b_pin = -1;

  // Derived data
  int max_rpm = 0;  // motor max rpm * gear ratio

  // Ramp state
  float requestedRPM = 0.0F;
  uint32_t lastRampTime = 0;
  float timeElapsed = 0.0F;

  // Encoder state
  int encoderACount = 0;
  int b_channel_state = 0;
  int rollover = 0;
  int prev_current_count = 0;
  int rollover_threshold = 0;
  uint32_t current_time = 0;
  uint32_t prev_current_time = 0;
  float omega = 0.0F;
  void configureCommon(
      MotorType type, bool hasEnc, float gearRatio, int encA, int encB);

 public:
  MotorControlCommon() = default;
  int getMaxRPM() const;
  int Percent2RPM(float pct) const;
  float RPM2Percent(int rpm) const;
  float ramp(float requestedPower, float accelRate);
  float ramp(float requestedPower, float accelRate, float decelRate);
  void readEncoder();
  int calcSpeed(int current_count);
};
