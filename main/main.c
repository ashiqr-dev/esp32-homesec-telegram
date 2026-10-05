#include <stdio.h>
#include <stdbool.h>
#include "driver/gpio.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"

static const char* TAG = "main";

#define GPIO_26 26
#define GPIO_26_MASK (1ULL << 26)

#define ALARM_PIN GPIO_26
#define ALARM_PIN_MASK GPIO_26_MASK

esp_err_t init_gpio(void);

void app_main(void)
{
    esp_log_level_set("*", ESP_LOG_VERBOSE);

    (void)init_gpio();
    ESP_LOGD(TAG, "Initialize GPIO");
    gpio_set_level(ALARM_PIN, 1);
    ESP_LOGD(TAG, "Alarm on");
}

esp_err_t init_gpio(void)
{
    gpio_config_t alarm_io_config = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (ALARM_PIN_MASK),
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE
    };
    return gpio_config(&alarm_io_config);
}