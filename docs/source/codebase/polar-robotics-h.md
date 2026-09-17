# `PolarRobotics.h`
## Description
- `PolarRobotics.h` is the corresponding header file to `main.cpp`, and is also included in most other files.
  - It contains global declarations useful for many parts of the codebase, namely motor pin definitions.

### Defined Pins
```{seealso}

Our [ESP32 pin reference sheet](https://docs.google.com/spreadsheets/d/17pdff4T_3GTAkoctwm2IMg07Znoo-iJkyDGN5CqXq3w/edit?gid=0#gid=0) provides more detailed (and probably more up-to-date) information.
```

#### Drive Motor Pins
- There are four defined motor pins.
  - For most robots, only the first two are needed, and they are the left and right motors, respectively (see "Cfg. 2W").
  - For four-wheel-drive robots, namely the mecanum center, all four pins are needed (see "Cfg. 4W").

| Motor # | Pin # | `#define` name | Cfg. 2W | Cfg. 4W     |
| ------- | ----- | -------------- | ------- | ----------- |
| 1       | 32    | `M1_PIN`       | Left    | Left Front  |
| 2       | 33    | `M2_PIN`       | Right   | Right Front |
| 3       | 26    | `M3_PIN`       | N/A     | Left Rear   |
| 4       | 27    | `M4_PIN`       | N/A     | Right Rear  |

#### "Special Bot" Pins
- These pins are reserved for robots that require additional actuators beyond the standard drive wheels.
  - These "special bots" include positions such as the kicker, center, and quarterback.


| Pin # | `#define` name |
| ----- | -------------- |
| 18    | `SPECBOT_PIN1` |
| 19    | `SPECBOT_PIN2` |
| 21    | `SPECBOT_PIN3` |
| 22    | `SPECBOT_PIN4` |

#### Encoder Pins
- These pins are reserved for motor encoders. Most of these are input only.
  - Some of these have alternate names as they appear on the PCB of the ESP32.
  - Four pins is enough to support two encoders (1, 2) each with two channels (A, B).

| Pin # | Alt. Name | `#define` name |
| ----- | --------- | -------------- |
| 35    |           | `ENC1_CHA`     |
| 34    |           | `ENC1_CHB`     |
| 36    | VP        | `ENC2_CHA`     |
| 39    | VN        | `ENC2_CHB`     |

#### Other Pins
- A variety of other pins are defined for various usages.
  - Note that the UART pins are currently only used for the Quarterback V3.

| Pin # | `#define` name | Description                                   |
| ----- | -------------- | --------------------------------------------- |
| 4     | `LED_PIN`      | LED Lights                                    |
| 13    | `TACKLE_PIN`   | Tackle Sensor                                 |
| 23    | `PAIRING_PIN`  | Pairing (if pin-pairing is enabled)           |
| 16    | `UART_RX2`     | Receiver pin for serial comm. w/ other ESP    |
| 17    | `UART_TX2`     | Transmitter pin for serial comm. w/ other ESP |

## Included Headers
- `Arduino.h`
- [`Utilities/BotTypes.h`](./utilities/bot-types-h.md)
- [`Utilities/MotorTypes.h`](./utilities/motor-types-h.md)
- [`Utilities/DriveParameters.h`](./utilities/drive-parameters-h.md)
