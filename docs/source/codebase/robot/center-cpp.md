# `Center.cpp`
## Description
## Functions
| Name | Description
|------|-------------
| `Center` | This function acts as a constructor for the Center class, setting up the arm and claw motors based on the pins passed into the function. This essentially link the two motors to their respective designated pins.
| `action` | This function moves the arm and claw in response to controller inputs, effectively establishing the control scheme for the center's unique features.
| `clawControl` | This function checks the status of the claw and updates the claw motor accordingly, allowing the claw to move as expected given its current status.
| `armControl` | This function checks the status of the arm and updates the arm motor accordingly, allowing the arm to move as expected given its current status.
## Included Headers
- `Center.h`