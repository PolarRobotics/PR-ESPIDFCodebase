#include <Center.h>

/*
       ____   _____   _   _   _____   _____   ____
      / ___| | ____| | \ | | |_   _| | ____| |  _ \
     | |     |  _|   |  \| |   | |   |  _|   | |_) |
     | |___  | |___  | |\  |   | |   | |___  |  _ <
      \____| |_____| |_| \_|   |_|   |_____| |_| \_\

*/

/** Center Code
    --- Functions List ---
    Center
    clawControl - Based on what is inputed in main, opens, closes, or stops the claw.
    armControl - Based on what is inputed in main, raises, lowers, or stops the arm.
*/

/**
 * Description: Public function that starts the arm and claw motors and sets their starting status to stop.
 * Author: @ n-johnson.3
 * Date: 9/19/22
 **/
Center::Center(uint8_t armPin, uint8_t clawPin)
{
  this->armPin = armPin;
  this->clawPin = clawPin;
  armMotor.setup(armPin, small_12v);
  clawMotor.setup(clawPin, small_12v);
}

void Center::action()
{
  // Button handling moved to centerCallback() via ps5.attach()
}

/**
 * Description: Public helper function that checks the claw status and updates the claw motor accordingly.
 * Author: @ n-johnson.3
 * Date: 9/19/22
 **/
void Center::clawControl(ClawStatus target)
{
  if (target == ClawStatus::OPEN)
    clawMotor.write(0.1);
  else if (target == ClawStatus::CLOSE)
    clawMotor.write(-0.1);
  else if (target == ClawStatus::STOP_CLAW)
    clawMotor.write(0);
}

/**
 * Description: Public helper function that checks the arm status and updates the arm motor accordingly.
 * Author: @ n-johnson.3
 * Date: 9/19/22
 **/
void Center::armControl(ArmStatus target)
{
  if (target == ArmStatus::LOWER)
    armMotor.write(0.15);
  else if (target == ArmStatus::HIGHER)
    armMotor.write(-0.1);
  else if (target == ArmStatus::STOP_ARM)
    armMotor.write(0);
  else if (target == ArmStatus::HOLD)
    armMotor.write(-0.05);
}