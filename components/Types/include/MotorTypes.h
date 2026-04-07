#pragma once

#ifndef MOTOR_TYPES_H
#define MOTOR_TYPES_H

#include <Pair.h>

#define NUM_MOTOR_TYPES 6

// Motor types can be found here:
// https://docs.google.com/spreadsheets/d/1DswoEAcry9L9t_4ouKL3mXFgDMey4KkjEPFXULQxMEQ/edit#gid=141541655

typedef enum
{
    serial,
    pwm
} MotorInterfaceType;

typedef enum
{
    big_ampflow,     // index 0 for 24v AmpFlow motor E30-400-24
    small_ampflow,   // index 1 for 24v AmpFlow motor E30-150-24
    pancake_ampflow, // index 2 for 24v AmpFlow motor P40-350-24
    falcon,          // index 3 for the falcon motors on the runningback
    neo_vortex,      // index 4: NEO Vortex motors
    small_12v        // index 5 for the small 12v motors on the old robots
} MotorType;

const int MOTOR_MAX_RPM_ARR[NUM_MOTOR_TYPES] = {
    5700, // 24v AmpFlow motor E30-400-24
    5600, // 24v AmpFlow motor E30-150-24
    3500, // 24v AmpFlow motor P40-350-24
    6380, // the falcon motors on the runningback
    6380, // NEO Vortex motors (using same as falcon for now)
    4000  // !TEMP, NEED TO CONFIRM NUMBER the small 12v motors on the old robots
};

const MotorInterfaceType MOTORTYPE_INTERFACE_ARRAY[NUM_MOTOR_TYPES] = {
    serial, // index 0: Big Ampflow Motor
    serial, // index 1: Small Ampflow Motor
    serial, // index 2: Pancake Ampflow Motor
    pwm,    // index 3: Falcon500 motors
    pwm,    // index 4: NEO Vortex motors
    serial  // index 5: Small 12v motors (old robots)
};

// PWM POWERS
// constexpr int MOTORTYPE_BNS_ARRAY[NUM_MOTOR_TYPES][3] = {
//     // Boost   Normal  Slow
//     {int(0.70f * SABERTOOTH_MAX_POWER), int(0.60f * SABERTOOTH_MAX_POWER), int(0.30f * SABERTOOTH_MAX_POWER)}, // index 0: Big Ampflow Motor
//     {int(0.85f * SABERTOOTH_MAX_POWER), int(0.70f * SABERTOOTH_MAX_POWER), int(0.40f * SABERTOOTH_MAX_POWER)}, // index 1: Small Ampflow Motor
//     {int(0.70f * SABERTOOTH_MAX_POWER), int(0.60f * SABERTOOTH_MAX_POWER), int(0.30f * SABERTOOTH_MAX_POWER)}, // index 2: Pancake Ampflow Motor
//     {int(0.60f * SABERTOOTH_MAX_POWER), int(0.40f * SABERTOOTH_MAX_POWER), int(0.15f * SABERTOOTH_MAX_POWER)}, // index 3: Falcon500 motors
//     {int(0.60f * SABERTOOTH_MAX_POWER), int(0.40f * SABERTOOTH_MAX_POWER), int(0.15f * SABERTOOTH_MAX_POWER)}, // index 4: NEO Vortex motors
//     {int(0.15f * SABERTOOTH_MAX_POWER), int(0.10f * SABERTOOTH_MAX_POWER), int(0.05f * SABERTOOTH_MAX_POWER)}  // index 5: Small 12v motors (old robots)
// };

// SERIAL POWERS
constexpr float MOTORTYPE_BNS_ARRAY[NUM_MOTOR_TYPES][3] = {
    // Boost   Normal  Slow
    {0.70f, 0.60f, 0.30f}, // index 0: Big Ampflow Motor
    {0.85f, 0.70f, 0.40f}, // index 1: Small Ampflow Motor
    {0.70f, 0.60f, 0.30f}, // index 2: Pancake Ampflow Motor
    {0.60f, 0.40f, 0.15f}, // index 3: Falcon500 motors
    {0.60f, 0.40f, 0.15f}, // index 4: NEO Vortex motors
    {0.15f, 0.10f, 0.05f}  // index 5: Small 12v motors (old robots)
};

const char *getMotorTypeString(MotorType type);

#endif // MOTOR_TYPES_H