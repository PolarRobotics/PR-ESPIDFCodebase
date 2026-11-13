#include "Drive.h"

#include <esp_log.h>
#include <math.h>

#include "MotorTypes.h"
#include "PWMMotor.h"
#include "SerialMotor.h"

static const char *TAG = "Drive";

/**
 * @brief Drive module, base drive configuration intended for the standard
 * linemen. this module takes the stick input, scales the turning value for each
 * motor and ramps that value over time, then sets the ramped value to the
 * motors
 * @authors Rhys Davies (@rdavies02), Max Phillips (@RyzenFromFire)
 *
 *    2 motor configuration shown below
 *
 *               ^
 *               | Fwd
 *       _________________
 *      |        _        |
 *      |       |O|       |
 *      |       |_|       |
 *      |  _           _  |
 *      | |L|         |R| |
 *      | |_|         |_| |
 *      |_________________|
 *
 *   O: represents the Omniwheel, a wheel that can turn on 2 axis
 *   L: represents the left Wheel, powered by the left motor via a chain
 *        - the left motor would turn ccw to move the bot forward
 *   R: represents the right Wheel, powered by the right motor via a chain
 *        - the right motor would turn cw to move the bot forward
 *
 *
 * @todo
 *  - add a turning radius parameter, needed for the kicker
 *  - add mechanium driving code, for the new center, needed next semester
 * (Spring 2023)
 */

// Module state (former Drive class members)
static bool driveInitialized = false;
static BotType botType;
static MotorType motorType;
static MotorInterfaceType motorInterfaceType;
static float gearRatio;
static bool hasEncoders;

static float speedScalar;
static float wheelBase;
static int omega;
static int omega_L, omega_R;
static float R, R_Max, R_Min;
static int enableTurnSensitivity;
// Turn sensitivity variables
static float scaledSensitiveTurn = 0.0F;
static float turnSensitivityScalar = 0.0F;
static float domainAdjustment = 0.0F;

// Drive can be either PWM or Packet Serial depending on `motorInterfaceType`.
static PWMMotor pwmM1, pwmM2;
static SerialMotor serialM1, serialM2;
static float stickForwardRev, stickTurn;

static float requestedMotorPower[NUM_MOTORS];
static int requestedMotorPowerSerial[NUM_MOTORS];
// Always tracked as normalized percent [-1, 1] for telemetry.
static float trackingMotorPower[NUM_MOTORS];
static float lastRampPower[NUM_MOTORS];
static float turnMotorValues[NUM_MOTORS];

static void calcTurning(float stickTrn, float fwdLinPwr);

void driveInit(
    BotType botTypeIn,
    drive_param_t driveParams,
    bool hasEncodersIn,
    int turnFunction)
{
  botType = botTypeIn;
  motorType = driveParams.motor_type;
  motorInterfaceType = MOTORTYPE_INTERFACE_ARRAY[driveParams.motor_type];
  hasEncoders = hasEncodersIn;
  gearRatio = driveParams.gear_ratio;
  wheelBase = driveParams.wheel_base;
  R_Min = driveParams.r_min;
  R_Max = driveParams.r_max;

  // initialize arrays
  for (int i = 0; i < NUM_MOTORS; i++)
  {
    requestedMotorPower[i] = 0.0F;
    lastRampPower[i] = 0.0F;
    turnMotorValues[i] = 0.0F;
  }

  // initialize parameters for turning model
  omega = 0;
  omega_L = 0;
  omega_R = 0;
  R = 0.0F;

  // initialize turn sensitivity variables
  // 0 for linear, 1 for Rhys's function, 2 for cubic
  enableTurnSensitivity = turnFunction;

  // Range: (0, 0.5) really [0.01, 0.49]
  turnSensitivityScalar = TURN_SENSITIVITY_SCALAR;
  domainAdjustment =
      1.0F / logf(
                 (1.0F - (turnSensitivityScalar + TURN_SENSITIVITY_HALF)) /
                 (turnSensitivityScalar + TURN_SENSITIVITY_HALF));

  driveInitialized = true;
}

