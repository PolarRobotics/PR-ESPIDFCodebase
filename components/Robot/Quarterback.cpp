#include <Quarterback.h>

// This for some reason has to be declared in the .cpp file and not the .h file so that it does not conflict with the same declaration in other .h files
HardwareSerial Uart_Turret(2); // UART2

// "define" static members to satisfy linker
uint8_t Quarterback::turretEncoderPinA;
uint8_t Quarterback::turretEncoderPinB;
uint8_t Quarterback::turretEncoderStateB;
int32_t Quarterback::currentTurretEncoderCount;

void Quarterback::turretEncoderISR()
{
  turretEncoderStateB = digitalRead(turretEncoderPinB);

  if (turretEncoderStateB == 1)
  {
    currentTurretEncoderCount--;
  }
  else if (turretEncoderStateB == 0)
  {
    currentTurretEncoderCount++;
  }
}

#pragma region Constructor
Quarterback::Quarterback(
    uint8_t flywheelLeftPin,    // M1
    uint8_t flywheelRightPin,   // M2
    uint8_t cradlePin,          // M3
    uint8_t turretPin,          // M4
    uint8_t assemblyPin,        // S1
    uint8_t magnetometerSdaPin, // S3
    uint8_t magnetometerSclPin, // S4
    uint8_t turretEncoderPinA,  // E1A
    uint8_t turretEncoderPinB,  // E1B
    uint8_t turretLaserPin      // E2A
)
{
  // The conveyor and flywheels are assumed to be off initially
  flywheelsOn = false;
  conveyorOn = false;

  // Initially we are not aiming up or down, and we assume the initial elevation is zero
  aimingUp = false;
  aimingDown = false;
  currentElevation = 0;

  // Set internal class fields from arguments
  this->flywheelPin = flywheelPin;
  this->conveyorPin = conveyorPin;
  this->elevationPin = elevationPin;

  // Attach the motors to their respective pins
  flywheelMotor.setup(flywheelPin);
  conveyorMotor.setup(conveyorPin);
  elevationMotors.setup(elevationPin);

  // Set the conveyor motor to zero so it doesnt spin on startup
  conveyorMotor.write(CONVEYOR_OFF);

  // Lower the Linear Actuators at start up so they are in the bottom position
  // through the updateAim() function
  setupMotors = true;
  lastElevationTime = millis();
}
#pragma endregion

#pragma region action()
void Quarterback::action()
{
  // Update the bools within the class to see if the user wants to go up or down
  if (ps5.Up())
    aim(QBAim::AIM_UP);
  else if (ps5.Down())
    aim(QBAim::AIM_DOWN);

  // Toogle the Conveyor and Flywheels
  if (ps5.Square())
    toggleConveyor();
  else if (ps5.Circle())
    toggleFlywheels();

  // Change the flywheel speed
  if (ps5.Triangle())
    changeFWSpeed(SpeedStatus::INCREASE);
  else if (ps5.Cross())
    changeFWSpeed(SpeedStatus::DECREASE);

  // Update the aim and flywheels on quarterback to see if we need to stop or not
  update();
}

void Quarterback::toggleFlywheels()
{
  if (millis() - lastDBFW >= DEBOUNCE_WAIT)
  {
    // Toggle the bool so we know if its on or not
    flywheelsOn = !flywheelsOn;

    lastDBFW = millis();
  }
}

float Quarterback::rampFW(float requestedPower)
{

  if (millis() - lastFlywheelRampTime >= FW_TIME_INCREMENT)
  {
    if (abs(requestedPower) < THRESHOLD)
    { // if the input is effectively zero
      // Experimental Braking Code
      if (abs(currentFlywheelPower) < 0.1)
      { // if the current power is very small just set it to zero
        currentFlywheelPower = 0;
      }
      else
      {
        currentFlywheelPower *= FW_BRAKE_PERCENTAGE;
      }
      lastFlywheelRampTime = millis();
    }
    else if (abs(requestedPower - currentFlywheelPower) < FW_ACCEL_RATE)
    { // if the input is effectively at the current power
      return requestedPower;
    }
    // if we need to increase speed and we are going forward
    else if (requestedPower > currentFlywheelPower)
    {
      currentFlywheelPower = currentFlywheelPower + FW_ACCEL_RATE;
      lastFlywheelRampTime = millis();
    }
    // if we need to decrease speed and we are going forward
    else if (requestedPower < currentFlywheelPower)
    {
      currentFlywheelPower = currentFlywheelPower - FW_ACCEL_RATE;
      lastFlywheelRampTime = millis();
    }
  }

  return currentFlywheelPower;
}

