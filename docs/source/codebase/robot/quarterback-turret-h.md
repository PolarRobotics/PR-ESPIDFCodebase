# `QuarterbackTurret.h`
## Description
- This file defines the following enums:
    | Enum            | States                     |
    |-----------------|----------------------------|
    | TurretMode      | manual, automatic, combine |
    | TurretUnits     | degrees, counts            |
    | AssemblyAngle   | straight, angled, unknownAngle                                   |
    | CradleState     | forward, back              |
    | TargetReceiver  | receiver_1, receiver_2     |
    | CombinePosition | combineLeft, combineStraight, combineRight                  |
    | FlywheelSpeed   | slow_inwards, stopped, slow_outwards, lvl1_outwards, lvl2_outwards, lvl3_outwards, maximum                         |
- This file also defines the following constants:
    - Turret Speed Constants:
        | Constant             | Value |
        |----------------------|-------|
        | QB_TURRET_NUM_SPEEDS | 7     |
        | flywheelSpeeds       | {-0.1, 0, 0.1, 0.215, 0.31, 0.3875, 1.0}        |
    - Debounce and Delay Constants:
        | Constant                      | Value |
        |-------------------------------|-------|
        | QB_BASE_DEBOUNCE_DELAY        | 50L   |
        | QB_ASSEMBLY_TILT_DELAY        | 200L  |
        | QB_CRADLE_TRAVEL_DELAY        | 750L  |
        | QB_CIRCLE_HOLD_DELAY          | 750L  |
        | QB_TRIANGLE_HOLD_DELAY        | 200L  |
        | QB_CROSS_HOLD_DELAY           | 200L  |
        | QB_TURRET_INTERPOLATION_DELAY | 5L    |
        | QB_TURRET_THRESHOLD           | 35    |
        | QB_TURRET_STICK_SCALE_FACTOR  | 0.15  |
    - Speed Constants:
        | Constant         | Value |
        |------------------|-------|
        | QB_MIN_PWM_VALUE | 0.1   |
        | QB_HOME_PCT      | 0.125 |
        | QB_HANDOFF       | 0.3   |
        | QB_HOME_MAG      | 0.1   |
        | QB_ASM_SPEED     | 0.3   |
    - Turret Angle Calculation Constants:
        | Constant                    | Value  |
        |-----------------------------|--------|
        | QB_COUNTS_PER_ENCODER_REV   | 1250   |
        | QB_COUNTS_PER_TURRET_REV    | 10556  |
        | QB_COUNTS_PER_TURRET_DEGREE | 29.321 |
        | QB_TURRET_SLOP_COUNTS       | 540    |
    - Turret Homing Constants:
        | Constant                        | Value |
        |---------------------------------|-------|
        | QB_TURRET_STOP_LOOP_DELAY_MS    | 10    |
        | QB_TURRET_STOP_THRESHOLD_MS     | 500   |
        | QB_TURRET_HOME_STOP_FACTOR      | 0     |
        | QB_TURRET_MANUAL_CONTROL_FACTOR | 4     |
    - Turret PID Controller Constants:
        | Constant                  | Value |
        |---------------------------|-------|
        | QB_TURRET_PID_THRESHOLD   | 3     |
        | QB_TURRET_PID_MIN_DELTA_T | 5     |
        | QB_TURRET_PID_MAX_DELTA_T | 25    |
        | QB_TURRET_PID_BAD_DELTA_T | 250   |
        | QB_NORTH_OFFSET           | 0     |
        | QB_AUTO_ENABLED           | false |
    - UART Communication Pins:
        | Constant | Value | Notes           |
        |----------|-------|-----------------|
        | RX2      | 16    | Reciever Pin    |
        | TX2      | 17    | Transmitter Pin |
- In addition, this file creates the QuarterbackTurret class, featuring relevant variable declarations and function prototypes. More information about these functions can be found in the documentation for QuarterbackTurret.cpp.
## Included Headers
- `Robot/Robot.h`
- `Robot/MotorControl.h`
- `ps5Controller.h`
- `Utilities/Debouncer.h`
- `Adafruit_LIS3MDL.h`
- `HardwareSerial.h`
- `ADXL335.h`

## Enums

