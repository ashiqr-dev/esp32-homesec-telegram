#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_private/esp_clk.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "io_handler.h"
#include "project_utils.h"
#include "telegram_client.h"
#include "wifi_handler.h"

#include <stdio.h>

#define DEBUG

static const char *TAG = "main";

void vHTTPSTask(void *pvParameters);

void app_main(void)
{
    esp_log_level_set("*", ESP_LOG_INFO);
#ifdef DEBUG
    esp_log_level_set("main", ESP_LOG_DEBUG);
    esp_log_level_set("io_handler", ESP_LOG_DEBUG);
    esp_log_level_set("wifi_handler", ESP_LOG_DEBUG);
    esp_log_level_set("telegram_client", ESP_LOG_DEBUG);
#endif

    printf("\n\n");
    ESP_LOGI(TAG, "esp32_homesec_telegram started");
    ESP_LOGD(TAG, "CPU clock at %dMHz", (esp_clk_cpu_freq() / 1000000));

    ERROR_CHECK_DELAY(io_init());
    ERROR_CHECK_DELAY(io_alarm_set(HIGH));

    wifi_init_nvs();
    wifi_init_sta();

    BaseType_t result =
        xTaskCreatePinnedToCore(vHTTPSTask, "https_task", 8 * 1024, NULL, 1, NULL, 0);
    if (result != pdPASS) {
        ESP_LOGE(TAG, "Failed to create https_task");
    }
}

void vHTTPSTask(void *pvParameters)
{
    (void)https_get();
    vTaskDelete(NULL);
}
