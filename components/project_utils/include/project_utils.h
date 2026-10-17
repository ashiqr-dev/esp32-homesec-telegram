#pragma once

#include "esp_err.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define ERROR_CHECK_DELAY(x)                                                                       \
    do {                                                                                           \
        esp_err_t err_rc_ = (x);                                                                   \
        if (unlikely(err_rc_ != ESP_OK)) {                                                         \
            vTaskDelay(pdMS_TO_TICKS(5000));                                                       \
            _esp_error_check_failed(err_rc_, __FILE__, __LINE__, __ASSERT_FUNC, #x);               \
        }                                                                                          \
    } while (0)

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))
