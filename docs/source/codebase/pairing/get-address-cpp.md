# `getAddress.cpp`
## Description
- This file gets the MAC address of a connected ESP by running the two functions described below. If it is not possible to access a MAC Address, this code can also provide feedback on where the process may have been held up.
- InitBluetooth and printDeviceAddress are both run in the main setup, and the main loop is empty.
## Functions
| Name | Description
|------|-------------
| `initBluetooth` | This function returns a boolean indicating whether or not the controller has established a bluetooth connection with an ESP. First, it checks if the controller is initialized. Next, it checks if the ESP is initialized. Finally, it checks if the ESP is enabled. If any of those three checks fail, a message describing the failure is printed. Thus, the function essentially provides alerts if the controller fails to connect to the ESP.
| `printDeviceAddress` | This function prints the MAC address of the connected ESP. An example of this function's output is as follows: `FOUND E0:5A:1B:77:20:26`
## Included Headers
- `Arduino.h`
- `esp_bt_main.h`
- `esp_bt_device.h`