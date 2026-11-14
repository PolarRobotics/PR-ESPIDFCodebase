#include "MotorControlCommon.h"

void MotorControlCommon::configureCommon(
    MotorType type, bool hasEnc, float gearRatio, int encA, int encB)
{
  has_encoder = hasEnc;
  motor_type = type;
  gear_ratio = gearRatio;
  enc_a_pin = encA;
  enc_b_pin = encB;

  max_rpm =
      int(MOTOR_MAX_RPM_ARR[static_cast<uint8_t>(motor_type)] * gear_ratio);

  // ramp init
  requestedRPM = 0.0F;
  lastRampTime = millis();
}

int MotorControlCommon::getMaxRPM() const { return max_rpm; }

int MotorControlCommon::Percent2RPM(float pct) const
{
  return max_rpm * constrain(pct, -1.0F, 1.0F);
}

float MotorControlCommon::RPM2Percent(int rpm) const
{
  if (rpm == 0) return 0.0F;
  return constrain(rpm, -max_rpm, max_rpm) / float(max_rpm);
}

/**
 * @brief Ramp requestedPower towards target at accelRate (%/ms).
 */
float MotorControlCommon::ramp(float requestedPower, float accelRate)
{
  return ramp(requestedPower, accelRate, accelRate);
}

/**
 * @brief Ramp requestedPower towards target with separate acceleration and
 * deceleration rates.
 * @param requestedPower target power in range [-1.0, 1.0]
 * @param accelRate acceleration rate (%/ms) - used when increasing power
 * @param decelRate deceleration rate (%/ms) - used when decreasing power
 */
float MotorControlCommon::ramp(
    float requestedPower, float accelRate, float decelRate)
{
  uint32_t currentMillis = millis();
  timeElapsed = float(currentMillis - lastRampTime);
  lastRampTime = currentMillis;

  if (requestedPower > requestedRPM)
  {
    // Accelerating - use accelRate
    requestedRPM = requestedRPM + accelRate * timeElapsed;
    if (requestedRPM > requestedPower) requestedRPM = requestedPower;
  }
  else
  {
    // Decelerating - use decelRate
    requestedRPM = requestedRPM - decelRate * timeElapsed;
    if (requestedRPM < requestedPower) requestedRPM = requestedPower;
  }

  return requestedRPM;
}

// Encoder related functions (shared)
void MotorControlCommon::readEncoder()
{
  if (!has_encoder || enc_b_pin < 0) return;

  b_channel_state = digitalRead(enc_b_pin);

  if (b_channel_state == 1)
    if (encoderACount >= rollover)
      encoderACount = 0;
    else
      encoderACount = encoderACount + 1;
  else if (encoderACount == 0)
    encoderACount = rollover;
  else
    encoderACount = encoderACount - 1;
}

int MotorControlCommon::calcSpeed(int current_count)
{
  current_time = millis();

  if (abs(current_count - prev_current_count) >= rollover_threshold)
  {
    if ((current_count - rollover_threshold) > 0)
      omega = float((current_count - rollover) - prev_current_count) /
              float(current_time - prev_current_time);
    else
      omega = float((current_count + rollover) - prev_current_count) /
              float(current_time - prev_current_time);
    {
      current_time = millis();

      if (abs(current_count - prev_current_count) >= rollover_threshold)
      {
        if ((current_count - rollover_threshold) > 0)
          omega = float((current_count - rollover) - prev_current_count) /
                  float(current_time - prev_current_time);
        else
          omega = float((current_count + rollover) - prev_current_count) /
                  float(current_time - prev_current_time);
      }
      else
      {
        omega = float(current_count - prev_current_count) /
                float(current_time - prev_current_time);
      }

      prev_current_count = current_count;
      prev_current_time = current_time;

      return int(
          omega * 156.25F);  // 156.25 for 384, 312.5 for 192, 1250 for 48
    }
  }
  else
  {
    omega = float(current_count - prev_current_count) /
            float(current_time - prev_current_time);
  }

  prev_current_count = current_count;
  prev_current_time = current_time;

  return int(omega * 156.25F);  // 156.25 for 384, 312.5 for 192, 1250 for 48
}