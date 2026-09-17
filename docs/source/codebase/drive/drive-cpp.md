# `Drive.cpp`
## Description
- This file, which contains the functions of the Drive class, controls several parameters related to the driving of the robots. The purpose of this code is to ensure that the robots respond appropriately to controller input. Based on stick inputs from a controller, this code will alter the speed of a robot's motors. The code also incorporates ramping to ensure that the robots don't lose control due to the motors starting up too quickly.
- This code is intended to be used with robots featuring the standard lineman configuration, with the back left wheel connected to a left motor, the back right wheel connected to a right motor, and the front wheel(s) not connected to any motor.

## Functions
| Name | Description 
|------|-------------
| `Drive` | The constructor for the Drive class. Parameters include robot type, [drive parameters](../utilities/drive-parameters-h.md), whether the robot has encoders, and which turn function to use.
| `setupMotors` | Performs setup for the motors, initializing the Motor objects and calling `attach` on the appropriate Arduino pins.
| `setMotorType` | Sets the drive motor type. Presently unused.
| `setStickPwr` | Used to retrieve the values from the joysticks of the PS5 controller. These are normalized and constrained to [-1, 1] before using them for drive control. The left stick Y-axis is used for speed (`stickForwardRev`) and the right stick X-axis is used for turning (`stickTurn`).
| `getForwardPower` | Returns `stickForwardRev`.
| `getTurnPower` | Returns `stickTurn`.
| `setSpeedScalar` | Adjusts the [`Speed`](./drive-h.md) (scalar) of the drive loop (boost, normal, slow, or brake).
| `setSpeedValue` | Directly adjusts the speed scalar to a non-predefined value, constrained to [-1, 1].
| `getSpeedScalar` | Returns the current speed scalar.
| `generateMotionValues` | Called during the drive loop, i.e., in `update()`. Generates the `requestedMotorPower` (pre-ramping) based on the stick inputs.
| `calcTurning` | Called in `generateMotionValues()`. Updates the `turnMotorValues` based on the value of `stickTurn` to enable turning.
| `emergencyStop` | Immediately write zero power to both motors.
| `printSetup` | Debug function to print all relevant parameters.
| `update` | The primary drive function to be called in the [main](../main-cpp.md) `loop()`. Takes `requestedMotorPower` for each motor, ramps individually, and write the percentage power to the `Motor` object.
| `getMotorWifiValue` | Used to retrieve the motor power for transmission between Quarterback V3 base and turret for stabilization.

## Included Headers
- `Arduino.h`
- [`Drive/Drive.h`](./drive-h.md)
- [`Robot/MotorControl.h`](../robot/motor-control-h.md)