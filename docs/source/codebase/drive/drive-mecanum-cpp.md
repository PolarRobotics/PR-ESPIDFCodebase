# `DriveMecanum.cpp`
## Description
- This file, which contains the functions of the DriveMecanum class, controls several parameters related to the driving of the robots with Mecanum wheels. The purpose of this code is to ensure that the robots respond appropriately to controller input. Based on stick inputs from a controller, this code will alter the speed of a robot's motors. The code also incorporates ramping to ensure that the robots don't lose control due to the motors starting up too quickly.
- This code is intended to be used with robots featuring the Mecanum wheel configuration, with each of the four wheels connected to its own motor. Further information on the mechanics of Mecanum driving can be found [here](https://gm0.org/en/latest/docs/software/tutorials/mecanum-drive.html) or seen in the diagram below.
![Mecanum Wheel Diagram](../../_static/images/codebase/mecanum_wheel.svg){w=500px align=center}

## Functions

| Name                     | Description                                                                                                                      |
| ------------------------ | -------------------------------------------------------------------------------------------------------------------------------- |
| `DriveMecanum`           | The only difference for this subclass constructor is that it initializes four motors instead of two, as for a normal drivetrain. |
| `setupMotors`            | Calls `setup()` on all the `Motor` objects (attaches pins, etc.)                                                                 |
| `setStickPwr`            | Same as the base class.                                                                                                          |
| `generateMotorValuesOld` | An alternative control value generation function based on trigonometry. Presently unused.                                        |
| `generateMotorValues`    | A simpler control value generation function. Currently used.                                                                     |
| `emergencyStop`          | Immediately write zero power to all motors.                                                                                      |
| `update`                 | Same as base class, except that it controls four motors.                                                                         |
| `printDebugInfo`         | Prints debug information (stick powers and motor power levels).                                                                  |

## Included Headers
- `Arduino.h`
- `Drive/Drive.h`
- `DriveMecanum.h`