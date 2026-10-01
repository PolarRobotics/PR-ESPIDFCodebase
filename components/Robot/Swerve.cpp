
#include <CAN.h>
#include <Swerve.h>
#include <ps5controller.h>

Swerve::Swerve(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4)
{
  (void)pin1;
  (void)pin2;
  (void)pin3;
  (void)pin4;

  initalizeCANDriver();
  setupPacket(&heartbeat, heartbeat_id, 0, heartbeat_dlc, heartbeat_data);
  setupPacket(&halt, halt_id, 0, halt_dlc, nullptr);
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

void Swerve::getInputs()
{
  if (ps5.R1())
    kinematics.BSN = "boost";
  else if (ps5.L1())
    kinematics.BSN = "slow";
  else
    kinematics.BSN = "normal";

  kinematics.leftStickX = ps5.LStickX();
  kinematics.leftStickY = ps5.LStickY();
  kinematics.rightStickX = ps5.RStickX();

  kinematics.setSpeedScalars();
  kinematics.normalizeInputs();
}