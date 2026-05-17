#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "storage.h"
#include "telegram_bot.h"
#include "wifi_sta.h"

void telegram_bot(void *pvParameters);

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
    }
    vTaskDelete(NULL);
}
