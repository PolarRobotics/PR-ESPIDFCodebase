#pragma once

#include <BotTypes.h>
#include <DriveParameters.h>
#include <MotorTypes.h>
#include <PWMMotor.h>
#include <SerialMotor.h>
#include <stdint.h>

#ifndef NUM_MOTORS
#define NUM_MOTORS 2
#endif  // !NUM_MOTORS

// RAMP DEFINES
// rate of change of power with respect to time when accelerating %power/10th of
// sec

// [RPM/ms] possibly change to RPM/s for future
#define ACCELERATION_RATE 0.00375F

// default: 0.00375f, Runningback old: 0.03f, 0.015f
#define RB_ACCELERATION_RATE 0.0015F

// slower acceleration for strength linemen
#define STRENGTH_LINEMAN_ACCELERATION_RATE 0.002F

// slower deceleration for strength linemen
#define STRENGTH_LINEMAN_DECELERATION_RATE 0.0005F

// Strength linemen deceleration rate - now uses same rate as acceleration
// #define STRENGTH_LINEMAN_DECELERATION_RATE 0.0001F // [%/ms] - no longer used

// packet-serial acceleration rate (%/ms)
#define SERIAL_ACCELERATION_RATE 0.002F

// rate of deceleration/braking
#define BRAKE_PERCENTAGE 0.9
// how often the ramp() function changes the motor power
#define TIME_INCREMENT 5

// drive param generation
#define NORMAL_TURN_CONSTANT 0.05
// Value for the tank mode speed reduction percentage
#define TANK_MODE_PCT 0.75
// Value for the tank mode speed reduction percentage
#define RB_TANK_MODE_PCT 0.5
// Value for the tank mode speed reduction percentage for strength linemen
#define STRENGTH_TANK_MODE_PCT 1.0
// Value for the Drift Mode Reduction Factor Percentage
#define DRIFT_MODE_PCT 0.8
// these should = normal speed, QB needs 0.5 for both

// Controller Defines
#define STICK_DEADZONE 0.075F  // 0.0390625F // 8.0 / 127.0
#define STICK_AXIS_HALF_RANGE 127.5F
#define THRESHOLD 0.00001

// Motor Percent Defines
#define FALCON_CALIBRATION_FACTOR 1.0F
// the minimum power that can be written to the motor, prevents stalling
// originally 0.05 for PWM: multiply by 2047 gives 102.35, round up to 103
#define MOTOR_ZERO_OFFST 103

// BSN defines for the small 12v motors
// define SMALL_12V_BOOST_PCT          0.15F
// define SMALL_12V_NORMAL_PCT         0.1F // 0.5
// define SMALL_12V_SLOW_PCT           0.05F

#define BRAKE_BUTTON_PCT 0

// Turn model / telemetry constants
constexpr int MIN_DRIVE_RPM = 200;
constexpr float TURN_SENSITIVITY_SCALAR = 0.49F;
constexpr float TURN_SENSITIVITY_HALF = 0.5F;
constexpr float TURN_SENSITIVITY_CUBIC_EXPONENT = 3.0F;
constexpr int WIFI_MOTOR_PERCENT_SCALE = 100;

// !TODO: not sure if this is the correct location for this array
// This array must follow the same order as MotorType to be used effectively
// Max power value accepted via USBSabertooth Packetized Serial Protocol
constexpr int SABERTOOTH_MAX_POWER = 2047;

class Drive
{
 private:
  BotType botType;
  MotorType motorType;
  MotorInterfaceType motorInterfaceType;
  float gearRatio;
  bool hasEncoders;

  float speedScalar;
  float wheelBase;
  int omega;
  int omega_L, omega_R;
  float R, R_Max, R_Min;
  int max_RPM, min_RPM;
  int enableTurnSensitivity;
  // Turn sensitivity variables
  float scaledSensitiveTurn = 0.0F;
  float turnSensitivityScalar = 0.0F;
  float domainAdjustment = 0.0F;

  void calcTurning(float stickTrn, float fwdLinPwr);

 protected:
  // Drive can be either PWM or Packet Serial depending on `motorInterfaceType`.
  PWMMotor pwmM1, pwmM2;
  SerialMotor serialM1, serialM2;
  float stickForwardRev, stickTurn;
  float lastTurnPwr;
  float turnPower;

  float requestedMotorPower[NUM_MOTORS];
  int requestedMotorPowerSerial[NUM_MOTORS];
  // Always tracked as normalized percent [-1, 1] for telemetry.
  float trackingMotorPower[NUM_MOTORS];
  float lastRampPower[NUM_MOTORS];
  float turnMotorValues[NUM_MOTORS];

 public:
  enum Speed : uint8_t
  {
    BOOST,
    NORMAL,
    SLOW,
    BRAKE
  };

  Drive();
  Drive(BotType botType, MotorType motorType);
  Drive(
      BotType botType,
      drive_param_t driveParams,
      bool hasEncoders = false,
      int turnFunction = 2);
  void setupMotors(uint8_t lpin, uint8_t rpin);
  void setupMotors(
      uint8_t lpin,
      uint8_t rpin,
      uint8_t left_enc_a_pin,
      uint8_t left_enc_b_pin,
      uint8_t right_enc_a_pin,
      uint8_t right_enc_b_pin);
  void setStickPwr(int8_t leftY, int8_t rightX);
  float getForwardPower() const;
  float getTurnPower() const;
  void setSpeedScalar(Speed bns);
  void setSpeedValue(float speed_pct);
  float getSpeedScalar() const;
  void emergencyStop();
  void generateMotionValues(float tankModePct = TANK_MODE_PCT);
  virtual void update();
  void printSetup();
  virtual void printDebugInfo();
  virtual void printCsvInfo();
  int getMotorWifiValue(int motorRequested);
};
