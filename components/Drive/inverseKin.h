#pragma once

#include <Arduino.h>

#include <array>
#include <cmath>
#include <cstring>
#include <iostream>
#include <string>
#include <unordered_map>

// Deadzone and speed limits
#define STICK_DEADZONE 0.2  // arbitary value from 0 to 1

// Highest possible velocity and omega values for the robot
#define MAX_OMEGA 20.0
#define MAX_VELOCITY 2000

// Wheel offsets from center of robot (x, y)
constexpr double wheelB_offset[2] = {-4.55, 4.55};   // Front left wheel
constexpr double wheelA_offset[2] = {4.55, 4.55};    // Front right wheel
constexpr double wheelC_offset[2] = {-4.55, -4.55};  // Bottom left wheel
constexpr double wheelD_offset[2] = {4.55, -4.55};   // Bottom right wheel

// "BSN" Speed Scalars (Left: Velocity Scalar, Right: Omega Scalar)
extern std::unordered_map<std::string, std::pair<double, double>> speedScalars;

// Struct to hold velocity and angle for a single wheel
struct WheelVelocity
{
  double magnitude;
  double angle_degrees;
};

// Struct to hold velocity and angle for all wheels
struct WheelData
{
  WheelVelocity wheels[4];
};

class InverseKin
{
 private:
  // Max velocity and omega values for the robot; scales stick values, and is
  // determined by BSN mode (bumper input)
  double omega;
  double scaled_velocity;

  // Obtained by PS5 analog stick inputs
  double angular_vel;   // right stick x input
  double linear_vel_x;  // left stick x input
  double linear_vel_y;  // left stick y input

  // Calculated wheel velocities (angle and magnitude)
  WheelVelocity wB_velocity;
  WheelVelocity wA_velocity;
  WheelVelocity wC_velocity;
  WheelVelocity wD_velocity;
  WheelVelocity baseplate_velocity;

  // Set in calculateVelocities based on deadzone logic
  bool halt;

  /// @brief Calculate angle and velocity for a single wheel
  /// @param offset The [x, y] offset (in inches) of the wheel from the center
  /// of the robot
  /// @return A struct containing the magnitude and angle of the wheel velocity
  WheelVelocity calculateWheelVelocity(const double offset[2]);

  /// @brief Apply deadzone to normalized stick inputs and scale to max
  /// velocity/omega
  /// @param normalized_vel Stick input normalized to -1.0 to 1.0
  /// @param angular True if the input is for angular velocity, false for linear
  void controllerDeadzone(double *normalized_vel, bool angular);

  /// @brief Calculate inverse kinematics for all wheels
  void calculateVelocities();

 public:
  // For debugPrint
  int8_t leftStickX;
  int8_t leftStickY;
  int8_t rightStickX;

  std::string BSN;

  /// @brief Normalize stick inputs to -1.0 to 1.0 and apply deadzone
  void normalizeInputs();

  /// @brief Print debug information to Serial
  /// @param delay_time Time in ms to delay after printing (default 1000ms)
  void debugPrint(int delay_time = 1000);

  /// @brief  Check if the robot is halted by deadzone logic in
  /// calculateVelocities
  /// @return True if the robot is halted, false otherwise
  bool checkHalt();

  void setSpeedScalars();

  /// @brief Creates and obtains new wheel data
  /// @return A struct containing the velocity and angle for each wheel
  WheelData getData();
};