| Enum | Definitions|
| :--: | :---------: |
|`TurretMode`| `Manual, Automatic` |
|`TurretUnits` |`Degrees, Counts`|
|`AssemblyAngle`|`Straight, Angled`|
|`CradleState`|`Forward, Back`|
|`TargetReciever`| `Reciever_1, Reciever_2`|
|`FlywheelSpeed`|`Slow_inwards`, `Stopped`,`Slow_outwards`, `Lvl1_outwards`, `Lvl2_outwards`, `lvl3_outwards`, `maximum`|

### _Combine Specific_
| Enum | Definitions |
| :--: | :---------- |
| `CombinePosition` | `CombineLeft`, `CombineStraight`, `CombineRight` |

## Defined Variables

### Turret Setup
- `QB_TURRET_NUM_SPEEDS`
  - 7

- `FlywheelSpeeds`
  - _Accepts `QB_TURRET_SPEEDS` as the array length_
  - -0.1, 0, 0.1, 0.3, 0.5, 0.7, 1.0
  
### Debounce & Miscellaneous Delay Constants

| Name | Value | isLong? |
|:----:|:-----:|:-------:|
| `QB_BASE_DEBOUNCE_DELAY` | 50 | ✓ | 
|`QB_CRADLE_TRAVEL_DELAY` | 750 | ✓ |
| `QB_CIRCLE_HOLD_DELAY` | 750 | ✓ | 
| `QB_CROSS_HOLD_DELAY` | 200 | ✓ | 
| `QB_TRIANGLE_HOLD_DELAY` | 200 | ✓ | 
| `QB_TURRET_INTERPOLATION_DELAY` | 5 | ✓ |
| `QB_TURRET_THRESHOLD` | 35 | X |
| `QB_TURRET_STICK_SCALE_FACTOR` | 0.25 | X |

### Speed Constants
| Name | Value |
|:----:|:-----:|
| `QB_MIN_PWM_VALUE` | 0.08 |
| `QB_HOME_PCT` | 0.125 | 
| `QB_HANDOFF` | 0.3 | 
| `QB_HOME_MAG ` | 0.1 | 

### Turret Angle Calculation Constants

- `QB_COUNTS_PER_ENCODER_REV`
  - _Number of ticks per encoder revolution_
  - 1000
- `QB_COUNTS_PER_TURRET_REV`
  - _The ratio is 27:1 for the falcon to turret, and the ratio is 5:1 for the falcon to the encoder (for a 12t driving sprocket on 60t gear)_
  - _Encoder spins 5.4 times (or 27/5) for every turret revolution equates for 5400 ticks per rev_
  - 5400
- `QB_COUNTS_PER_TURRET_DEGREE`
  - _15 ticks per degree (5400 / 360)_
  - 15
- `QB_TURRET_SLOP_COUNTS`
  - _Due to the high levels of backlash between the input and output, there are 540 ticks used in the encoder to add a delay before the turret actually starts to move. This prevents problems when switching directions._
  - 540

### Turret Homing Constants
- `QB_TURRET_STOP_LOOP_DELAY_MS`
  - _*ASSUMED to be a delay (measured in milliseconds) between iterations of the turret's move loop_
  - 10
- `QB_TURRET_STOP_THRESHOLD_MS`
  - _MUST be a multiple of `QB_TURRET_STOP_LOOP_DELAY_MS`_
  - 500
- `QB_TURRET_HOME_STOP_FACTOR`
  - _A correction constant for homing,_
    - _*ASSUMED to be multiplied into the stop counts in the final homing algorithm_
  - 0.5
- `QB_TURRET_MANUAL_CONTROL_FACTOR`
  - _Higher values equate to less sensitivity during manual control (Basically dividing the clock of the loop)_
  - 4
  
### Turret PID Controller Constants
- `QB_TURRET_PID_THRESHOLD`
  - _The acceptable error in the position control (in degrees) that will zero the error constants / build up_
  - 3
- `QB_TURRET_PID_MIN_DELTA_T`
  - _*ASSUMED to be a failsafe, if the PID error values are not updated fast enough, such as if the controller hangs for half a second, the PWM values will be massive compared to what they should be_
  - 5
- `QB_TURRET_PID_MAX_DELTA_T`
  - _The maximum time between updates_
- `QB_TURRET_PID_BAD_DELTA_T`
  - _*ASSUMED to be the maximum time before the turret needs to be zeroed out_
  - 250
