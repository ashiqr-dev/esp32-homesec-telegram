#include "driver/gpio.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"

#include "io_handler.h"

#include <stdbool.h>
#include <stdio.h>

void app_main(void)
{
    esp_log_level_set("*", ESP_LOG_VERBOSE);

    ESP_ERROR_CHECK(io_init());
    ESP_ERROR_CHECK(io_alarm_set(HIGH));
}
