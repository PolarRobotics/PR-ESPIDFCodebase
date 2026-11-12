# `Quarterback.h`
## Description
- This file defines the following enums:
    | Enum        | States                        |
    |-------------|-------------------------------|
    | QBAim       | AIM_UP, AIM_DOWN              |
    | QBElevation | LOW_ELEVATION, HIGH_ELEVATION |
- This file also defines the following constants:
    | Constant             | Value |
    |----------------------|-------|
    | FLYWHEEL_SPEED_FULL  | 0.4   |
    | FLYWHEEL_STOP_SPEED  | 0     |
    | FW_TIME_INCREMENT    | 25    |
    | FW_BRAKE_PERCENTAGE  | 0.9   |
    | FW_ACCEL_RATE        | 0.025 |
    | SERVO_SPEED_UP       | 1     |
    | SERVO_SPEED_STOP     | 0     |
    | SERVO_SPEED_DOWN     | -1    |
    | MAX_ELEVATION        | 100   |
    | ELEVATION_PERIOD     | 3750  |
    | CONVEYOR_ON          | 1     |
    | CONVEYOR_OFF         | 0     |
    | DEBOUNCE_WAIT        | 250   |
    | NUM_SPEED_INCREMENTS | 4     |
- In addition, this file creates the Quarterback class, featuring relevant variable declarations and function prototypes. More information about these functions can be found in the documentation for Quarterback.cpp.
## Included Headers
- `Robot/Robot.h`
- `Robot/MotorControl.h`
- `ps5Controller.h`
