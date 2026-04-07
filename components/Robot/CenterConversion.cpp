#include <CenterConversion.h>

CenterConversion::CenterConversion(uint8_t actuatorPin)
{
    this->actuatorPin = actuatorPin;
    actuatorMotor.setup(actuatorPin, small_12v);
}

void CenterConversion::action()
{
    // Control the actuator of the center conversion
    if (ps5.Triangle())
        actuatorControl(true);
    else if (ps5.Cross())
        actuatorControl(false);
    else
        actuatorMotor.write(0);
}

void CenterConversion::actuatorControl(bool raise)
{
    if (raise)
        actuatorMotor.write(0.5);
    else
        actuatorMotor.write(-0.5);
}