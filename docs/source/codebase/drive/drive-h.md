# `Drive.h`
## Description
### Constants
- The following constants are `#define`d in `Drive.h`:

    | Constant                    | Value    |
    | --------------------------- | -------- |
    | `NUM_MOTORS`                | 2        |
    | `ACCELERATION_RATE`         | 0.00375f |
    | `RB_ACCELERATION_RATE`      | 0.0015f  |
    | `BRAKE_PERCENTAGE`          | 0.9      |
    | `TIME_INCREMENT`            | 5        |
    | `NORMAL_TURN_CONSTANT`      | 0.05     |
    | `TANK_MODE_PCT`             | 0.75     |
    | `RB_TANK_MODE_PCT`          | 0.5      |
    | `DRIFT_MODE_PCT`            | 0.8      |
    | `STICK_DEADZONE`            | 0.075    |
    | `THRESHOLD`                 | 0.00001  |
    | `FALCON_CALIBRATION_FACTOR` | 1.0f     |
    | `MOTOR_ZERO_OFFSET`         | 0.05f    |
    | `SMALL_12V_BOOST_PCT`       | 0.15f    |
    | `SMALL_12V_NORMAL_PCT`      | 0.1f     |
    | `SMALL_12V_SLOW_PCT`        | 0.05f    |
    | `BRAKE_BUTTON_PCT`          | 0        |

- The speed scalar values (boost, normal, and slow) are also defined for each motor type. 

### `Drive` Class
- Notable class fields include:
  - the robot type (`BotType`)
  - the motor type (`MotorType`)
  - [drive parameters](../utilities/drive-parameters-h.md)
  - turn adjustment variables
  - `stickForwardRev` and `stickTurn`
  - `requestedMotorPower`, `turnMotorValues`, and `lastRampPower` (all arrays with a size equal to `NUM_MOTORS`)
- The `Speed` (BNS) enum is defined with values `BOOST`, `NORMAL`, `SLOW`, `BRAKE`.
- Functions defined in [Drive.cpp](./drive-cpp.md) are prototyped.

## Included Headers
- `Arduino.h`
- [`Robot/MotorControl.h`](../robot/motor-control-h.md)
- [`PolarRobotics.h`](../polar-robotics-h.md)