# `DriveMecanum.h`
## Description
### Constants
- This following constants are `#define`d in `DriveMecanum.h`:

    | Constant               | Value  |
    | ---------------------- | ------ |
    | `MC_NUM_MOTORS`        | 4      |
    | `MC_ACCELERATION_RATE` | 0.0375 |
    | `MC_STICK_DEADZONE`    | 0.075  |

### `DriveMecanum` class
- This is a subclass of [Drive](./drive-h.md#drive-class), specialized for a four-motor mecanum drivetrain.
  - There are not a substantial amount of high-level differences from the base class.
- Functions defined in [DriveMecanum.cpp](./drive-cpp.md) are prototyped.

## Include Headers
- `Arduino.h`
- [`Drive/Drive.h`](./drive-h.md)