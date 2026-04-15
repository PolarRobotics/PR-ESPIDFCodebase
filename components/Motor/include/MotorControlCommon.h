#pragma once

#include <Arduino.h>
#include <MotorTypes.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

/**
 * @brief Shared logic for motor control implementations (PWM + Packet Serial).
 *
 * Note: This class intentionally does NOT know how to output power to hardware.
 * Use `PWMMotor` or `SerialMotor` for the actual write/attach behavior.
 */
class MotorControlCommon
{
protected:
    // Configuration
    MotorType motor_type = big_ampflow;
    float gear_ratio = 1.0f;
    bool has_encoder = false;
    int enc_a_pin = -1;
    int enc_b_pin = -1;

    // Derived data
    int max_rpm = 0; // motor max rpm * gear ratio

    // Ramp state
    float requestedRPM = 0.0f;
    uint32_t lastRampTime = 0;
    float timeElapsed = 0.0f;

    // Encoder state
    volatile int encoderACount = 0;
    volatile int b_channel_state = 0;
    int rollover = 0;

    int prev_current_count = 0;
    int rollover_threshold = 0;
    uint32_t current_time = 0;
    uint32_t prev_current_time = 0;
    float omega = 0.0f;

    // RTOS synchronization
    SemaphoreHandle_t motorMutex = nullptr;

    // Motor command state (thread-safe)
    volatile float requestedPercent = 0.0f;
    volatile float outputPercent = 0.0f;
    volatile bool emergencyStopFlag = false;

    void configureCommon(MotorType type, bool hasEnc, float gearRatio, int encA, int encB)
    {
        has_encoder = hasEnc;
        motor_type = type;
        gear_ratio = gearRatio;
        enc_a_pin = encA;
        enc_b_pin = encB;

        max_rpm = int(MOTOR_MAX_RPM_ARR[static_cast<uint8_t>(motor_type)] * gear_ratio);

        // ramp init
        requestedRPM = 0.0f;
        lastRampTime = millis();

        // Initialize mutex
        motorMutex = xSemaphoreCreateMutex();
    }

public:
    MotorControlCommon() = default;
    virtual ~MotorControlCommon()
    {
        if (motorMutex != nullptr)
        {
            vSemaphoreDelete(motorMutex);
        }
    }

    int getMaxRPM() const { return max_rpm; }

    int Percent2RPM(float pct) const
    {
        return max_rpm * constrain(pct, -1.0f, 1.0f);
    }

    float RPM2Percent(int rpm) const
    {
        if (rpm == 0)
            return 0.0f;
        return constrain(rpm, -max_rpm, max_rpm) / float(max_rpm);
    }

    /**
     * @brief Set the desired motor power (thread-safe).
     */
    void setDesiredPercent(float pct)
    {
        if (motorMutex != nullptr && xSemaphoreTake(motorMutex, pdMS_TO_TICKS(5)) == pdTRUE)
        {
            requestedPercent = constrain(pct, -1.0f, 1.0f);
            xSemaphoreGive(motorMutex);
        }
    }

    /**
     * @brief Get the current output power (thread-safe).
     */
    float getOutputPercent()
    {
        float val = 0.0f;
        if (motorMutex != nullptr && xSemaphoreTake(motorMutex, pdMS_TO_TICKS(5)) == pdTRUE)
        {
            val = outputPercent;
            xSemaphoreGive(motorMutex);
        }
        return val;
    }

    /**
     * @brief Emergency stop the motor (thread-safe).
     */
    void emergencyStop()
    {
        if (motorMutex != nullptr && xSemaphoreTake(motorMutex, pdMS_TO_TICKS(5)) == pdTRUE)
        {
            emergencyStopFlag = true;
            outputPercent = 0.0f;
            requestedPercent = 0.0f;
            xSemaphoreGive(motorMutex);
        }
        // Immediately apply stop to hardware
        applyOutput();
    }

    /**
     * @brief Process motor updates (ramp and apply output). Call from RTOS task.
     */
    void process(float accelRate)
    {
        if (motorMutex != nullptr && xSemaphoreTake(motorMutex, pdMS_TO_TICKS(5)) == pdTRUE)
        {
            if (emergencyStopFlag)
            {
                outputPercent = 0.0f;
            }
            else
            {
                outputPercent = ramp(requestedPercent, accelRate);
            }
            xSemaphoreGive(motorMutex);
        }
        applyOutput();
    }

    /**
     * @brief Ramp requestedPower towards target at accelRate (%/ms).
     */
    float ramp(float requestedPower, float accelRate)
    {
        timeElapsed = float(millis() - lastRampTime);
        lastRampTime = millis();

        if (requestedPower > requestedRPM)
        {
            requestedRPM = requestedRPM + accelRate * timeElapsed;
            if (requestedRPM > requestedPower)
                requestedRPM = requestedPower;
        }
        else
        {
            requestedRPM = requestedRPM - accelRate * timeElapsed;
            if (requestedRPM < requestedPower)
                requestedRPM = requestedPower;
        }

        return requestedRPM;
    }

    // Encoder related functions (shared)
    void readEncoder()
    {
        if (!has_encoder || enc_b_pin < 0)
            return;

        b_channel_state = digitalRead(enc_b_pin);

        if (b_channel_state == 1)
        {
            if (encoderACount >= rollover)
                encoderACount = 0;
            else
                encoderACount = encoderACount + 1;
        }
        else
        {
            if (encoderACount == 0)
                encoderACount = rollover;
            else
                encoderACount = encoderACount - 1;
        }
    }

    int calcSpeed(int current_count)
    {
        current_time = millis();

        if (abs(current_count - prev_current_count) >= rollover_threshold)
        {
            if ((current_count - rollover_threshold) > 0)
                omega = float((current_count - rollover) - prev_current_count) / float(current_time - prev_current_time);
            else
                omega = float((current_count + rollover) - prev_current_count) / float(current_time - prev_current_time);
        }
        else
        {
            omega = float(current_count - prev_current_count) / float(current_time - prev_current_time);
        }

        prev_current_count = current_count;
        prev_current_time = current_time;

        return int(omega * 156.25f); // 156.25 for 384, 312.5 for 192, 1250 for 48
    }

    // ISR for encoder A channel
    static void IRAM_ATTR encoderISR(void *arg)
    {
        MotorControlCommon *motor = static_cast<MotorControlCommon *>(arg);
        motor->readEncoder();
    }

    // Virtual method for hardware-specific output
    virtual void applyOutput() = 0;
};
