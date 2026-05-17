#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "storage.h"
#include "telegram_bot.h"
#include "wifi_sta.h"
#include <stdint.h>

void telegram_bot(void *pvParameters);

static const char *TAG = "main";

void app_main(void)
{
    ESP_ERROR_CHECK(nvs_init());
    ESP_ERROR_CHECK(wifi_init_sta());
    ESP_ERROR_CHECK(telegram_test_bot());
    (void)telegram_send_message("Hello World!");

    xTaskCreate(telegram_bot, "telegram_bot", 8192, NULL, 5, NULL);
}

void telegram_bot(void *pvParameters)
{
    for (;;) {
        esp_err_t err = telegram_get_updates();
        if (err != ESP_OK) {
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        uint32_t high_water = uxTaskGetStackHighWaterMark(NULL);
        ESP_LOGI(TAG, "Stack High Water Mark: %lu bytes remaining", high_water);
    }
    vTaskDelete(NULL);
}
