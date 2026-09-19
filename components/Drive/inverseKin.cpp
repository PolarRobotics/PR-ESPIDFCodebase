/**
 * InverseKin.cpp
 * @brief implements inverse kinematics
 * @authors Ashton Beresford, Quentin Osterhage
 *
 *  Conversion and adaptation of inverse kinematics python code from PRB1/PRB2
 **/

#include "inverseKin.h"

std::unordered_map<std::string, std::pair<double, double>> speedScalars = {
    {"boost", {0.8, 1.0}}, {"slow", {0.25, 1.0}}, {"normal", {0.6, 1.0}}};

WheelVelocity InverseKin::calculateWheelVelocity(const double offset[2])
{
  // omega_max is the max that omega can be, essentially controlls the maximum
  // rotational speed and how quickly it gets to that point

  // Convert the rightStickX input to be a proper Omega for the problem

  // Each motor has a part of the linear velocities that is affected by the
  // angular velocity
  double vix = linear_vel_x + angular_vel * offset[1];
  double viy = linear_vel_y - angular_vel * offset[0];

  // Calculate magnitude and angle using atan2 for proper quadrant handling
  double velocity_magnitude = sqrt(vix * vix + viy * viy);
  double angle_radians = atan2(viy, vix);
  double angle_degrees = (angle_radians * 180.0 / PI);

  WheelVelocity result;
  result.magnitude = velocity_magnitude;
  result.angle_degrees = angle_degrees;
  return result;
}

void InverseKin::calculateVelocities()
{
  // Calculate velocities for each wheel
  wA_velocity = calculateWheelVelocity(wheelA_offset);
  wB_velocity = calculateWheelVelocity(wheelB_offset);
  wC_velocity = calculateWheelVelocity(wheelC_offset);
  wD_velocity = calculateWheelVelocity(wheelD_offset);

  // Baseplate calculation
  baseplate_velocity.magnitude =
      sqrt(linear_vel_x * linear_vel_x + linear_vel_y * linear_vel_y);
  baseplate_velocity.angle_degrees =
      (atan2(linear_vel_y, linear_vel_x) * 180.0 / PI);

  // Check for halt logic
  if (fabs(baseplate_velocity.magnitude) <= 1 && fabs(angular_vel) <= 1)
    halt = true;
  else
    halt = false;
}

void InverseKin::normalizeInputs()
{
  linear_vel_x = (leftStickX / 127.5f);
  linear_vel_y = (leftStickY / 127.5f);
  angular_vel = (rightStickX / 127.5f);

  controllerDeadzone(&linear_vel_x, false);
  controllerDeadzone(&linear_vel_y, false);
  controllerDeadzone(&angular_vel, true);
}

void InverseKin::controllerDeadzone(double *normalized_vel, bool angular)
{
  // stick deadzones
  // set to zero (no input) if within the set deadzone
  // subtacting STICK_DEADZONE and deviding by 1-STICK_DEADZONE normalize the
  // inputs to use the full 0-1 range
  if (fabs(*normalized_vel) < STICK_DEADZONE)
  {
    *normalized_vel = 0;
    return;
  }
  else if (*normalized_vel > 0)
  {
    *normalized_vel = (*normalized_vel - STICK_DEADZONE) / (1 - STICK_DEADZONE);
  }
  else if (*normalized_vel < 0)
  {
    *normalized_vel = (*normalized_vel + STICK_DEADZONE) / (1 - STICK_DEADZONE);
  }

  if (angular)
    *normalized_vel = *normalized_vel * omega;
  else
    *normalized_vel = *normalized_vel * scaled_velocity;
}

void InverseKin::debugPrint(int delay_time)
{
  // Print results to Serial
  if (halt) Serial.println("HALT");

  Serial.print("Right Stick X: ");
  Serial.println(rightStickX);
  Serial.print("Left Stick X: ");
  Serial.println(leftStickX);
  Serial.print("Left Stick Y: ");
  Serial.println(leftStickY);

  Serial.print("Normalized Stick X: ");
  Serial.println(linear_vel_x);
  Serial.print("Normalized Stick Y: ");
  Serial.println(linear_vel_y);
  Serial.print("Normalized Right Stick X (Omega): ");
  Serial.println(angular_vel);

  Serial.print("Wheel 1: magnitude=");
  Serial.print(wA_velocity.magnitude);
  Serial.print(", angle=");
  Serial.print(wA_velocity.angle_degrees);
  Serial.println("°");

  Serial.print("Wheel 2: magnitude=");
  Serial.print(wB_velocity.magnitude);
  Serial.print(", angle=");
  Serial.print(wB_velocity.angle_degrees);
  Serial.println("°");

  Serial.print("Wheel 3: magnitude=");
  Serial.print(wC_velocity.magnitude);
  Serial.print(", angle=");
  Serial.print(wC_velocity.angle_degrees);
  Serial.println("°");

  Serial.print("Wheel 4: magnitude=");
  Serial.print(wD_velocity.magnitude);
  Serial.print(", angle=");
  Serial.print(wD_velocity.angle_degrees);
  Serial.println("°");

  Serial.print("Baseplate: magnitude=");
  Serial.print(baseplate_velocity.magnitude);
  Serial.print(", angle=");
  Serial.print(baseplate_velocity.angle_degrees);
  Serial.println("°");
  Serial.println();

  delay(delay_time);
}

bool InverseKin::checkHalt() { return halt; }

void InverseKin::setSpeedScalars()
{
  scaled_velocity = speedScalars[BSN].first * MAX_VELOCITY;
  omega = speedScalars[BSN].second * MAX_OMEGA;
}

WheelData InverseKin::getData()
{
  calculateVelocities();
  WheelData data;

  data.wheels[0] = wB_velocity;  // Front left wheel
  data.wheels[1] = wA_velocity;  // Front right wheel
  data.wheels[2] = wC_velocity;  // Bottom left wheel
  data.wheels[3] = wD_velocity;  // Bottom right wheel

  return data;
}