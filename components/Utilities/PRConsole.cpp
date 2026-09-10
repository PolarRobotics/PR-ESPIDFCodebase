#include <PRConsole.h>

static const char *TAG = "Console";
static ConfigManager *sConfig = nullptr;

int console_write_bot_config(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: write <index>\\n");
        return 1;
    }

    char *end = nullptr;
    long parsedIndex = strtol(argv[1], &end, 10);
    if (end == argv[1] || *end != '\0' || parsedIndex < 0 || parsedIndex > 255)
    {
        printf("Invalid index '%s'. Expected an integer in range [0, 255].\\n", argv[1]);
        return 1;
    }

    const uint8_t index = static_cast<uint8_t>(parsedIndex);
    const bool validConfig = sConfig->setConfig(index);

    if (!validConfig)
    {
        printf("Failed to write bot config for index %u.\\n", index);
        return 1;
    }

    sConfig->read();
    printf("Bot config index %u written successfully.\\n", index);
    printf("Readback: %s\\n", sConfig->toString().c_str());
    printf("Restart the ESP32 for new config to apply to robot runtime.\\n");
    return 0;
}

esp_err_t register_console_commands(void)
{
    esp_console_cmd_t writeCmd = {};
    writeCmd.command = "write";
    writeCmd.help = "Write bot configuration by index from BotTypes table: write <index>";
    writeCmd.hint = nullptr;
    writeCmd.func = &console_write_bot_config;
    writeCmd.argtable = nullptr;

    return esp_console_cmd_register(&writeCmd);
}

void console_task(void *pvParameters)
{
    (void)pvParameters;

    // Route UART0 through VFS so esp_console + linenoise can read/write over serial monitor.
    setvbuf(stdin, nullptr, _IONBF, 0);
    setvbuf(stdout, nullptr, _IONBF, 0);
    esp_vfs_dev_uart_use_driver(CONFIG_ESP_CONSOLE_UART_NUM);
    esp_vfs_dev_uart_port_set_rx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM, ESP_LINE_ENDINGS_CR);
    esp_vfs_dev_uart_port_set_tx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM, ESP_LINE_ENDINGS_CRLF);

    esp_console_config_t consoleConfig = {};
    consoleConfig.max_cmdline_args = CONSOLE_MAX_ARGS;
    consoleConfig.max_cmdline_length = CONSOLE_MAX_CMDLINE_LEN;
    consoleConfig.hint_color = 0;

    const esp_err_t initErr = esp_console_init(&consoleConfig);
    if (initErr != ESP_OK)
    {
        ESP_LOGE(TAG, "esp_console_init failed: %s", esp_err_to_name(initErr));
        vTaskDelete(nullptr);
        return;
    }

    esp_console_register_help_command();
    const esp_err_t registerErr = register_console_commands();
    if (registerErr != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to register console commands: %s", esp_err_to_name(registerErr));
        vTaskDelete(nullptr);
        return;
    }

    linenoiseSetMultiLine(true);
    linenoiseAllowEmpty(false);

    ESP_LOGI(TAG, "Console ready on UART%d. Run 'help' or 'write <index>'.", CONFIG_ESP_CONSOLE_UART_NUM);
    while (true)
    {
        char *line = linenoise("robot> ");
        if (line == nullptr)
        {
            vTaskDelay(pdMS_TO_TICKS(20));
            continue;
        }

        if (strlen(line) > 0)
        {
            linenoiseHistoryAdd(line);
            int ret = 0;
            esp_err_t cmdErr = esp_console_run(line, &ret);
            if (cmdErr == ESP_ERR_NOT_FOUND)
            {
                printf("Unrecognized command. Type 'help'.\\n");
            }
            else if (cmdErr == ESP_ERR_INVALID_ARG)
            {
                printf("Invalid command arguments. Type 'help'.\\n");
            }
            else if (cmdErr == ESP_OK && ret != ESP_OK)
            {
                printf("Command returned error code 0x%x\\n", ret);
            }
            else if (cmdErr != ESP_OK)
            {
                printf("Command execution failed: %s\\n", esp_err_to_name(cmdErr));
            }
        }

        linenoiseFree(line);
    }
}

void startConsole(ConfigManager &config)
{
    sConfig = &config;

    const esp_err_t uartInstallErr = uart_driver_install(static_cast<uart_port_t>(CONFIG_ESP_CONSOLE_UART_NUM), 256, 0, 0, nullptr, 0);
    if (uartInstallErr != ESP_OK)
    {
        ESP_LOGE(TAG, "uart_driver_install failed: %s", esp_err_to_name(uartInstallErr));
        return;
    }

    BaseType_t taskCreated = xTaskCreate(console_task, "console_task", 4096, nullptr, 2, nullptr);
    if (taskCreated != pdPASS)
    {
        ESP_LOGE(TAG, "Failed to create console task");
    }
}