// Aiming related functions
void Quarterback::aim(QBAim dir)
{
  moveTurret(heading, power, relativeToRobot, ramp);
  while (turretMoving && !testForDisableOrStop())
  {
    updateTurretMotionStatus();
    delay(10);
  }
}
#pragma endregion

void Quarterback::updateTurretMotionStatus()
{
  // if (utmsCtr >= UTMS_CTR_MAX) {
  //   utmsCtr = 0;
  Serial.print(F("update called with ctec = "));
  Serial.print(currentTurretEncoderCount);
  Serial.print(F("; ttec = "));
  Serial.print(targetTurretEncoderCount);
  Serial.print(F("; error (ct) = "));
  Serial.println(fabs((currentTurretEncoderCount % QB_COUNTS_PER_TURRET_REV) - targetTurretEncoderCount));
  // } else {
  //   utmsCtr++;
  // }
  // determines if encoder is within "spec"
  if (turretMoving && fabs((currentTurretEncoderCount % QB_COUNTS_PER_TURRET_REV) - targetTurretEncoderCount) < QB_TURRET_THRESHOLD)
  {
    turretMoving = false;
    setTurretSpeed(0);
  }
}

// deprecated
void Quarterback::turretDirectionChanged()
{
  if (currentTurretSpeed > 0 && targetTurretSpeed < 0)
  { // going CW, trying to go CCW
    currentTurretEncoderCount -= slopError;
    targetTurretEncoderCount -= slopError;
  }
  else if (currentTurretSpeed < 0 && targetTurretSpeed > 0)
  { // going CCW, trying to go CW
    currentTurretEncoderCount += slopError;
    targetTurretEncoderCount += slopError;
  }
}

//* get current heading in degrees
int16_t Quarterback::getCurrentHeading()
{
  return (int)((double)currentTurretEncoderCount / QB_COUNTS_PER_TURRET_DEGREE) % 360;
}

// Function to normalize an angle to the range [0, 360)
int Quarterback::NormalizeAngle(int angle)
{
  while (angle < 0)
  {
    angle += 360;
  }
  angle %= 360; // Ensure angle is within [0, 360) range
  return angle;
}

// Function to calculate the shortest rotation direction
// Returns -1 for counterclockwise, 1 for clockwise, or 0 if no rotation needed
int Quarterback::CalculateRotation(float currentAngle, float targetAngle)
{
  currentAngle = NormalizeAngle(currentAngle);
  targetAngle = NormalizeAngle(targetAngle);

  int positiveDegreeCount = currentAngle;
  int negativeDegreeCount = currentAngle;
  int iter = 0;

  while (NormalizeAngle(negativeDegreeCount) != targetAngle && NormalizeAngle(positiveDegreeCount) != targetAngle)
  {
    positiveDegreeCount++;
    negativeDegreeCount--;
    iter++;
  }

  if (iter == 0)
  {
    return 0;
  }
  else if (NormalizeAngle(negativeDegreeCount) == targetAngle)
  {
    return iter;
  }
  else
  {
    return iter * -1;
  }
}

// not currently used
int16_t Quarterback::findNearestHeading(int16_t targetHeading, int16_t currentHeading)
{
  // assuming targetHeading is positive
  int16_t positiveHeading = targetHeading;

  // if targetHeading is negative, convert to a positive heading
  if (targetHeading < 0)
  {
    positiveHeading = targetHeading + 360;
  }

  // properly constrain headings to be within (-360, +360)
  positiveHeading %= 360;
  int16_t negativeHeading = positiveHeading - 360;
  if (negativeHeading == -360) // same as if (positiveHeading == 0)
    negativeHeading = 0;

  // calculate which heading is closer to the current heading
  int16_t adjustedCurrentHeading = currentHeading % 360;

  if (abs(adjustedCurrentHeading - positiveHeading) < abs(adjustedCurrentHeading - negativeHeading))
  {
    // negative heading is closer
    return (adjustedCurrentHeading - positiveHeading);
  }
  else
  {
    // positive heading is closer
    return (adjustedCurrentHeading - negativeHeading);
  }
}

