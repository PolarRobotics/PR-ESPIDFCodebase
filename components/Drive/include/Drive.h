#pragma once

#include <Arduino.h>
#include <BotTypes.h>
#include <DriveParameters.h>
#include <PWMMotor.h>
#include <SerialMotor.h>
#include <MotorTypes.h>

#ifndef NUM_MOTORS
#define NUM_MOTORS 2
#endif // !NUM_MOTORS

// RAMP DEFINES
// rate of change of power with respect to time when accelerating %power/10th of sec
#define ACCELERATION_RATE 0.00375f   // [RPM/ms] possibly change to RPM/s for future
#define RB_ACCELERATION_RATE 0.0015f // default: 0.00375f, Runningback old: 0.03f, 0.015f
// Strength linemen deceleration rate - slows down gradually to prevent flipping when releasing joystick
// 0.0001 = ~1 second to decelerate from full power to zero, adjust higher for faster decel
#define STRENGTH_LINEMAN_DECELERATION_RATE 0.0001f // [%/ms]

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
// Value for the Drift Mode Reduction Factor Percentage
#define DRIFT_MODE_PCT 0.8
// these should = normal speed, QB needs 0.5 for both

// Controller Defines
#define STICK_DEADZONE 0.075 // 0.0390625F // 8.0 / 127.0
#define THRESHOLD 0.00001

// Motor Percent Defines
#define FALCON_CALIBRATION_FACTOR 1.0f
// the minimum power that can be written to the motor, prevents stalling
#define MOTOR_ZERO_OFFST 103 // originally 0.05 for PWM: multiply by 2047 gives 102.35, round up to 103

// BSN defines for the small 12v motors
// define SMALL_12V_BOOST_PCT          0.15f
// define SMALL_12V_NORMAL_PCT         0.1f // 0.5
// define SMALL_12V_SLOW_PCT           0.05f

#define BRAKE_BUTTON_PCT 0

// !TODO: not sure if this is the correct location for this array
// This array must follow the same order as MotorType to be used effectively
constexpr int SABERTOOTH_MAX_POWER = 2047; // Max power value accepted via USBSabertooth Packetized Serial Protocol

class Drive
{
private:
  BotType botType;
  MotorType motorType; // TODO: Why is this private if we have a setter with no input validation? - MP 2023-05-10
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
  float scaledSensitiveTurn = 0.0f;
  float turnSensitivityScalar = 0.0f;
  float domainAdjustment = 0.0f;

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
  enum Speed
  {
    BOOST,
    NORMAL,
    SLOW,
    BRAKE
  };

  Drive();
  Drive(BotType botType, MotorType motorType);
  Drive(BotType botType, drive_param_t driveParams, bool hasEncoders = false, int turnFunction = 2);
  void setupMotors(uint8_t lpin, uint8_t rpin);
  void setupMotors(uint8_t lpin, uint8_t rpin, uint8_t left_enc_a_pin, uint8_t left_enc_b_pin, uint8_t right_enc_a_pin, uint8_t right_enc_b_pin);
  void setStickPwr(int8_t leftY, int8_t rightX);
  float getForwardPower();
  float getTurnPower();
  void setSpeedScalar(Speed bns);
  void setSpeedValue(float speed_pct);
  float getSpeedScalar();
  void emergencyStop();
  void generateMotionValues(float tankModePct = TANK_MODE_PCT);
  virtual void update();
  void printSetup();
  virtual void printDebugInfo();
  virtual void printCsvInfo();
  int getMotorWifiValue(int motorRequested);

  //* The following variables are initialized in the constructor
  // maximum speed for these is 1.0
  // percentage of power used when boosting for big motors
  float BIG_BOOST_PCT;

  // default: 0.6, this is the typical percentage of power out of the motors' range that is used (to ensure they don't do seven wheelies)
  float BIG_NORMAL_PCT;

  // should be a value less than BIG_NORMAL_PCT, to slow down for precision maneuvering, QB needs this to be 0.3
  float BIG_SLOW_PCT;
};