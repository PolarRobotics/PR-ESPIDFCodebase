#pragma once

#ifndef CFG_MGR_H
#define CFG_MGR_H

#include <Arduino.h>
#include <Preferences.h>
#include <MotorTypes.h>
#include <BotTypes.h>
#include <string>

// #include <Utilities/BotTypes.h>
#include <PolarRobotics.h>

// #define CODE_VERSION_PREF_KEY "version"
// #define NO_VERSION_PLACEHOLDER "NO_VERSION"

class ConfigManager
{
private:
  bot_config_t *config;

  // Members of Config
  uint8_t bot_name_index;
  BotType bot_type;
  // MotorType motor_type;

  // Write Function
  bool write(bot_config_t *cfg);

protected:
  Preferences preferences;

public:
  ConfigManager();
  // ConfigManager(bool writable = false);
  ~ConfigManager();
  void read();
  String version();
  int getBotIndex();
  BotType getBotType();
  drive_param_t getDriveParams();
  String toString();
  bool setConfig(uint8_t botindex);
  bool setConfig(uint8_t botindex, BotType bottype, MotorType motortype, float gearratio, float wheelbase, float r_min, float r_max);
};

#endif // CFG_MGR_H