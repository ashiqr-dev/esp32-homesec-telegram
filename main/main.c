#include "esp_err.h"
#include "telegram_bot.h"
#include "wifi_sta.h"

void app_main(void)
{
    ESP_ERROR_CHECK(nvs_init());
    ESP_ERROR_CHECK(wifi_init_sta());
    ESP_ERROR_CHECK(telegram_test_bot());
    telegram_send_message("Hello World!");
}
