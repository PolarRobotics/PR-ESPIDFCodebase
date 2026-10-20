# `Quarterback.cpp`

## Startup Conditions

- The conveyor and flywheels will start off
- Initial elevation is set to `0`
- conveyor motor is set to `0` to prevent spinning 
- Reset linear actuators to be in the bottom position

## Button Controls

| Button | Function |
|:-------:|:--------:|
| Up | Aim up | 
| Down | Aim down |
| Square | Toggle conveyor | 
| Circle | Toggle flywheels | 
| Triangle | Increase flywheel speed | 
| Cross | Decrease flywheel speed | 

## Functions

```{note}
All functions begin with `Quarterback::`
```


| Name | Return | Parameters | Description | 
|:-----:|:------:|:----------:|:-----------:|
| `toggleFlywheels()` | `void` | N/A | Toggles the flywheels on/off and updates debounce | 
| `rampFW()` | `float` | `float requestedPower` | Controls some stuff on the ramp? | 
| `toggleConveyor()` | `void` | N/A | Toggles the conveyor belt on/off and updates debounce | 




