#pragma once
#include <CAN.h>
#include <Robot.h>
#include <inverseKin.h>

#include <cstdint>

Swerve::Swerve(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4);

void Swerve::action();