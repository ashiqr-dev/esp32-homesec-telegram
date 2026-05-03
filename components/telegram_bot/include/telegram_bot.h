#pragma once
#include "esp_err.h"

esp_err_t telegram_test_bot(void);
esp_err_t telegram_send_message(const char *message);
