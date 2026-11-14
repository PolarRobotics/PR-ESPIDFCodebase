#pragma once

#include <Arduino.h>
#include <USBSabertooth.h>

#include "MotorControlCommon.h"

/**
 * @brief Packet Serial motor control using `USBSabertooth`.
 *
 * Raw power is in range [-2047, 2047].
 */
class SerialMotor : public MotorControlCommon
{
 private:
  int mot_idx = 0;  // Sabertooth motor channel (typically 1 or 2)

 public:
  SerialMotor() = default;

  void setup(
      int motorIndex,
      MotorType motorType = big_ampflow,
      bool hasEncoder = false,
      float gearRatio = 1.0F,
      int encAChanPin = -1,
      int encBChanPin = -1)
  {
    mot_idx = motorIndex;
    configureCommon(motorType, hasEncoder, gearRatio, encAChanPin, encBChanPin);
  }

  void writeRaw(int pwr);

  void writePercent(float pct)
  {
    pct = constrain(pct, -1.0F, 1.0F);
    writeRaw(int(pct * 2047.0F));
  }

  int motorIndex() const { return mot_idx; }
};