// not currently used
int16_t Quarterback::findNearestHeading(int16_t targetHeading)
{
  return findNearestHeading(targetHeading, currentRelativeHeading);
}

#pragma region Assembly
void Quarterback::aimAssembly(AssemblyAngle angle, bool force)
{
  if (enabled)
  {
    if (!assemblyMoving)
    {
      targetAssemblyAngle = angle;

      if (targetAssemblyAngle != currentAssemblyAngle || force)
      {
        moveAssemblySubroutine();
      }

      //* force is a blocking routine to ensure it works without interruption
      //* do not use force frequently as it can strain the motor
      //* this should only be used on startup
      if (force)
      {
        // also allow emergency stop
        while ((millis() - assemblyStartTime) <= QB_ASSEMBLY_TILT_DELAY && !testForDisableOrStop())
        {
          NOP();
        }
        currentAssemblyAngle = targetAssemblyAngle;
        assemblyMoving = false;
        assemblyMotor.write(0);
      }
    }
    else if ((millis() - assemblyStartTime) > QB_ASSEMBLY_TILT_DELAY)
    {
      currentAssemblyAngle = targetAssemblyAngle;
      assemblyMoving = false;
      assemblyMotor.write(0);
    }
  }
  else
  {
    assemblyMotor.write(0);
  }
}

//! this is a dangerous function to call
// should only be called with known good state
void Quarterback::moveAssemblySubroutine()
{
  if (targetAssemblyAngle == straight)
  {
    assemblyMotor.write(QB_ASM_SPEED);
  }
  else if (targetAssemblyAngle == angled)
  {
    assemblyMotor.write(-1.25 * QB_ASM_SPEED);
  }
  assemblyStartTime = millis();
  assemblyMoving = true;
}
#pragma endregion

#pragma region Cradle
//! this is a dangerous function to call
// should only be called with known good state
void Quarterback::moveCradleSubroutine()
{
  // Serial.print(F("target neq current  | "));
  if (targetCradleState == forward)
  {
    // move forwards
    cradleActuator.write(1.0);
    cradleStartTime = millis();
    cradleMoving = true;
    // Serial.print(F("cradle moving forward  | "));
  }
  else if (targetCradleState == back)
  {
    // move backwards
    cradleActuator.write(-1.0);
    cradleStartTime = millis();
    cradleMoving = true;
    // Serial.print(F("cradle moving backward  | "));
  }
}

void Quarterback::moveCradle(CradleState state, bool force)
{
  if (enabled)
  {
    if (!cradleMoving)
    {
      targetCradleState = state;

      // Serial.print(F("current state: "));
      // if (currentCradleState == forward) {
      //   Serial.print(F("forward  | "));
      // } else if (currentCradleState == back) {
      //   Serial.print(F("backward  | "));
      // }

      // Serial.print(F("target state: "));
      // if (targetCradleState == forward) {
      //   Serial.print(F("forward  | "));
      // } else if (targetCradleState == back) {
      //   Serial.print(F("backward  | "));
      // }

      if (targetCradleState != currentCradleState || force)
      {
        moveCradleSubroutine();
      }

      //* force is a blocking routine to ensure it works without interruption
      //* do not use force frequently as it can strain the actuator
      //* this should only be used on startup
      if (force)
      {
        // also allow emergency stop
        while ((millis() - cradleStartTime) <= QB_CRADLE_TRAVEL_DELAY && !testForDisableOrStop())
        {
          NOP();
        }
        currentCradleState = targetCradleState;
        cradleMoving = false;
        cradleActuator.write(0);
      }

      // Serial.print(F("past delay? "));
      // Serial.print((millis() - cradleStartTime) > QB_CRADLE_TRAVEL_DELAY);
      // Serial.print(F(" | "));
    }
    else if ((millis() - cradleStartTime) > QB_CRADLE_TRAVEL_DELAY)
    {
      currentCradleState = targetCradleState;
      cradleMoving = false;
      cradleActuator.write(0);
      // Serial.print(F("cradle stopped  | "));
    }
  }
  else
  {
    cradleActuator.write(0);
  }

  // Serial.println();
}
#pragma endregion

