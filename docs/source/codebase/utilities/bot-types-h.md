# `BotTypes.h`
## Description
- This file defines the following enums:
    | Enum    | States |
    |---------|--------|
    | BotType | lineman, receiver, running_back, center, kicker, kicker_old, mecanum_center, quarterback_old, quarterback_base, quarterback_turret |
- This file also defines the following constants:
    - Standard Constants
        | Constant      | Value |
        |---------------|-------|
        | NUM_POSITIONS | 10    |
        | NUM_BOTS      | 21    |
    - Bot Aliases
        | Constant           | Value |
        |--------------------|-------|
        | BOT_IPP            | 0     |
        | BOT_I_PLUS_PLUS    | 0     |
        | BOT_SQRT_NEG_1     | 1     |
        | BOT_PI             | 2     |
        | BOT_RHO            | 3     |
        | BOT_2_72           | 4     |
        | BOT_SMILEY         | 5     |
        | BOT_GEQ            | 6     |
        | BOT_32_2           | 7     |
        | BOT_9_8            | 8     |
        | BOT_C              | 9     |
        | BOT_RB             | 9     |
        | BOT_PHI            | 10    |
        | BOT_CENTER         | 10    |
        | BOT_INF            | 11    |
        | BOT_QB             | 11    |
        | BOT_QB_OLD         | 11    |
        | BOT_THETA          | 12    |
        | BOT_OLD_KICKER     | 12    |
        | BOT_MC             | 13    |
        | BOT_MECANUM_CENTER | 13    |
        | BOT_QB_BASE        | 14    |
        | BOT_QB_BOTTOM      | 14    |
        | BOT_QB_TURRET      | 15    |
        | BOT_QB_TOP         | 15    |
        | BOT_LINEMAN_V1     | 16    |
        | BOT_420            | 17    |
        | BOT_24             | 18    |
        | BOT_25             | 19    |
        | BOT_TAU            | 20    |
        | BOT_KICKER         | 20    |
- In addition, this file also establishes the present configurations for each robot, recording relevant information including each robot's index number, name, type, motor type, gear ratio, wheel base, r minimum, and r maximum.
## Included Headers
- `Arduino.h`
- `Utilities/Pair.h`
- `Utilities/MotorTypes.h`
- `Utilities/DriveParameters.h`