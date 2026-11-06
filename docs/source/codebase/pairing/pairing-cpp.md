# `pairing.cpp`
## Description
- This file provides a framework for pairing an ESP32 to a PS5 controller. The intended method for using the file's functionality is through calling the "activatePairing" function in main, and none of the functions called in activatePairing are intended to be called in any other context.
- This file's code is "blocking", meaning that it takes a discrete amount of time to run and it doesn't run asynchronously.

## Functions
| Name | Description
|------|-------------
| `addressIsController` | This function determines whether or not a given MAC address is associated with a PS5 controller, returning true if so and false otherwise. Thus, the function helps to identify PS5 controllers as such via MAC address.
| `startDiscovery` | This function initiates the "discovery" process for PS5 controllers, which is essentially the process through which the controller reach out and connect to the ESPs. The function returns true if the process is started successfully and false otherwise, indicating that the discovery process has or hasn't begun.
| `storeAddress` | This function stores a paired controller's MAC address into the ESP's persistent memory. By storing the address on the ESP32, future connections between it and the corresponding controller become more streamlined. This function can also clear the persistent memory before writing.
| `getAddress` | This function retrieves whatever controller MAC address has previously been stored in its persistent memory.
| `pairToLastController` | This function allows the ESP32 to pair to the controller whose MAC address is stored in the ESP32's persistent memory.
| `searchForNewController` | This function allows the ESP32 to search for new controllers to pair with. This effectively serves as the beginning of establishing a new ESP32-controller pairing.
| `activatePairing` | This function effectively centralizes the pairing functionalities, as it is meant to be called directly from main. In all, the function searches for controllers to pair to and then pairs with the first eligible controller found.

## Included Headers
- `map`
- `BluetoothSerial.h`
- `ps5Controller.h`
- `Preferences.h`
- `Pairing/pairing.h`
- `Robot/builtInLED.h`
- `Robot/Lights.h`