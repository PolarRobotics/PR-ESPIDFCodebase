#pragma once

#include <Arduino.h>
#include <USBSabertooth.h>

#include <MotorControlCommon.h>

/**
 * @brief Packet Serial motor control using `USBSabertooth`.
 *
 * Raw power is in range [-2047, 2047].
 */
class SerialMotor : public MotorControlCommon
{
private:
    int mot_idx = 0; // Sabertooth motor channel (typically 1 or 2)

public:
    SerialMotor() = default;

    void setup(int motorIndex,
               MotorType type = big_ampflow,
               bool hasEncoder = false,
               float gearRatio = 1.0f,
               int encAChanPin = -1,
               int encBChanPin = -1)
    {
        mot_idx = motorIndex;
        configureCommon(type, hasEncoder, gearRatio, encAChanPin, encBChanPin);

        // Setup encoder ISR if encoders are enabled
        if (hasEncoder && encAChanPin >= 0)
        {
            pinMode(encAChanPin, INPUT);
            attachInterruptArg(encAChanPin, MotorControlCommon::encoderISR, this, RISING);
        }
    }

    void writeRaw(int pwr);

    void writePercent(float pct)
    {
        pct = constrain(pct, -1.0f, 1.0f);
        writeRaw(int(pct * 2047.0f));
    }

    int motorIndex() const { return mot_idx; }

    // Implement virtual applyOutput
    virtual void applyOutput() override
    {
        if (motorMutex != nullptr && xSemaphoreTake(motorMutex, pdMS_TO_TICKS(5)) == pdTRUE)
        {
            writeRaw(int(outputPercent * 2047.0f));
            xSemaphoreGive(motorMutex);
        }
    }
};
