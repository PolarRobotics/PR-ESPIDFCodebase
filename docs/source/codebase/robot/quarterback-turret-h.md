# `QuarterbackTurret.h`
## Description
- This header defines the quarterback turret subsystem used by the QB robot: turret rotation, assembly tilt, cradle actuation, flywheel control, and PS5-driven modes.
- The qb_capstone_pt2 branch updates the older `automatic` naming to the current `tracking` mode, adds the `combine` preset flow, and documents the current preset flywheel values used by the turret.
- This file also declares the `QuarterbackTurret` class and the state variables used by the turret control loop, encoder ISR, and UART target data.

## Included Headers
- `Debouncer.h`
- `HardwareSerial.h`
- `MotorEnums.h`
- `PWMMotor.h`
- `Robot.h`
- `ps5Controller.h`

## Enums
| Enum | States |
|------|--------|
| `TurretMode` | `manual`, `combine`, `tracking` |
| `TurretUnits` | `degrees`, `counts` |
| `AssemblyAngle` | `straight`, `angled`, `unknownAngle` |
| `CradleState` | `forward`, `back` |
| `TargetReceiver` | `receiver_1`, `receiver_2` |
| `CombinePosition` | `combineLeft`, `combineStraight`, `combineRight` |
| `FlywheelSpeed` | `slow_inwards`, `stopped`, `slow_outwards`, `lvl1_outwards`, `lvl2_outwards`, `lvl3_outwards`, `maximum` |

## Constants
### Flywheel and combine presets
| Constant | Value |
|----------|-------|
| `QB_TURRET_NUM_SPEEDS` | `7` |
| `QB_TURRET_NUM_COMBINE_SPEEDS` | `5` |
| `QB_DECLINATION` | `-6.5` |

`flywheelSpeeds` = `{-0.1, 0, 0.1, 0.215, 0.31, 0.3875, 1.0}`

`combineSpeeds` = `{-0.1, 0, 0.215, 0.35, 0.45}`

- In `combine` mode, D-pad left/right changes the target combine angle while the flywheel speed can still be controlled with the left stick or preset increments.
- In `combine` mode, D-pad up advances the flywheel preset; D-pad down resets to the minimum preset (`-0.1`).

### Debounce and delay constants
| Constant | Value |
|----------|-------|
| `QB_BASE_DEBOUNCE_DELAY` | `50L` |
| `QB_ASSEMBLY_TILT_DELAY` | `700L` |
| `QB_CRADLE_TRAVEL_DELAY` | `750L` |
| `QB_CIRCLE_HOLD_DELAY` | `750L` |
| `QB_TRIANGLE_HOLD_DELAY` | `200L` |
| `QB_CROSS_HOLD_DELAY` | `200L` |
| `QB_TURRET_INTERPOLATION_DELAY` | `5L` |
| `QB_TURRET_THRESHOLD` | `35` |
| `QB_TURRET_STICK_SCALE_FACTOR` | `0.15` |

### Speed constants
| Constant | Value |
|----------|-------|
| `QB_MIN_PWM_VALUE` | `0.08` |
| `QB_HOME_PCT` | `0.05` |
| `QB_HANDOFF` | `0.1` |
| `QB_HOME_MAG` | `0.1` |
| `QB_ASM_SPEED` | `0.3` |

### Turret angle and encoder constants
| Constant | Value |
|----------|-------|
| `QB_COUNTS_PER_ENCODER_REV` | `1250` |
| `QB_COUNTS_PER_TURRET_REV` | `13417` |
| `QB_COUNTS_PER_TURRET_DEGREE` | `13417 / 360.0` |
| `QB_TURRET_ZERO_OFFSET_DEGREES` | `-8.2` |
| `QB_TURRET_SLOP_COUNTS` | `540` |

### Homing and PID constants
| Constant | Value |
|----------|-------|
| `QB_TURRET_STOP_LOOP_DELAY_MS` | `10` |
| `QB_TURRET_STOP_THRESHOLD_MS` | `500` |
| `QB_TURRET_HOME_STOP_FACTOR` | `0` |
| `QB_TURRET_MANUAL_CONTROL_FACTOR` | `4` |
| `QB_TURRET_PID_THRESHOLD` | `3` |
| `QB_TURRET_PID_MIN_DELTA_T` | `5` |
| `QB_TURRET_PID_MAX_DELTA_T` | `25` |
| `QB_TURRET_PID_BAD_DELTA_T` | `250` |
| `QB_NORTH_OFFSET` | `0` |
| `NUM_CHARS` | `1000` |

### UART pins
| Pin | Value |
|-----|-------|
| `RX2` | `16` |
| `TX2` | `17` |

## Class overview
The `QuarterbackTurret` class inherits from `Robot` and manages:
- turret motion and encoder feedback
- assembly tilt and cradle position
- flywheel speed control and preset stages
- controller-driven `manual`, `combine`, and `tracking` operation
- receiver targeting and handoff/load macros
- emergency stop and startup home/zero routines

## Primary public API
### Startup and safety
- `QuarterbackTurret(...)`
- `action() override`
- `zeroTurret()`
- `reset()`
- `setEnabled(bool enabled)`
- `emergencyStop()`
- `testForDisableOrStop()`
- `printDebug()`
- `testRoutine()`

### Turret, assembly, and cradle control
- `setTurretSpeed(float absoluteSpeed, bool overrideEncoderTare = false)`
- `moveTurret(int16_t heading, bool relativeToRobot = true, bool ramp = true)`
- `moveTurret(int16_t heading, float power = QB_HOME_PCT, bool relativeToRobot = true, bool ramp = true)`
- `moveTurretAndWait(int16_t heading, float power = QB_HOME_PCT, bool relativeToRobot = true, bool ramp = true)`
- `aimAssembly(AssemblyAngle angle, bool force = false)`
- `moveCradle(CradleState state, bool force = false)`

### Flywheel control
- `setFlywheelSpeed(float absoluteSpeed, bool forceUpdate = false)`
- `float setAutoFlywheelSpeed(float distance = 0)`
- `setFlywheelSpeedStage(FlywheelSpeed stage)`
- `adjustFlywheelSpeedStage(SpeedStatus speed)`

### Targeting and modes
- `switchMode(TurretMode mode)`
- `switchMode()`
- `switchTarget(TargetReceiver target)`
- `moveToTarget(int targetHeading)`
- `readTargetingInfo()`

### Strategic macros
- `loadFromCenter()`
- `handoff()`
- `combineMoveRight()`
- `combineMoveLeft()`

## Encoder ISR and status data
The class exposes the static encoder variables used by the interrupt service routine:
- `currentTurretEncoderCount`
- `turretEncoderStateB`
- `turretEncoderISR()`

This is used to track turret position and to keep the turret aligned during startup and motion changes.
