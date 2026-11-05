#pragma once

#ifndef MOTOR_TYPES_H
#define MOTOR_TYPES_H

#include <Pair.h>

#define NUM_MOTOR_TYPES 7

// Motor types can be found here:
// https://docs.google.com/spreadsheets/d/1DswoEAcry9L9t_4ouKL3mXFgDMey4KkjEPFXULQxMEQ/edit#gid=141541655

typedef enum
{
  serial,
  pwm
} MotorInterfaceType;

typedef enum
{
  big_ampflow,      // 0: for 24v AmpFlow motor E30-400-24 (serial)
  small_ampflow,    // 1: for 24v AmpFlow motor E30-150-24 (serial)
  pancake_ampflow,  // 2: for 24v AmpFlow motor P40-350-24 (serial)
  falcon,           // 3: for the falcon motors on the runningback (pwm)
  neo_vortex,       // 4: NEO Vortex motors (pwm)
  small_12v,        // 5: small 12v motors on the old robots (serial)
  big_ampflow_pwm,  // 6: 24v AmpFlow motor E30-400-24 with PWM
                    // interface (strength linemen)
} MotorType;

const int MOTOR_MAX_RPM_ARR[NUM_MOTOR_TYPES] = {
    5700,  // 24v AmpFlow motor E30-400-24 (serial)
    5600,  // 24v AmpFlow motor E30-150-24 (serial)
    3500,  // 24v AmpFlow motor P40-350-24 (serial)
    6380,  // the falcon motors on the runningback
    6380,  // NEO Vortex motors (using same as falcon for now)
    4000,  // !TEMP, NEED TO CONFIRM NUMBER the small 12v motors on the old
           // robots
    5700   // 24v AmpFlow motor E30-400-24 with PWM interface (strength linemen)
};

const MotorInterfaceType MOTORTYPE_INTERFACE_ARRAY[NUM_MOTOR_TYPES] = {
    serial,  // 0: Big Ampflow Motor (serial)
    serial,  // 1: Small Ampflow Motor
    serial,  // 2: Pancake Ampflow Motor
    pwm,     // 3: Falcon500 motors
    pwm,     // 4: NEO Vortex motors
    serial,  // 5: Small 12v motors (old robots)
    pwm      // 6: Big Ampflow Motor with PWM interface (strength linemen)
};

// SERIAL POWERS
constexpr float MOTORTYPE_BNS_ARRAY[NUM_MOTOR_TYPES][3] = {
    // Boost   Normal  Slow
    {0.70F, 0.60F, 0.30F},  // 0: Big Ampflow Motor (serial)
    {0.85F, 0.70F, 0.40F},  // 1: Small Ampflow Motor
    {0.70F, 0.60F, 0.30F},  // 2: Pancake Ampflow Motor
    {0.60F, 0.40F, 0.15F},  // 3: Falcon500 motors
    {0.60F, 0.40F, 0.15F},  // 4: NEO Vortex motors
    {0.15F, 0.10F, 0.05F},  // 5: Small 12v motors (old robots)
    {0.70F, 0.60F, 0.30F}   // 6: Big Ampflow Motor with PWM
                            // interface (strength linemen)
};

const char *getMotorTypeString(MotorType type);

#endif  // MOTOR_TYPES_H