#pragma region Flywheels
void Quarterback::setFlywheelSpeed(float absoluteSpeed)
{
  // update the motors so they are spinning at the new speed
  if (enabled)
  {
    // if current speed is not the passed speed, change the motor speed. this is only to avoid unnecessary writes
    if (fabs(currentFlywheelSpeed - absoluteSpeed) > STICK_DEADZONE)
    {
      // constrain to the first and last values of the flywheel speed array.
      // the first value should be the slow intake speed -- the flywheels should NEVER spin more quickly *inwards* than this.
      // the last value should be the maximum speed (ordinarily 1, but we may change this).
      targetFlywheelSpeed = constrain(absoluteSpeed, flywheelSpeeds[0], flywheelSpeeds[QB_TURRET_NUM_SPEEDS - 1]);
      flywheelLeftMotor.write(targetFlywheelSpeed);
      flywheelRightMotor.write(-targetFlywheelSpeed);
      currentFlywheelSpeed = targetFlywheelSpeed; //! for now, will probably need to change later, like an interrupt
    }
  }
  else
  {
    flywheelLeftMotor.write(0);
    flywheelRightMotor.write(0);
  }
}

void Quarterback::setFlywheelSpeedStage(FlywheelSpeed stage)
{
  targetFlywheelStage = stage;
  setFlywheelSpeed(flywheelSpeeds[static_cast<uint8_t>(targetFlywheelStage)]);
  currentFlywheelStage = targetFlywheelStage;
}

void Quarterback::adjustFlywheelSpeedStage(SpeedStatus speed)
{
  uint8_t idx = static_cast<uint8_t>(currentFlywheelStage);

  // Change the speed stage based on whether the user wants to increase or decrease
  if (speed == INCREASE && idx < QB_TURRET_NUM_SPEEDS - 1)
  {
    idx++;
  }
  else if (speed == DECREASE && idx > 0)
  {
    idx--;
  }

  setFlywheelSpeedStage(static_cast<FlywheelSpeed>(idx));
}
#pragma endregion

#pragma region Auto Mode
void Quarterback::switchMode()
{
  if (mode == manual)
  {
    switchMode(automatic);
  }
  else if (mode == automatic)
  {
    switchMode(manual);
  }
}

void Quarterback::switchMode(TurretMode mode)
{
  this->mode = mode;
}

void Quarterback::switchTarget(TargetReceiver target)
{
  switchMode(automatic);
  this->target = target;
  // todo: not sure if this needs more functionality?
}
#pragma endregion

#pragma region Macros
void Quarterback::loadFromCenter()
{
  this->runningMacro = true;
  aimAssembly(straight);
  setFlywheelSpeedStage(slow_inwards);
  moveCradle(back);
  // zeroTurret();
  this->runningMacro = false;
}

void Quarterback::handoff()
{
  this->runningMacro = true;
  aimAssembly(straight);
  int16_t targetHeading = (getCurrentHeading() + 130) % 360;
  calculateHeadingMag();
  targetAbsoluteHeading = headingDeg + 180;
  targetAbsoluteHeading %= 360;

  if (useMagnetometer)
  {
    // moveTurretAndWait(targetHeading);
    // Use the magnetometer to make sure we get close to the requested angle
    // calculateHeadingMag();
    // holdTurretStill();
    // cradleActuator.write(1.0);
    setFlywheelSpeedStage(slow_outwards);
    long currentTime = millis();
    while ((currentTime + 4000) > millis())
    {
      calculateHeadingMag();
      turretPIDSpeed = turretPIDController(headingDeg, (float)targetAbsoluteHeading, .01, 0, 0, .25);
      setTurretSpeed(turretPIDSpeed, true);
    }
    cradleActuator.write(1.0);
    delay(2000);
  }
  else
  {
    targetHeading += 10;
    targetHeading %= 360;
    moveTurretAndWait(targetHeading);
    cradleActuator.write(1.0);
    setFlywheelSpeedStage(slow_outwards);
    delay(2000);
    setFlywheelSpeedStage(stopped);
  }
  cradleActuator.write(-1);
  delay(2000);
  cradleActuator.write(0);
  this->runningMacro = false;
}

