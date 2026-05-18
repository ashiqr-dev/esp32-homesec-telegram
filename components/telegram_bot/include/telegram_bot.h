#pragma once
#include "esp_err.h"

esp_err_t telegram_register_commands(void);
esp_err_t telegram_test_bot(void);
esp_err_t telegram_send_message(const char *message);
esp_err_t telegram_get_updates(void);
