# `depairingStation.cpp`
## Description
- This file is meant to be used in "depairing" controllers from ESPs if necessary. It should be downloaded onto a spare ESP that a controller could be paired to using this code. The main goal behind this is to ensure that there aren't two controllers simultaneously paired to one active robot.
- The function "onConnection" prints a message when the robot and a controller connect.
- The function "onDisconnect" prints a message when the robot and a controller disconnect.
- The setup function initializes the ESP's LEDs and starts the pairing process.
- The loop function provides various visual indications for whether or not the PS5 controller is connected. If the controller is conected, the ESP will display solid LED lights and print a message whenever a button is pressed on the controller. If the controller is disconnected, the ESP will display flashing LED lights and a message indicating the disconnect will be printed when the disconnect occurs. This functionality is meant to help with debugging the connection.
## Included Headers
- `ps5Controller.h`
- `PolarRobotics.h`
- `Pairing/pairing.h`
- `Robot/builtInLED.h`