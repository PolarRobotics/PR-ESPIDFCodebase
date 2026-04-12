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
        stop(); // Stop the motor when no button is pressed
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