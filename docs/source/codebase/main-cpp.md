# `main.cpp`
## Description
- This file acts as a central platform for all of the robots' functionality.
- The `setup()` function initializes several aspects of the robot, including the ESP32 pins, the robot and drive objects, and the LED lights.
- The `loop()` function repeatedly reads inputs from the PS5 controller and issues corresponding instructions to the robot.
- The function `onConnection()` prints a message when a controller is connected to the robot.
- The function `onDisconnect()` prints a message when a previously connected controller disconnects from the robot.

## Included Headers
- `Arduino.h`
- `ps5Controller.h`
- [`PolarRobotics.h`](./polar-robotics-h.md)
- [`Pairing/pairing.h`](./pairing/pairing-h.md)
- [`Utilities/ConfigManager.h`](./utilities/config-manager-h.md)
- [`Robot/Lights.h`](./robot/lights-h.md)
- [`Robot/Robot.h`](./robot/robot-h.md)
- [`Robot/Lineman.h`](./robot/lineman-h.md)
- [`Robot/Center.h`](./robot/center-h.md)
- [`Robot/MecanumCenter.h`](./robot/mecanum-center-h.md)
- [`Robot/Kicker.h`](./robot/kicker-h.md)
- [`Robot/Quarterback.h`](./robot/quarterback-h.md)
- [`Robot/QuarterbackBase.h`](./robot/quarterback-base-h.md)
- [`Robot/QuarterbackTurret.h`](./robot/quarterback-turret-h.md)
- [`Drive/Drive.h`](./drive/drive-h.md)
- [`Drive/DriveMecanum.h`](./drive/drive-mecanum-h.md)