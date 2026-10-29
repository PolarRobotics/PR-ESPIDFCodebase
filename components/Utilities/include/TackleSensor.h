#ifndef TACKLE_SENSOR_H
#define TACKLE_SENSOR_H

#include <Arduino.h>
#include <Debouncer.h>
#include <PolarRobotics.h>
#include <ps5Controller.h>

void switchTackleSensor();
void checkOptionsTackle(Debouncer *db);

#endif  // TACKLE_SENSOR_H