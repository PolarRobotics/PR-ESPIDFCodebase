# `Lights.h`
## Description
- This file defines the following constants:
    | Constant             | Value |
    |----------------------|-------|
    | NUM_LEDS             | 100   |
    | TIME_BETWEEN_TOGGLES | 500   |
    | LIGHTS_SWITCH_TIME   | 1000  |
    | LIGHTS_UPDATE_TIME   | 20    |
- This file also defines the following enum:
    | Enum     | States       |
    |----------|--------------|
    | LEDState | PAIRING, PAIRED, UNPAIRED, HOME, AWAY, TACKLED, DISCO, OFF |
- In addition, this file creates the Lights class, featuring relevant variable declarations and function prototypes. More information about these functions can be found in the documentation for Lights.cpp.
## Included Headers
- `FastLED.h`
- `PolarRobotics.h`