bool driveIsInitialized() { return driveInitialized; }

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters) -- intentional L/R pin
// pair
void setupMotors(uint8_t lpin, uint8_t rpin)
{
  if (motorInterfaceType == pwm)
  {
    pwmM1.setup(lpin, motorType, hasEncoders, gearRatio);
    pwmM2.setup(rpin, motorType, hasEncoders, gearRatio);
  }
  else
  {
    serialM1.setup(lpin, motorType, hasEncoders, gearRatio);
    serialM2.setup(rpin, motorType, hasEncoders, gearRatio);
  }
}

/**
 * setupMotors
 * @brief to be called when setting up a motor with an encoder
 */
void setupMotors(  // NOLINT(bugprone-easily-swappable-parameters) --
                   // intentional pin/encoder args
    uint8_t lpin,
    uint8_t rpin,  // NOLINT(bugprone-easily-swappable-parameters)
    uint8_t left_enc_a_pin,
    uint8_t left_enc_b_pin,  // NOLINT(bugprone-easily-swappable-parameters)
    uint8_t right_enc_a_pin,
    uint8_t right_enc_b_pin)
{
  if (motorInterfaceType == pwm)
  {
    pwmM1.setup(
        lpin,
        motorType,
        hasEncoders,
        gearRatio,
        left_enc_a_pin,
        left_enc_b_pin);
    pwmM2.setup(
        rpin,
        motorType,
        hasEncoders,
        gearRatio,
        right_enc_a_pin,
        right_enc_b_pin);
  }
  else
  {
    serialM1.setup(
        lpin,
        motorType,
        hasEncoders,
        gearRatio,
        left_enc_a_pin,
        left_enc_b_pin);
    serialM2.setup(
        rpin,
        motorType,
        hasEncoders,
        gearRatio,
        right_enc_a_pin,
        right_enc_b_pin);
  }
}

/**
 * setStickPwr takes the stick values passed in and normalizes them to values
 * between -2047 and 2047 and sets this value to the private variables
 * stickFwdRev and stickTurn respectively
 * @author Rhys Davies (Edited by Quentin Osterhage for USBSabertooth)
 * Created: 9-12-2022 (Edited 10-04-2025)
 *
 * @param leftY the forward backward value from the left stick an unsigned 8-bit
 * float (0 to 255)
 * @param rightX the left right value from the right stick an unsigned 8-bit
 * float (0 to 255)
 */
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters) -- intentional stick
void setStickPwr(int8_t leftY, int8_t rightX)
{
  // left stick all the way forward is 0, backward is 255
  // +: forward, -: backward. needs to be negated so that forward is forward and
  // v.v.; subtracting 1 bumps into correct range
  stickForwardRev = static_cast<float>(leftY) / STICK_AXIS_HALF_RANGE;
  stickTurn = static_cast<float>(rightX) / STICK_AXIS_HALF_RANGE;
  // stick deadzones
  // set to zero (no input) if within the set deadzone
  // subtacting STICK_DEADZONE and deviding by 1-STICK_DEADZONE normalize the
  // inputs to use the full 0-1 range
  if (fabsf(stickForwardRev) < STICK_DEADZONE)
    stickForwardRev = 0.0F;
  else if (stickForwardRev > 0.0F)
    stickForwardRev =
        (stickForwardRev - STICK_DEADZONE) / (1.0F - STICK_DEADZONE);
  else if (stickForwardRev < 0.0F)
    stickForwardRev =
        (stickForwardRev + STICK_DEADZONE) / (1.0F - STICK_DEADZONE);

  if (fabsf(stickTurn) < STICK_DEADZONE)
    stickTurn = 0.0F;
  else if (stickTurn > 0.0F)
    stickTurn = (stickTurn - STICK_DEADZONE) / (1.0F - STICK_DEADZONE);
  else if (stickTurn < 0.0F)
    stickTurn = (stickTurn + STICK_DEADZONE) / (1.0F - STICK_DEADZONE);
}

float getForwardPower() { return stickForwardRev; }

float getTurnPower() { return stickTurn; }

/**
 * @brief setSpeedScalar sets the internal variable to the requested percent
 * power, this is what the motor power gets multiplied by, this is where the
 * boost, normal and slow scalars get passed in
 * @author Rhys Davies
 * Created: 9-12-2022
 *
 * @param bns input speed choice BOOST, NORMAL, SLOW, BRAKE
 */
