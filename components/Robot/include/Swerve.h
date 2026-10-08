#pragma once
#include <CAN.h>
#include <Robot.h>
#include <inverseKin.h>

#include <cstdint>

class Swerve : public Robot
{
 public:
  Swerve(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4);
  void action();

 private:
  void getInputs();

  InverseKin kinematics;
  uint8_t can_data[64];
};