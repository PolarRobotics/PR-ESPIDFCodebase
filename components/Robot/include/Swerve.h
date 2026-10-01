#pragma once
#include <Robot.h>
#include <inverseKin.h>

#include <cstdint>
class Swerve : public Robot
{
 public:
  Swerve(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4);
  void action() override;

 private:
  void getInputs();

  InverseKin kinematics;
  uint8_t can_data[64]{};
};

Swerve::Swerve(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4)
{
  (void)pin1;
  (void)pin2;
  (void)pin3;
  (void)pin4;
}

void Swerve::action()
{
  getInputs();
  const WheelData wheelData = kinematics.getData();
  if (!kinematics.checkHalt())
  {
    createDataCAN(wheelData, can_data);
    updateMotors(can_data);
  }
  else
  {
    mcp2515.sendMessage(&halt);
  }
}