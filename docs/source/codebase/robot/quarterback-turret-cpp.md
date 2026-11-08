# `QuarterbackTurret.cpp`
## Description
- This source file implements the quarterback turret subsystem declared in `QuarterbackTurret.h`.
- The current branch uses the mode names `manual`, `combine`, and `tracking` instead of the older `automatic` naming.
- The implementation includes encoder ISR logic, flywheel presets, receiver tracking, handoff logic, and the `reset`/`zeroTurret` startup flow.

## Key implementation areas
| Area | Notes |
|------|-------|
| Encoder ISR | Reads the turret encoder and updates `currentTurretEncoderCount` |
| Startup | Home/zero routine plus default targeting state |
| Controlled modes | Manual, combine, and tracking motion control |
| Assembly and cradle | Tilt and ball cradle movement routines |
| Flywheel control | Manual, preset, and auto-distance flywheel speed logic |
| Targeting | Receiver selection, heading math, and receiver tracking updates |
| Macros | `loadFromCenter()`, `handoff()`, `combineMoveLeft()`, `combineMoveRight()` |

## Primary functions
| Name | Description |
|------|-------------|
| `turretEncoderISR` | Updates the turret encoder count from the A/B channels |
| `QuarterbackTurret` | Constructor; initializes motor, encoder, debouncer, and target state |
| `action` | Main PS5-driven control loop for manual/combine/tracking operation |
| `setTurretSpeed` | Sets open-loop turret speed |
| `moveTurret` | Moves the turret to a target heading |
| `moveTurretAndWait` | Synchronous turret move helper |
| `updateTurretMotionStatus` | Updates internal turret motion state |
| `getCurrentHeading` | Reads the robot-relative turret heading |
| `NormalizeAngle` | Wraps heading values into the expected range |
| `CalculateRotation` | Computes shortest signed rotation delta |
| `findNearestHeading` | Finds the nearest valid heading target |
| `aimAssembly` | Moves the assembly between straight and angled states |
| `moveAssemblySubroutine` | Helper for assembly motion execution |
| `moveCradleSubroutine` | Helper for cradle motion execution |
| `moveCradle` | Moves the cradle between forward and back states |
| `setFlywheelSpeed` | Sets a flywheel speed from a normalized value |
| `setFlywheelSpeedStage` | Applies a named flywheel stage |
| `adjustFlywheelSpeedStage` | Moves to the next or previous preset speed |
| `switchMode` | Switches between `manual`, `combine`, and `tracking` |
| `switchTarget` | Switches receiver target |
| `readTargetingInfo` | Reads incoming targeting data from the receiver UART stream |
| `moveToTarget` | Moves turret to targeted receiver heading |
| `loadFromCenter` | Prepares the QB to receive the ball from center |
| `handoff` | Performs the QB handoff macro |
| `combineMoveRight` | Moves to right combine angle |
| `combineMoveLeft` | Moves to left combine angle |
| `zeroTurret` | Homes/zeros the turret |
| `reset` | Resets the turret and flywheels to the safe startup state |
| `testForDisableOrStop` | Safety check for e-stop/disabled state |
| `setEnabled` | Enables or disables turret motion |
| `emergencyStop` | Forces the turret and flywheels to stop |
| `printDebug` | Prints the current turret/flywheel status for debugging |
| `turretPIDController` | Position-control loop used for target tracking |
| `updateReadMotorValues` | Reads current motor values for monitoring |

## Included Headers
- `QuarterbackTurret.h`
