#include <Debouncer.h>
#include "esp_log.h"

static const char *TAG = "Debouncer";

// based on: https://arduinogetstarted.com/tutorials/arduino-button-debounce

// Input: debounce delay (milliseconds)
Debouncer::Debouncer(unsigned long delay, bool activeLow)
{
  if (activeLow)
  {
    BASE_STATE = HIGH;
    ACTIVE_STATE = LOW;
  }
  else
  {
    BASE_STATE = LOW;
    ACTIVE_STATE = HIGH;
  }

  // Initialize Variables
  this->lastLastStableState = BASE_STATE;
  this->lastStableState = BASE_STATE;
  this->lastUnstableState = BASE_STATE;
  this->lastToggleTime = 0;
  this->debounceDelay = delay;
}

// takes only input of 0 or 1, and outputs 0 or 1
// @param inputState: "current" call to debounce
uint8_t Debouncer::debounce(uint8_t inputState)
{
  // ESP_LOGV(TAG, "start: l_stab:%d, l_unst: %d, input: %d", lastStableState, lastUnstableState, inputState);

  // if the switch was toggled, update the last toggle time
  if (inputState != lastUnstableState)
  {
    lastToggleTime = millis();
    lastUnstableState = inputState;
  }

  // ESP_LOGV(TAG, " | over delay?: %d", ((millis() - lastToggleTime) > debounceDelay));

  lastLastStableState = lastStableState;

  // test if the delay has been exceeded
  if ((millis() - lastToggleTime) > debounceDelay)
  {

    // ESP_LOGV(TAG, " | stab_st changed?: %d", (lastStableState != inputState));

    // if the state has changed, update it
    if (lastStableState != inputState)
    {
      lastStableState = inputState;

      // ESP_LOGV(TAG, " | inputState == ACTIVE_STATE: %d", (inputState == ACTIVE_STATE));
    }
  }

  // ESP_LOGV(TAG, " | end: l_stab:%d, l_unst: %d, input: %d", lastStableState, lastUnstableState, inputState);

  return lastStableState;
}

uint8_t Debouncer::wasToggled()
{
  return (lastLastStableState != lastStableState);
}

uint8_t Debouncer::debounceAndToggled(uint8_t inputState)
{
  this->debounce(inputState);
  return this->wasToggled();
}

uint8_t Debouncer::wasSwitchedToState(DebouncerState state)
{
  if (this->wasToggled())
  {
    if (state == active)
    {
      return lastStableState == ACTIVE_STATE;
    }
    else
    { // state == base
      return lastStableState == BASE_STATE;
    }
  }
  return false;
}

uint8_t Debouncer::debounceAndSwitchedTo(uint8_t inputState, DebouncerState targetState)
{
  this->debounce(inputState);
  return this->wasSwitchedToState(targetState);
}

uint8_t Debouncer::debounceAndPressed(uint8_t inputState)
{
  return this->debounceAndSwitchedTo(inputState, active);
}