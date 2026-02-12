#pragma once

#include <Arduino.h>
#include <MotorInterface.h>

#include <MotorControlCommon.h>

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

    /**
     * @return channel number from `MotorInterface::attach()` (or 255 on failure)
     */
    uint8_t setup(int motorPin,
                  MotorType type = big_ampflow,
                  bool hasEncoder = false,
                  float gearRatio = 1.0f,
                  int encAChanPin = -1,
                  int encBChanPin = -1)
    {
        pin = motorPin;
        configureCommon(type, hasEncoder, gearRatio, encAChanPin, encBChanPin);
        return motor.attach(pin, MIN_PWM_US, MAX_PWM_US);
    }

    void write(float pct)
    {
        motor.write(pct);
    }
};
