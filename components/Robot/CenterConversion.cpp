#include <CenterConversion.h>

CenterConversion::CenterConversion(uint8_t actuatorPin)
{
    this->actuatorPin = actuatorPin;
    actuatorMotor.setup(actuatorPin, small_12v);
}

void CenterConversion::action()
{
    // Button handling moved to centerConversionCallback() via ps5.attach()
}

void CenterConversion::actuatorControl(bool raise)
{
    if (raise)
        actuatorMotor.write(0.5);
    else
        actuatorMotor.write(-0.5);
}

/**
 * @brief Stop Motor
 *
 * Stops the motor by writing the actuatorMotor SPECBOT_1 (D18) pin to 0
 */
void CenterConversion::stop()
{
    actuatorMotor.write(0);
}