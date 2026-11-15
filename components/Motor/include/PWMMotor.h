#pragma once

#include "MotorControlCommon.h"
#include "MotorInterface.h"

/**
 * @brief PWM motor control using `MotorInterface` (LEDC).
 *
 * Power input/output is normalized percent in range [-1.0, 1.0].
 */
class PWMMotor : public MotorControlCommon
{
 private:
  MotorInterface motor;
  int pin = -1;

 public:
  PWMMotor() = default;

  uint8_t setup(
      int motorPin,
      MotorType motorType = big_ampflow,
      bool hasEncoder = false,
      float gearRatio = 1.0F,
      int encAChanPin = -1,
      int encBChanPin = -1);

  void write(float pct);
};
