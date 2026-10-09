#pragma once
#include "esp_err.h"

typedef enum {
    LOW,
    HIGH,
} io_t;

esp_err_t io_init(void);
esp_err_t io_alarm_set(io_t level);
