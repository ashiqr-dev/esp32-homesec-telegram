#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_private/esp_clk.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "io_handler.h"
#include "project_utils.h"

#include <stdbool.h>
#include <stdio.h>

static const char *TAG = "main";

void app_main(void)
{
    esp_log_level_set("*", ESP_LOG_VERBOSE);
    printf("\n\n");
    ESP_LOGI(TAG, "esp32_homesec_telegram started");
    ESP_LOGD(TAG, "CPU clock %dMHz", (esp_clk_cpu_freq() / 1000000));

    io_init();
    ERROR_CHECK_DELAY(io_alarm_set(HIGH));
}