- `QB_NORTH_OFFSET`
  - _Deals with the problems of zeroing the turret but then holding a set angle afterwards_
  - _NOTE: May be obsolete now_
  - 0
- `QB_AUTO_ENABLED`
  - _Enables or disables Auto Mode. Used for testing_
  - False

### UART Communication Pins
| Pin | Name | Value |
| :-: | :--: | :---: |
| `RX2` | Reciever Pin | 16 |
| `TX2` | Transmitter Pin | 17 |


## Classes
### QuarterbackTurret

### Private

As is the case for most of these variables, they are *initiated* in a `.h` file, and *instanciated* in the corresponding `.cpp` file.

**MotorControl Instances**
* *Taken from the MotorControl file*
- `cradleActuator`
- `turretMotor`
- `assemblyMotor`
- `flywheelLeftMotor`
- `flywheelRightMotor`

**Pin Declartion**
| Name | Type | Value | Modifyable? |
| :--: | :--: | :--: | :--: | 
| `turretEncoderPinA` | Unsigned 8-bit int | defined in `QuarterbackTurret.cpp` | X |
| `turretEncoderPinB` | Unsigned 8-bit integer |  defined in `QuarterbackTurret.cpp` | X |
| `turretLaserPin` | Unsigned 8-bit integer | defined in `QuarterbackTurret.cpp` | ✓ | 


**Joystick Inputs**
| Name | Type | Value | Modifyable? |
| :--: | :--: | :--: | :--: | 
| `stickTurret` | float | defined in `QuarterbackTurret.cpp` | ✓ |
| `stickFlywheel` | float | defined in `QuarterbackTurret.cpp` | ✓ | 

```{note}
Note: Write the purpose found in `.h` when doccumenting `.cpp` for both of the above* 
```


**Autonomous Targeting**

| Name | Enum | Value | Modifyable? |
| :--: | :--: | :--: | :--: | 
| `mode` | `Manual`, `Automatic` | ✓ |
| `target` | `TargetReciever` | `Reciever_1`, `Reciever_2`| ✓ |
| `combinePosition` |  `CombineLeft`, `CombineStraight`, `CombineRight` | ✓ | 

```{note}
Note: These derive from the enums earlier in this doc*
```

**Setup & Status variables**

| Name | Type | Value | Modifyable? |
| :--: | :--: | :--: | :--: | 
| `enabled` | bool | True or False | ✓ |
| `initialized` | bool | True or False | ✓ |
| `runningMacro` | bool | True or False | ✓ |

```{note}
 Note: The definitions for these items will be included in the `.cpp` file for convenience sake*
 ``` 

**Assembly Movement**
* *The definitions for these items will be included in the .cpp file for convenience*
* *Again, these are enums, not variables*

```{note}
Continue to create a table for next time here. 

~ PW
``` 

| Name | Enum | Value | Modifyable? |
| :--: | :--: | :--: | :--: | 
| `currentAssemblyAngle` | `AssemblyAngle` | 

- `currentAssemblyAngle`
  - Enum: `AssemblyAngle`
  - Value: `Straight`,`Angle`
  - Modifyable: YES
- `targetAssemblyAngle`
  - Enum: `AssemblyAngle`
  - Value: `Straight`, `Angle`
  - Modifyable: YES
- `assemblyMoving`
  - Type: boolean
  - Value: True or False
  - Modifyable: YES
- `assemblyTriggerToggled`
  - Type: boolean
  - Value: True or False
  - Modifyable: YES
- `assemblyStartTime`
  - Type: unsigned 8-bit integer
  - Value: Defined in `QuarterbackTurret.cpp`
  - Modifyable: YES
  
**Ball Cradle State Variables**
* *Again, some of these are enums, the ones that have enums are denoted*
- `currentCradleState`
  - Enum: `CradleState`
  - Value: `Forward`, `Back`
  - Modifyable: YES
- `targetCradleState`
  - Enum: `CradleState`
  - Value: `Forward`, `Back`
  - Modifyable: YES
- `cradleMoving`
  - Type: boolean
  - Value: True or False
  - Modifyable: YES
- `cradleStartTime`
  - Type: unsigned 32-bit integer
  - Value: defined in QuarterbackTurret.cpp
  - Modifyable: YES

