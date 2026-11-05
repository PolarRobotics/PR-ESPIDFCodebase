#pragma once

#include <ConfigManager.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "freertos/task.h"

#define CONSOLE_MAX_CMDLINE_LEN 128
#define CONSOLE_MAX_ARGS 8

int console_write_bot_config(int argc, char **argv);
esp_err_t register_console_commands(void);
void console_task(void *pvParameters);
void startConsole(ConfigManager &config);
