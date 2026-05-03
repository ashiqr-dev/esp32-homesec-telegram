#include "esp_err.h"
#include "wifi_sta.h"

void app_main(void)
{
    ESP_ERROR_CHECK(nvs_init());
    ESP_ERROR_CHECK(wifi_init_sta());
}