**Flywheel State Variables**
- `currentFlywheelStage`
  - Enum: `FlywheelSpeed`
  - Value: Defined in QuarterbackTurret.cpp
  - Modifyable: YES
- `targetFlywheelStage`
  - Enum: `FlywheelSpeed`
  - Value: Defined in QuarterbackTurret.cpp
  - Modifyable: YES
- `currentFlywheelSpeed`
  - Type: `float`
  - Value: Defined in QuarterbackTurret.cpp
  - Modifyble: YES
- `flywheelManualOverride`
  - Type: boolean
  - Value: True or False
  - Modifyable: YES

**Turret State Variables**
- `currentTurretSpeed`
  - Type: float
  - Value: Defined in QuarterbackTurret.cpp
  - Modifyable: YES
- `targetTurretSpeed`
  - Type: float
- `utmsCtr`
  - Type: Signed 8-bit Integer
  - Value: 0
  - Modifyable: YES
- `UTMS_CTR_MAX`
  - Type: `define`
  - Value: 15
  - Modifyable: NO

**Private Encoder State Variables**
- `targetTurretEncorderCount`
  - Type: Signed 8-bit Integer
  - Value: Defined in QuarterbackTurret.cpp
  - Modifyable: YES
- `errorEncoderCount`
  - Type: Signed 8-bit Integer
  - Value: Defined in QuarterbackTurret.cpp
  - Modifyable: YES
- `slopError`
  - Type: Signed 8-bit Integer
  - Value: Defined in QuarterbackTurret.cpp
  - Modifyable: YES
- `stopError`
  - Type: Signed 8-bit Integer
  - Value: Defined in QuarterbackTurret.cpp
  - Modifyable: YES
- `manualHeadingIncrementCount`
  - Type: Unsighed 8-bit Integer
  - Value: Defined in QuarterbackTurret.cpp
  - Modifyable: YES
- `turretMoving`
  - Type: `true` or `false`
  - Value: Boolean
  - Modifyable: YES

**Robot Relative Headings**
- `curretRelativeHeading`
  - Type: Signed 16-bit Integer
  - Value: Defined in QuarterbackTurret.cpp
  - Modifyable: YES
- `targetRelativeHeading`
  - Type: Signed 16-bit Integer
  - Value: Defined in QuarterbackTurret.cpp
  - Modifyable: YES
- `curretnRelativeTurretCount`
  - Type: Signed 32-bit Integer
  - Value: Defined in QuarterbackTurret.cpp
  - Modifyable: YES

**Absolute (World Relative) Headings**
- `currentAbsoluteHeading`
  - Type: Signed 16-bit Integer
  - Value: Defined in QuarterbackTurret.cpp
  - Modifyable: YES
- `targetAbsoluteHeading`
  - Type: Signed 16-bit Integer
  - Value: Defined in QuarterbackTurret.cpp
  - Modifyable: YES
- `turretLaserState`
  - Type: Unsigned 8-bit Integer
  - Value: Defined in QuarterbackTurret.cpp
  - Modifyable: YES

**Control Input Debouncers**
- *Note: All of these are instances of the `Debouncer` class*
- `dbShare`
- `dbOptions`
- `dbSquare`
- `dbDpadUp`
- `dbDpadDown`
- `dbDpadUp`
- `dbDpadDown`
- `dbDpadLeft`
- `dbDpadRight`

**Macro Button Debouncer**
- `dbCircle`
- `dbTriangle`
- `dbCross`
- `dbTurretInterpolator`

### Magnometer
**Setting up the Magnometer**
- `lis3mdl`
  - Description: This is the instanciation of the magnometer referenced in the code. It allows for one to access information from the `Adafruit_LIS3MDL.h` class.
  - Parent Class: `Adafruit_LIS4MDL.h`

- `useMagnetometer`
  - Description: A variable that allows for one to enable/disable the magnetometer at will. 
  - Type: `boolean`

- `holdTurretStillEnabled`
  - Description: A variable that allows one to use the magnetomer for **only** the handoff and *not* the holding code
  - Type: `boolean`

**Calibrating the Magnetometer**

- `mag_yVal`, `mag_xVal`
  - Description: The current x and y values read by the magnetometer (after correcting for errors)
  - Type: `int`
  - Default Value: 0

