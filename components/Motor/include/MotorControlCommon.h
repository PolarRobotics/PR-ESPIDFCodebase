#pragma once

#include <Arduino.h>
#include <MotorTypes.h>

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
    int encoderACount = 0;
    int b_channel_state = 0;
    int rollover = 0;

    int prev_current_count = 0;
    int rollover_threshold = 0;
    uint32_t current_time = 0;
    uint32_t prev_current_time = 0;
    float omega = 0.0f;

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
    }

public:
    MotorControlCommon() = default;

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
     * @brief Ramp requestedPower towards target at accelRate (%/ms).
     */
    float ramp(float requestedPower, float accelRate)
    {
        return ramp(requestedPower, accelRate, accelRate);
    }

    /**
     * @brief Ramp requestedPower towards target with separate acceleration and deceleration rates.
     * @param requestedPower target power in range [-1.0, 1.0]
     * @param accelRate acceleration rate (%/ms) - used when increasing power
     * @param decelRate deceleration rate (%/ms) - used when decreasing power
     */
    float ramp(float requestedPower, float accelRate, float decelRate)
    {
        return ramp(requestedPower, accelRate, accelRate);
    }

    /**
     * @brief Ramp requestedPower towards target with separate acceleration and deceleration rates.
     * @param requestedPower target power in range [-1.0, 1.0]
     * @param accelRate acceleration rate (%/ms) - used when increasing power
     * @param decelRate deceleration rate (%/ms) - used when decreasing power
     */
    float ramp(float requestedPower, float accelRate, float decelRate)
    {
        uint32_t currentMillis = millis();
        timeElapsed = float(currentMillis - lastRampTime);
        lastRampTime = currentMillis;

        if (requestedPower > requestedRPM)
        {
            // Accelerating - use accelRate
            requestedRPM = requestedRPM + accelRate * timeElapsed;
            if (requestedRPM > requestedPower)
                requestedRPM = requestedPower;
        }
        else
        {
            // Decelerating - use decelRate
            requestedRPM = requestedRPM - decelRate * timeElapsed;
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
};