void setSpeedScalar(DriveSpeed bns)
{
  // set the scalar to zero if the requested value is greater than 1, this is
  // not entirely necessary, but is a safety
  if (bns == BRAKE)
    speedScalar = BRAKE_BUTTON_PCT;
  else
    // Grab the predefined bns values from the
    speedScalar = MOTORTYPE_BNS_ARRAY[static_cast<uint8_t>(motorType)]
                                     [static_cast<uint8_t>(bns)];
}

/**
 * @brief setSpeedValue overrides the default predefined values from
 * MOTORTYPE_BNS_ARRAY
 */
void setSpeedValue(float speed_pct)
{
  speedScalar = constrain(speed_pct, 0.0F, 1.0F);
}

float getSpeedScalar() { return speedScalar; }

/**
 * generateTurnScalar takes the input stick power and scales the max turning
 * power allowed with the forward power input
 * @authors Grant Brautigam, Rhys Davies, Max Phillips
 * Created: 9-12-2022
 */
void generateMotionValues(float tankModePct)
{
  if (fabsf(stickForwardRev) < STICK_DEADZONE)
  {  // fwd stick is zero
    if (fabsf(stickTurn) < STICK_DEADZONE)
    {
      // turn stick is zero
      // not moving, set motors to zero
      requestedMotorPower[0] = 0, requestedMotorPower[1] = 0;
    }
    else if (stickTurn > STICK_DEADZONE)
    {  // turning right, but not moving forward much so use tank mode
      requestedMotorPower[0] = speedScalar * abs(stickTurn) * tankModePct;
      requestedMotorPower[1] = -speedScalar * abs(stickTurn) * tankModePct;
    }
    else if (stickTurn < -STICK_DEADZONE)
    {
      // turning left, but not moving forward muchso use tank mode
      requestedMotorPower[0] = -speedScalar * abs(stickTurn) * tankModePct;
      requestedMotorPower[1] = speedScalar * abs(stickTurn) * tankModePct;
    }
    // no general else since encountered infinite loop
  }
  else
  {  // fwd stick is not zero
    if (fabsf(stickTurn) < STICK_DEADZONE)
    {  // turn stick is zero
      // just move forward directly
      requestedMotorPower[0] = speedScalar * stickForwardRev;
      requestedMotorPower[1] = speedScalar * stickForwardRev;
    }
    else
    {
      // moving forward and turning
      /*
      if the sticks are not in any of the edge cases tested for above (when both
      sticks are not 0), a value must be calculated to determine how to scale
      the motor that is doing the turning. i.e.: if the user moves the left
      stick all the way forward (stickFwdRev = 1), and they are attempting to
      turn right. The left motor should get set to 1 and the right motor should
      get set to some value less than 1, this value is determined by the
      function calcTurningMotorValue
      */
      if (stickTurn > STICK_DEADZONE)
      {  // turn Right
        calcTurning(abs(stickTurn), abs(speedScalar * stickForwardRev));

        requestedMotorPower[0] = copysign(turnMotorValues[0], stickForwardRev);
        requestedMotorPower[1] = copysign(turnMotorValues[1], stickForwardRev);
      }
      else if (stickTurn < -STICK_DEADZONE)
      {  // turn Left
        calcTurning(abs(stickTurn), abs(speedScalar * stickForwardRev));

        requestedMotorPower[0] = copysign(turnMotorValues[1], stickForwardRev);
        requestedMotorPower[1] = copysign(turnMotorValues[0], stickForwardRev);
      }
    }
  }
}