- `mag_xMax`, `mag_xMin`
  - Description: The maximum and minimum values for `x` recorded when calibrating the magnometer & stores in their respective variables
  - Type:`int`
  - Default Value: -1000000 & 1000000

- `mag_yMax`, `mag_yMin`
  - Description: The maximum and minimum values for `y` are recorded when calibrating the magnometer & stores in their respective variables
  - Type: `int`
  - Default Value: -1000000 & 1000000

- `mag_xHalf`, `mag_yHalf`
  - Description: Half of the the *total* range of expected the respective `x` and `y` values. Used to shift the their respective values back to the origin (0,0)
  - Type: `int`
  - Default Value: 0

  - `mag_xSign`, `mag_ySign`
    - Description: Used to determine whether to add the respective `half` value or subtract it when shifting the values back to the origin. If the value is `true`, the it subtracts, if not, it adds them.
    - Type: `boolean`
    - Default Value: `false`

- `northHeadingDegrees`
  - Description: An offset variable. Its purpose is to re-align the magnetometer due to errors in calculating the proper direction (caused by interference from the motors & other mechanical devices nearby). Right now it is only partially working
  - Type: `integer`
  - Default Value: 0

- `headingRad`
  - Description: The current calculated heading in radians using the `x` and `y` values *after* calibration
  - Type: Float
  - Default Value: N/A

- `headingDeg`
  - Description: The current calculated heading in degrees. Converts from `headingRad`
  - Type: Float
  - Default Value: N/A


**PID Variables**
- Used to correct offsets calculated by the code that need to account for reality

- `PID_ERROR_AVG_ARRAY_LENGTH`
  - Type: DEFINE
  - Description: The length of the array used for averaging the error
  - Value: 5

- `prevErrorVals`
  - Type: Integer Array
  - Description: An array of previously measured error values; used to average the collected error values and offset the random spikes in the magnetometer
  - Length: `PID_ERROR_AVG_ARRAY_LENGTH`
  - Values: {0, 0, 0, 0, 0}

- `prevErrorIndex`
  - Type: Integer
  - Description: Used to update the index accessed from the `prevErrorVals`, allows each index to be accessed over time
  - Default Value: 0

- `previousTime`
  - Type: Long
  - Description: Used to calculate errors and deltaT (in respect to the PID function)
  
- `ePrevious`
  - Type: Float
  - Description: The percent error from the magnetometer builds over time - this is used to record the previous and current error values
  - Default Value: 0

- `eIntegral`
  - Type: Float
  - Description: The current error integral calculated and added to `ePrevious` in the PID loop
  - Default Value: 0
- `kp`
  - Type: Float
  - Description: The proportional gain used in PID calculations
  - Default Value: 0.005
- `ki`
  - Type: Float
  - Description: The integral gain used in PID calculations
  - Default Value: 0.0012
- `kd`
  - Type: Float
  - Description: The derivative gain used in PID calculations
  - Default Value: 0.0
- `turretPIDSpeed`
  - Type: Float
  - Description: The calculated PWM used in the PID loop
  - Default Value: 0
- `minMagSpeed`
  - Type: Float
  - Description: Sets a minimum bound on the PWM signal to prevent errors in the PID loop. PWM signals below this range usually fail to make the motor turn and throw off the PID calculations. 
  - Defualt Value: 0.075

## Magnetometer Functions

**magnetometerSetup()**
- As defined in `QuarterbackTurret.cpp`:
  - First checks to see if the **LIS3MDL** has been identified or not.
    - If not, it sets the sensitivity, operation mode, rate of data transmission, and the error range of the magnetic field detection to a series of pre-defined values. Finally, it enables certain features of the **LIS3MDL**. Runs once on startup.
- Type: `void`

- **calibMagnetometer()**
  - As defined in `QuarterbackTurret.cpp`:
    - First rotates the turret 360 degrees and calibrates how to calculate movement given input from the **LIS3MDL**. Used to make sure that the data is processed properly and that the outputs are to the correct angles.
- Type: `void`

- **calculateHeadingMag()**
  - As defined in `QuarterbackTurret.cpp`
    - Run inside of the **`calibMagnetometer()`** function. Uses the values calculated in the beginning of **`calibMangetometer()`** to determine the current angle of the turret, corrects and verifies that the ranges of `x` and `y` are legal. It then calculates the offset and integrates it into the measurement from the magnetometer.
- Type: `void`
