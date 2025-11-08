#include <Arduino.h>

// #include <PolarRobotics.h>
#include "ConfigManager.h"

// Preferences preferences;
ConfigManager config;

bool validConfig = false;

extern "C" void app_main()
{
  initArduino();

  Serial.begin(115200);
  Serial.println(F("Writing Bot Type\n"));

//* STANDARD BOT CONFIGURATION
#ifndef BOT_INDEX
#define BOT_INDEX 0 // Default to 0 if not defined
#endif
  uint8_t index = BOT_INDEX;

  //* CUSTOM BOT CONFIGURATION
  BotType bot_type = lineman;
  MotorType motor_type = small_12v;
  float gear_ratio = 1;
  float wheel_base = 10;
  float r_min = 9.00f;
  float r_max = 36.00f;

  constexpr bool useCustomConfig = false;

  if (useCustomConfig)
  {
    //* Write custom bot configuration
    validConfig = config.setConfig(index, bot_type, motor_type, gear_ratio, wheel_base, r_min, r_max);
  }
  else
  {
    //* Write standard bot configuration from BotTypes.botConfigArray
    validConfig = config.setConfig(index);
  }

  if (validConfig)
  {
    Serial.println(F("Config write successful"));
  }
  else
  {
    Serial.println(F("Error writing bot config"));
  }

  //* Read back for verification
  Serial.println(F("Readback:"));
  config.read();                      // read the configuration from eeprom
  Serial.print(F(config.toString())); // print the configuration to the serial monitor

  Serial.println(F("Done"));

  // Keep the app alive rather than returning from app_main (which would reboot)
  while (true)
  {
    delay(1000);
  }
}
