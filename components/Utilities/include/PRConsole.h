#pragma once

#include <ConfigManager.h>

#include "esp_console.h"
#include "esp_vfs_dev.h"
#include "driver/uart.h"
#include "linenoise/linenoise.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#define CONSOLE_MAX_CMDLINE_LEN 128
#define CONSOLE_MAX_ARGS 8

int console_write_bot_info(int argc, char **argv);
esp_err_t register_console_commands(void);
void console_task(void *pvParameters);
void startConsole(ConfigManager &config);
