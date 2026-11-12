# `builtInLED.cpp`
## Description
- Provides a framework for controlling the LED built into the ESP32 board.
- Globally scoped to avoid multiple drivers of the LED.
  - Basically functions like a mutex.

## Functions
| Name | Description
|------|-------------
| `builtInLedOn` | This function tests and reports whether or not the built-in LED is on, returning true if it is on or false if it is off.
| `toggleBuiltInLED` | This function toggles the state of the built-in LED, turning in on if it is off or off if it is on. Thus it serves as the main method for switching the light on and off.
| `setBuiltInLED` | This function sets the state of the built-in LED, setting it on or off depending on the boolean value passed in. This serves as a more direct method of controlling the LED than simply toggling.

## Included Headers
- `Arduino.h`
- [`Robot/builtInLED.h`](./built-in-led-h.md)