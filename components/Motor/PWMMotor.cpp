#include "PWMMotor.h"

/**
 * @return channel number from `MotorInterface::attach()` (or 255 on failure)
 */
uint8_t PWMMotor::setup(
    int motorPin,
    MotorType motorType,
    bool hasEncoder,
    float gearRatio,
    int encAChanPin,
    int encBChanPin)
{
  pin = motorPin;
  configureCommon(motorType, hasEncoder, gearRatio, encAChanPin, encBChanPin);
  return motor.attach(pin, MIN_PWM_US, MAX_PWM_US);
}

void PWMMotor::write(float pct) { motor.write(pct); }