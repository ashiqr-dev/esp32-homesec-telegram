#include "wifi_handler.h"
#include "project_utils.h"

#include "esp_log.h"
#include "esp_wifi.h"
#include "nvs_flash.h"

#include "sdkconfig.h"

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#define APP_WIFI_SSID          CONFIG_APP_WIFI_SSID
#define APP_WIFI_PASS          CONFIG_APP_WIFI_PASS
#define APP_WIFI_MAXIMUM_RETRY 5

static EventGroupHandle_t wifi_event_group;

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

static const char *TAG = "wifi_handler";

static int retry_num = 0;

static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id,
                          void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGW(TAG, "Failed to connect to the AP (%d/5)", retry_num + 1);
        if (retry_num < APP_WIFI_MAXIMUM_RETRY - 1) {
            esp_wifi_connect();
            retry_num++;
            ESP_LOGI(TAG, "Retrying to connect to the AP...");
        } else {
            xEventGroupSetBits(wifi_event_group, WIFI_FAIL_BIT);
        }
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGD(TAG, "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));
        retry_num = 0;
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

void wifi_init_sta(void)
{
    wifi_event_group = xEventGroupCreate();

    ERROR_CHECK_DELAY(esp_netif_init());

    ERROR_CHECK_DELAY(esp_event_loop_create_default());
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();

    const char *hostname = "esp32-homesec-telegram";
    esp_err_t err = esp_netif_set_hostname(sta_netif, hostname);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set hostname '%s': %s", hostname, esp_err_to_name(err));
    } else {
        ESP_LOGI(TAG, "Hostname successfully set to '%s'", hostname);
    }

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ERROR_CHECK_DELAY(esp_wifi_init(&cfg));
    ERROR_CHECK_DELAY(esp_wifi_set_storage(WIFI_STORAGE_RAM));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ERROR_CHECK_DELAY(esp_event_handler_instance_register(WIFI_EVENT,
                                                          ESP_EVENT_ANY_ID,
                                                          &event_handler,
                                                          NULL,
                                                          &instance_any_id));

    ERROR_CHECK_DELAY(esp_event_handler_instance_register(IP_EVENT,
                                                          IP_EVENT_STA_GOT_IP,
                                                          &event_handler,
                                                          NULL,
                                                          &instance_got_ip));

    wifi_config_t wifi_config = {
        .sta =
            {
                .ssid = APP_WIFI_SSID,
                .password = APP_WIFI_PASS,

                .threshold.authmode = WIFI_AUTH_WPA2_PSK,
                .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
                .sae_h2e_identifier = "",

#ifdef CONFIG_ESP_WIFI_WPA3_COMPATIBLE_SUPPORT
                .disable_wpa3_compatible_mode = 0,
#endif
            },
    };
    ERROR_CHECK_DELAY(esp_wifi_set_mode(WIFI_MODE_STA));
    ERROR_CHECK_DELAY(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ERROR_CHECK_DELAY(esp_wifi_start());

    ESP_LOGD(TAG, "wifi_init_sta finished");

    EventBits_t bits = xEventGroupWaitBits(wifi_event_group,
                                           WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                           pdFALSE,
                                           pdFALSE,
                                           portMAX_DELAY);

    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "Connected to AP. SSID:'%s'", APP_WIFI_SSID);
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGE(TAG, "Failed to connect to AP. SSID:'%s'", APP_WIFI_SSID);
    } else {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
    }
}

void wifi_init_nvs(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ERROR_CHECK_DELAY(ret);
}