/**
 * @brief calcTurning generates power values for each motor to achieve a given
 * turn
 * @authors Grant Brautigam, Rhys Davies
 * Created: 9-12-2022
 * updated: 10-6-2023
 * Mathematical model:
 * Whitepaper: https://www.cs.columbia.edu/~allen/F17/NOTES/icckinematics.pdf
 *
 * Add link to Desmos in future
 *
 * Differential drive turning model
 *
 * omega_R = (omega/R)*(R + l/2)
 * omega_L = (omega/R)*(R - l/2)
 *
 * omega_R: right wheel angular velocity
 * omega_R: left wheel angular velocity
 * omega: the rate of rotation around the ICC (Instantaneous Center of
 * Curvature) l: distance between each wheel (wheelbase) R: distance the ICC
 * from the center of the wheelbase (l/2)
 *
 * @param stickTrn the absoulte value of the current turning stick input
 * @param fwdLinPwr the non-turning motor value from the previous loop, which
 * was actually sent to the motor
 */
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters) -- intentional turn
static void calcTurning(float stickTrn, float fwdLinPwr)
{
  MotorControlCommon *leftMotor =
      (motorInterfaceType == pwm)
          ? static_cast<MotorControlCommon *>(&pwmM1)
          : static_cast<MotorControlCommon *>(&serialM1);
  MotorControlCommon *rightMotor =
      (motorInterfaceType == pwm)
          ? static_cast<MotorControlCommon *>(&pwmM2)
          : static_cast<MotorControlCommon *>(&serialM2);

  // R_Min = R_Min + abs(stickForwardRev)*(R_High_Min - R_Min);
  // // start of turn scaling
  if (enableTurnSensitivity == 0)  // linear
    scaledSensitiveTurn = stickTrn;
  else if (enableTurnSensitivity == 1)  // rhys's function
    scaledSensitiveTurn =
        logf(
            (1.0F -
             (turnSensitivityScalar * stickTrn + TURN_SENSITIVITY_HALF)) /
            (turnSensitivityScalar * stickTrn + TURN_SENSITIVITY_HALF)) *
        domainAdjustment;
  else  // cubic
    scaledSensitiveTurn = powf(stickTrn, TURN_SENSITIVITY_CUBIC_EXPONENT);

  // Calculate the R value from the stick turn input
  R = (1.0F - scaledSensitiveTurn) * (R_Max - R_Min) + R_Min;

  // calculate the requested angular velocity for the robot
  omega = abs(leftMotor->Percent2RPM(fwdLinPwr));

  // calculate the rpm for the left/right wheels in float, then store as int
  const float halfWheelBase = wheelBase / 2.0F;
  const float omegaF = static_cast<float>(omega);
  omega_L = static_cast<int>((omegaF / R) * (R + halfWheelBase));
  omega_R = static_cast<int>((omegaF / R) * (R - halfWheelBase));

  // ensure the left wheel RPM doesnt go below the min or above the max RPM
  omega_L = constrain(omega_L, MIN_DRIVE_RPM, leftMotor->getMaxRPM());
  // ensure the left wheel RPM doesnt go below the min or above the max RPM
  omega_R = constrain(omega_R, MIN_DRIVE_RPM, leftMotor->getMaxRPM());

  turnMotorValues[0] = leftMotor->RPM2Percent(omega_L);
  turnMotorValues[1] = rightMotor->RPM2Percent(omega_R);
}

void emergencyStop()
{
  if (motorInterfaceType == pwm)
  {
    pwmM1.write(0.0F);
    pwmM2.write(0.0F);
  }
  else
  {
    serialM1.writeRaw(0);
    serialM2.writeRaw(0);
  }
}

void printSetup()
{
  const int maxRpm =
      (motorInterfaceType == pwm) ? pwmM1.getMaxRPM() : serialM1.getMaxRPM();

  ESP_LOGI(TAG, "printSetup():");
  ESP_LOGI(TAG, "MotorType: %s", getMotorTypeString(motorType));
  ESP_LOGI(TAG, "GearRatio: %f", (double)gearRatio);
  ESP_LOGI(TAG, "R_Min: %f", (double)R_Min);
  ESP_LOGI(TAG, "R_Max: %f", (double)R_Max);
  ESP_LOGI(TAG, "Min RPM: %d", MIN_DRIVE_RPM);
  ESP_LOGI(TAG, "MAX RPM: %d", maxRpm);
  ESP_LOGI(TAG, "TurnSensitivityMode: %d", enableTurnSensitivity);
  ESP_LOGI(TAG, "Encoders: ");
  ESP_LOGI(TAG, "Has Encoders? %s", hasEncoders ? "True" : "False");
}

/**
 * prints the internal variables to the serial monitor in a clean format,
 * this function exists out of pure laziness to not have to comment out all the
 * print statments
 * @author
 * Updated:
 */
void printDebugInfo()
{
  ESP_LOGI(
      TAG,
      "L_Hat_Y: %f  R_HAT_X: %f  |  Omega: %f  omega_L: %f  omega_R: %f  Left "
      "Motor: %d  Right: %d",
      (double)stickForwardRev,
      (double)stickTurn,
      (double)omega,
      (double)omega_L,
      (double)omega_R,
      requestedMotorPowerSerial[0],
      requestedMotorPowerSerial[1]);
}
/**
 * @brief updates the motors after calling all the functions to generate
 * turning and scaling motor values, the intention of this is so the
 * programmer doesnt have to call all the functions, this just handles it,
 * reducing clutter in the main file.
 * DO NOT CALL THIS FUNCTION UNTIL setStickPwr and setSpeedScalar have been
 * called before update
 * @author Rhys Davies
 * Created: 9-12-2022
 */
