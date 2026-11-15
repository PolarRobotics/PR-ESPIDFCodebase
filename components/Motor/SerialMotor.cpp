#include "SerialMotor.h"

#include <PolarRobotics.h>

void SerialMotor::setup(
    int motorIndex,
    MotorType motorType,
    bool hasEncoder,
    float gearRatio,
    int encAChanPin,
    int encBChanPin)
{
  mot_idx = motorIndex;
  configureCommon(motorType, hasEncoder, gearRatio, encAChanPin, encBChanPin);
}

void SerialMotor::writeRaw(int pwr)
{
  pwr = constrain(pwr, -2047, 2047);
  ST.motor(this->mot_idx, pwr);
}

void SerialMotor::writePercent(float pct)
{
  pct = constrain(pct, -1.0F, 1.0F);
  writeRaw(int(pct * 2047.0F));
}

int SerialMotor::motorIndex() const { return mot_idx; }