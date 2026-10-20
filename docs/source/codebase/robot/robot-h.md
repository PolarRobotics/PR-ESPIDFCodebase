# `Robot.h`
## Description
- This robot base class is an abstraction to allow polymorphic behavior for all robots.
- Essentially, declare an object of type Robot in `main.cpp`, then implement/override the `initialize()` and `action()` virtual functions in derived classes.
- This will allow main to perform runtime polymorphism, effectively simplifying the main code while still allowing individual robot types to have their own special actions.
- Each class inheriting from Robot must implement the two virtual functions.
  - It should also set its own drive to a derived class of [`Drive`](../drive/drive-h.md#drive-class) if necessary.

## Included Headers
- `Arduino.h`
- [`PolarRobotics.h`](../polar-robotics-h.md)
- [`Drive/Drive.h`](../drive/drive-h.md)