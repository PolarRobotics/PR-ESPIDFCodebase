#pragma once

#include <Robot.h>
#include <PWMMotor.h>
#include <ps5Controller.h>

class CenterConversion : public Robot
{
private:
    uint8_t actuatorPin;
    PWMMotor actuatorMotor;

public:
    CenterConversion(uint8_t actuatorPin);
    void action() override;
    void actuatorControl(bool raise);
    void stop();
};