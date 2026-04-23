#include <Arduino.h>
#include <ConfigManager.h>
#include "esp_log.h"

static const char *TAG = "WriteBotInfo";

// Preferences preferences;
ConfigManager config;

bool validConfig = false;

extern "C" void main_app(void)
{
  initArduino();

  ESP_LOGI(TAG, "Writing Bot Type\n");

//* STANDARD BOT CONFIGURATION
#ifndef BOT_INDEX
#define BOT_INDEX 0 // Default to 0 if not defined
#endif
  uint8_t index = 3;

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
    ESP_LOGI(TAG, "Config write successful");
  }
  else
  {
    ESP_LOGI(TAG, "Error writing bot config");
  }

  //* Read back for verification
  ESP_LOGI(TAG, "Readback:");
  config.read();                                  // read the configuration from eeprom
  ESP_LOGI(TAG, "%s", config.toString().c_str()); // print the configuration to the serial monitor

  ESP_LOGI(TAG, "Done");

  // Keep the app alive rather than returning from app_main (which would reboot)
  while (true)
  {
    delay(1000);
  }
}