void Quarterback::changeFWSpeed(SpeedStatus speed)
{
  // Debounce for button press
  if (millis() - lastDBFWChange >= DEBOUNCE_WAIT)
  {
    // Change the speed factor based on whether the user wants to increase or decrease (HARD CODED)
    switch (speed)
    {
    case INCREASE:
      arrayPos++;
      break;
    case DECREASE:
      arrayPos--;
      break;
    }

  lis3mdl.setOperationMode(LIS3MDL_CONTINUOUSMODE);
  Serial.print("Operation mode set to: ");
  // Single shot mode will complete conversion and go into power down
  switch (lis3mdl.getOperationMode())
  {
  case LIS3MDL_CONTINUOUSMODE:
    Serial.println("Continuous");
    break;
  case LIS3MDL_SINGLEMODE:
    Serial.println("Single mode");
    break;
  case LIS3MDL_POWERDOWNMODE:
    Serial.println("Power-down");
    break;
  }

  lis3mdl.setDataRate(LIS3MDL_DATARATE_155_HZ);
  // You can check the datarate by looking at the frequency of the DRDY pin
  Serial.print("Data rate set to: ");
  switch (lis3mdl.getDataRate())
  {
  case LIS3MDL_DATARATE_0_625_HZ:
    Serial.println("0.625 Hz");
    break;
  case LIS3MDL_DATARATE_1_25_HZ:
    Serial.println("1.25 Hz");
    break;
  case LIS3MDL_DATARATE_2_5_HZ:
    Serial.println("2.5 Hz");
    break;
  case LIS3MDL_DATARATE_5_HZ:
    Serial.println("5 Hz");
    break;
  case LIS3MDL_DATARATE_10_HZ:
    Serial.println("10 Hz");
    break;
  case LIS3MDL_DATARATE_20_HZ:
    Serial.println("20 Hz");
    break;
  case LIS3MDL_DATARATE_40_HZ:
    Serial.println("40 Hz");
    break;
  case LIS3MDL_DATARATE_80_HZ:
    Serial.println("80 Hz");
    break;
  case LIS3MDL_DATARATE_155_HZ:
    Serial.println("155 Hz");
    break;
  case LIS3MDL_DATARATE_300_HZ:
    Serial.println("300 Hz");
    break;
  case LIS3MDL_DATARATE_560_HZ:
    Serial.println("560 Hz");
    break;
  case LIS3MDL_DATARATE_1000_HZ:
    Serial.println("1000 Hz");
    break;
  }

  lis3mdl.setRange(LIS3MDL_RANGE_4_GAUSS);
  Serial.print("Range set to: ");
  switch (lis3mdl.getRange())
  {
  case LIS3MDL_RANGE_4_GAUSS:
    Serial.println("+-4 gauss");
    break;
  case LIS3MDL_RANGE_8_GAUSS:
    Serial.println("+-8 gauss");
    break;
  case LIS3MDL_RANGE_12_GAUSS:
    Serial.println("+-12 gauss");
    break;
  case LIS3MDL_RANGE_16_GAUSS:
    Serial.println("+-16 gauss");
    break;
  }

  lis3mdl.setIntThreshold(500);
  lis3mdl.configInterrupt(false, false, true, // enable z axis
                          true,               // polarity
                          false,              // don't latch
                          true);              // enabled!
}

/**
 * @brief Spins the turret 360 degrees slowly to allow magnetometer to calibrate itself on startup
 * @author George Rak
 * @date 4-9-2024
 */
void Quarterback::calibMagnetometer()
{

  mag_yVal = 0;
  mag_xVal = 0;
  mag_xMax = -1000000;
  mag_xMin = 1000000;
  mag_xHalf = 0;
  mag_yMax = -1000000;
  mag_yMin = 1000000;
  mag_yHalf = 0;
  mag_xSign = false;
  mag_ySign = false;

  northHeadingDegrees = 0;

  int degreesMove = 360;
  targetTurretEncoderCount = (int)round((double)degreesMove * QB_COUNTS_PER_TURRET_DEGREE);
  turretMoving = true;
  setTurretSpeed(QB_HOME_MAG * copysign(1, degreesMove), true);
  // Loop until the target encoder count has been achieved
  while (currentTurretEncoderCount < targetTurretEncoderCount && !testForDisableOrStop())
  {
    // get X Y and Z data all at once
    lis3mdl.read();

    // Constantly looking for min and max values of X
    if (lis3mdl.x < mag_xMin && lis3mdl.x != -1 && lis3mdl.x != 0)
    {
      mag_xMin = lis3mdl.x;
    }
    else if (lis3mdl.x > mag_xMax && lis3mdl.x != -1 && lis3mdl.x != 0)
    {
      mag_xMax = lis3mdl.x;
    }

    // Adjusting X values to range from + or - values rather than all positive
    mag_xHalf = abs(mag_xMax) - abs(mag_xMin);
    mag_xHalf /= 2;
    mag_xHalf += abs(mag_xMin);

    // Constantly looking for min and max values of Y
    if (lis3mdl.y < mag_yMin && lis3mdl.y != -1 && lis3mdl.y != 0 && lis3mdl.y != 10)
    {
      mag_yMin = lis3mdl.y;
    }
    else if (lis3mdl.y > mag_yMax && lis3mdl.y != -1 && lis3mdl.y != 0 && lis3mdl.y != 10)
    {
      mag_yMax = lis3mdl.y;
    }

    // Adjusting Y values to range from + or - values rather than all positive
    mag_yHalf = abs(mag_yMax) - abs(mag_yMin);
    mag_yHalf /= 2;
    mag_yHalf += abs(mag_yMin);

    /*DEBUGGING PRINTOUTS*/
    // Serial.print("X:  "); Serial.print(lis3mdl.x);
    // Serial.print("\tY:  "); Serial.print(lis3mdl.y);
    // Serial.print("\tMinX:  "); Serial.print(mag_xMin);
    // Serial.print("\tMaxX:  "); Serial.print(mag_xMax);
    // Serial.print("\tMinY:  "); Serial.print(mag_yMin);
    // Serial.print("\tMaxY:  "); Serial.print(mag_yMax);
    // Serial.println();
  }

  setTurretSpeed(0, true);

  // Updating variables that will be used to handle other two possible sign cases for each value
  if ((mag_xMax + mag_xMin) < 0)
  {
    mag_xSign = true;
  }
  if ((mag_yMax + mag_yMin) < 0)
  {
    mag_ySign = true;
  }
  //

  calculateHeadingMag();

  Serial.print(F("Magnetometer reading after calib: "));
  Serial.print(headingDeg);
  Serial.print(F("\tEncoder after calib:"));
  Serial.println(currentTurretEncoderCount);

  // delay(5000);

  currentTurretEncoderCount = 0;
  targetTurretEncoderCount = 0;

  turretMoving = true;
  moveTurretAndWait(0, true); // go to zero of the encoder

  magnetometerCalibrated = true;

  calculateHeadingMag(); // calculate current value of magnetometer (headingDeg)

  Serial.print("Target Abs Heading Before 0: ");
  Serial.print(targetAbsoluteHeading);
  Serial.print("\tNorth Heading Degrees: ");
  this->northHeadingDegrees = headingDeg; // + 45;
  Serial.print(northHeadingDegrees);
  Serial.println();

  // delay(2000);

  // from here on out, headingDeg and targetAbsoluteHeading are offset by northHeadingDegrees
  // headingDeg = 0;
  targetAbsoluteHeading = 0;

  Serial.println("Magnetometer has been calibrated!");
  eIntegral = 0;
  previousTime = millis();

  setTurretSpeed(0);

  // delay(5000);
}

/**
 * @brief Uses the data collected at calibration to calculate the current heading relative to magnetic north
 * @author George Rak
 * @date 4-9-2024
 */
void Quarterback::calculateHeadingMag()
{
  // Only run the code in here if the calibration has been done to the magnetometer
  if (magnetometerCalibrated)
  {
    lis3mdl.read();
    // Calculate the current angle of the turret based on the calibration data
    if (mag_xSign)
    {
      mag_xVal = lis3mdl.x + mag_xHalf;
    }
    else
    {
      mag_xVal = lis3mdl.x - mag_xHalf;
    }

    if (mag_ySign)
    {
      mag_yVal = lis3mdl.y + mag_yHalf;
    }
    else
    {
      mag_yVal = lis3mdl.y - mag_yHalf;
    }

    // Evaluate both ranges of X and Y then scale the smaller value to be within the same range as the larger
    if (mag_yHalf > mag_xHalf)
    {
      mag_xVal = (double)((double)mag_xVal / ((double)mag_xHalf)) * (double)mag_yHalf;
    }
    else if (mag_xHalf > mag_yHalf)
    {
      mag_yVal = (double)((double)mag_yVal / ((double)mag_yHalf)) * (double)mag_xHalf;
    }

    // Calculate angle in radians
    if (mag_xVal != -1 && mag_xVal != 0 && mag_yVal != 0 && mag_yVal != -1)
    {
      headingRad = atan2(mag_yVal, mag_xVal);
    }

    // Convert to degrees
    headingDeg = headingRad * 180 / M_PI;

    // If the degrees are negative then they just need inversed plus 180
    if (headingDeg < 0)
    {
      headingDeg += 360;
    }

    // integrate offset into measurement
    // + 180 - QB_NORTH_OFFSET
    headingDeg = ((int)headingDeg) + northHeadingDegrees;
    if (headingDeg > 360)
      headingDeg = ((int)headingDeg) % 360;

    /*DEBUGGING PRINTOUTS*/
    // Serial.print("X:  "); Serial.print(lis3mdl.x);
    // Serial.print("\tY:  "); Serial.print(lis3mdl.y);
    // Serial.print("\tMinX:  "); Serial.print(mag_xMin);
    // Serial.print("\tMaxX:  "); Serial.print(mag_xMax);
    // Serial.print("\tMinY:  "); Serial.print(mag_yMin);
    // Serial.print("\tMaxY:  "); Serial.print(mag_yMax);
    // Serial.print("\txAdapt:  "); Serial.print(mag_xVal);
    // Serial.print("\tyAdapt:  "); Serial.print(mag_yVal);
    // Serial.print("\tHeading [deg]:   "); Serial.print(headingDeg);
    // Serial.println();
  }
}
#pragma endregion

#pragma region PID
/**
 * @brief Checks if the turret should be held still and runs the PID loop setting turret speed equal to PWM value calculated
 * @author George Rak
 * @date 4-9-2024
 */
void Quarterback::holdTurretStill()
{
  if (magnetometerCalibrated)
  {
    int maxSpeed = .2;
    if (motor1Value > 25 || motor2Value > 25)
    {
      // We should limit the rotation rate of the turret since the base is moving as well and we don't want the robot to flip
      maxSpeed = .125;
    }

    // Run the PID loop
    turretPIDSpeed = turretPIDController(headingDeg, (float)targetAbsoluteHeading, kp, kd, ki, .2);
    setTurretSpeed(turretPIDSpeed, true);
  }
}

/**
 * @brief PID controller to hold the turret still (gains tuned, not calculated)
 * @author George Rak
 * @date 4-9-2024
 */
float Quarterback::turretPIDController(float current, float target, float kp, float kd, float ki, float maxSpeed)
{
  if (maxSpeed > .5)
  {
    maxSpeed = .5;
  }
  else if (maxSpeed < -.5)
  {
    maxSpeed = -.5;
  }

  // Measure the time elapsed since last iteration
  long currentTime = millis();
  float deltaT = ((float)(currentTime - previousTime));

  // PID loops should update as fast as possible but if it waits too long this could be a problem
  if (deltaT > QB_TURRET_PID_MIN_DELTA_T && deltaT < QB_TURRET_PID_MAX_DELTA_T)
  {

    // Find which direction will be closer to requested angle
    int e = CalculateRotation(current, target);

    // Taking the average of the error
    prevErrorVals[prevErrorIndex] = e;
    prevErrorIndex++;
    prevErrorIndex %= PID_ERROR_AVG_ARRAY_LENGTH;

    // For the first one populate the average so it does not freak out
    if (firstAverage)
    {
      for (int i = 0; i < PID_ERROR_AVG_ARRAY_LENGTH; i++)
      {
        prevErrorVals[i] = e;
      }
      firstAverage = false;
    }

    // Taking the avergage for error
    int avgError = 0;
    for (int i = 0; i < PID_ERROR_AVG_ARRAY_LENGTH; i++)
    {
      avgError += prevErrorVals[i];
    }
    avgError /= PID_ERROR_AVG_ARRAY_LENGTH;
    e = avgError;

    // Calculate the derivative and integral values
    float eDerivative = (e - ePrevious);
    eIntegral = eIntegral + e * .01;

    // Compute the PID control signal
    float u = (kp * e) + (ki * eIntegral) + (kd * eDerivative);

    // Constrain output PWM values to -.2 to .2
    if (u > maxSpeed)
    {
      u = maxSpeed;
    }
    else if (u < -maxSpeed)
    {
      u = -maxSpeed;
    }

    // If PWM value is less than the minimum PWM value needed to move the robot,
    if (abs(u) < QB_MIN_PWM_VALUE)
    {
      u = 0.0;
    }

    // If the robot gets within an acceptable range then send error etc to 0
    if (abs(e) < QB_TURRET_PID_THRESHOLD)
    {
      e = 0;
      eDerivative = 0;
      eIntegral = 0;
      ePrevious = 0;
    }

    Serial.print("DeltaT: ");
    Serial.print(deltaT);
    Serial.print("\tError: [deg]:  ");
    Serial.print(e);
    Serial.print("\tP: ");
    Serial.print((kp * e), 4);
    Serial.print("\tI: ");
    Serial.print((ki * eIntegral), 4);
    Serial.print("\tD:\t");
    Serial.print((kd * eDerivative), 4);
    Serial.print("\tPWM Value: ");
    Serial.print(u, 4);
    Serial.print("\tCurrent [deg]: ");
    Serial.print(current, 0);
    Serial.print("\tTarget [deg]: ");
    Serial.print(target);
    Serial.println();

    // Update variables for next iteration
    previousTime = currentTime;
    ePrevious = e;

    // Constrain the values that are sent to the motor while keeping sign
    u = copysign(constrain(abs(u), 0, 1), u); // TODO: maybe not necessary?
    if (e == 0)
    {
      u = 0.0;
    }

    return -u;
  }
  else if (deltaT > QB_TURRET_PID_BAD_DELTA_T)
  {
    // Drop the value if the time since last loop is too high so that errors don't spike
    previousTime = currentTime;
    return turretPIDSpeed;
  }
  else
  {
    // If the loop runs faster than the minimum time just return the last value and wait for next loop
    return turretPIDSpeed;
  }
}
#pragma endregion

#pragma region Stabilization
/**
 * @brief Reads a UART communication from the other ESP mounted to the turret. This ESP currently provides the speed of both motors on the drivetrain so we know if the robot is moving
 * @author George Rak
 * @date 5-14-2024
 */
void Quarterback::updateReadMotorValues()
{
  recievedMessage = "";
  // While there are characters available in the buffer read each one individually
  while (Uart_Turret.available())
  {
    char character = Uart_Turret.read();
    // Added a delimeter between messages since loop times are different and multiple messages might come in before they are read and the buffer is cleared
    // Since they are coming so fast and there is no need to remember past values only the most recent is kept
    if (character == '~')
    {
      if (Uart_Turret.available())
      {
        recievedMessage = "";
      }
    }
    else
    {
      recievedMessage += character;
    }
  }
  // The Server client relationship between the ESPs knows if they disconnect so it is possible that they might send DISCONNECTED over the communication instead of values, in this case set the value to the max so that the turret spins slower
  if (recievedMessage != "")
  {
    if (recievedMessage == "DISCONNECTED")
    {
      motor1Value = 100;
      motor2Value = 100;
    }
    else
    {
      // Doing some string formatting here, a delimiter was added between the data to help keep them separate for motor #1 and motor #2
      motor1Value = (recievedMessage.substring(0, recievedMessage.indexOf('&'))).toInt();
      motor2Value = (recievedMessage.substring(recievedMessage.indexOf('&') + 1)).toInt();
    }
  }
  // Serial.print("Motor1: ");
  // Serial.print(motor1Value);
  // Serial.print("\tMotor2: ");
  // Serial.print(motor2Value);
  // Serial.println();
}
#pragma endregion