void driveUpdate()
{
  const bool isRunningback = (botType == runningback);
  const bool isStrengthLineman = (botType == strength_lineman);

  float tankPct = TANK_MODE_PCT;
  if (isStrengthLineman)
    tankPct = STRENGTH_TANK_MODE_PCT;
  else if (isRunningback || motorInterfaceType == serial)
    tankPct = RB_TANK_MODE_PCT;

  float accelRate = ACCELERATION_RATE;
  if (isRunningback)
    accelRate = RB_ACCELERATION_RATE;
  else if (isStrengthLineman)
    accelRate = STRENGTH_LINEMAN_ACCELERATION_RATE;

  const float serialAccelRate = SERIAL_ACCELERATION_RATE;
  const float decelRate =
      isStrengthLineman ? STRENGTH_LINEMAN_DECELERATION_RATE : accelRate;

  // Generate turning motion
  generateMotionValues(tankPct);

  // Ramp in normalized percent space [-1, 1]
  if (motorInterfaceType == pwm)
  {
    requestedMotorPower[0] =
        pwmM1.ramp(requestedMotorPower[0], accelRate, decelRate);
    requestedMotorPower[1] =
        pwmM2.ramp(requestedMotorPower[1], accelRate, decelRate);
  }
  else
  {
    requestedMotorPower[0] =
        serialM1.ramp(requestedMotorPower[0], serialAccelRate, decelRate);
    requestedMotorPower[1] =
        serialM2.ramp(requestedMotorPower[1], serialAccelRate, decelRate);
  }

  // Deadband (percent-space)
  const float deadbandPct = static_cast<float>(MOTOR_ZERO_OFFST) /
                            static_cast<float>(SABERTOOTH_MAX_POWER);
  requestedMotorPower[0] = (fabsf(requestedMotorPower[0]) < deadbandPct)
                               ? 0.0F
                               : requestedMotorPower[0];
  requestedMotorPower[1] = (fabsf(requestedMotorPower[1]) < deadbandPct)
                               ? 0.0F
                               : requestedMotorPower[1];

  // Track ramp output for debugging/turn model
  lastRampPower[0] = requestedMotorPower[0];
  lastRampPower[1] = requestedMotorPower[1];

  // this fixes C's funny backwards motor
  if (botType == runningback && motorType == falcon)
    requestedMotorPower[1] = -requestedMotorPower[1];

  // Write output
  if (motorInterfaceType == pwm)
  {
    pwmM1.write(requestedMotorPower[0]);
    pwmM2.write(requestedMotorPower[1]);

    requestedMotorPowerSerial[0] = static_cast<int>(
        requestedMotorPower[0] * static_cast<float>(SABERTOOTH_MAX_POWER));
    requestedMotorPowerSerial[1] = static_cast<int>(
        requestedMotorPower[1] * static_cast<float>(SABERTOOTH_MAX_POWER));
  }
  else
  {
    requestedMotorPowerSerial[0] = static_cast<int>(
        requestedMotorPower[0] * static_cast<float>(SABERTOOTH_MAX_POWER));
    requestedMotorPowerSerial[1] = static_cast<int>(
        requestedMotorPower[1] * static_cast<float>(SABERTOOTH_MAX_POWER));

    serialM1.writeRaw(requestedMotorPowerSerial[0]);
    serialM2.writeRaw(requestedMotorPowerSerial[1]);
  }

  trackingMotorPower[0] = requestedMotorPower[0];
  trackingMotorPower[1] = requestedMotorPower[1];
}

int getMotorWifiValue(int motorRequested)
{
  int valueToReturn = 0;
  if (motorRequested >= 0 && motorRequested < NUM_MOTORS)
  {
    valueToReturn = static_cast<int>(
        trackingMotorPower[motorRequested] *
        static_cast<float>(WIFI_MOTOR_PERCENT_SCALE));
  }
  return valueToReturn;
}
