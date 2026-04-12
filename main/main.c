#include "storage.h"
#include "wifi_sta.h"

#include <stdio.h>

void app_main(void)
{
    ESP_ERROR_CHECK(storage_init());
    wifi_init_sta();
}
