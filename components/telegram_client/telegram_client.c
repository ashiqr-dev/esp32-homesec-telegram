#include "telegram_client.h"

#include "esp_crt_bundle.h"
#include "esp_err.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "esp_tls.h"

#include "sdkconfig.h"

#include <sys/param.h>

#define APP_BOT_TOKEN CONFIG_APP_TELEGRAM_BOT_TOKEN

#define MAX_HTTP_OUTPUT_BUFFER 2048
static const char *TAG = "telegram_client";

extern const char telegram_root_cert_pem[] asm("_binary_telegram_root_cert_pem_start");

static esp_err_t http_event_handler(esp_http_client_event_t *evt)
{
    static int output_len; // Stores the number of bytes already read
    switch (evt->event_id) {
    case HTTP_EVENT_ERROR:
        ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
        break;
    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
        break;
    case HTTP_EVENT_HEADER_SENT:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
        break;
    case HTTP_EVENT_ON_HEADERS_COMPLETE:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADERS_COMPLETE");
        break;
    case HTTP_EVENT_ON_DATA:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
        // Clean the buffer in case of a new request
        if (output_len == 0 && evt->user_data) {
            memset(evt->user_data, 0, MAX_HTTP_OUTPUT_BUFFER);
        }

        if (!esp_http_client_is_chunked_response(evt->client)) {
            // If user_data buffer is configured, copy the response into the buffer
            int copy_len = 0;
            if (evt->user_data) {
                // The last byte in evt->user_data is kept for the NULL character in case of
                // out-of-bound access.

                // If incoming data exceeds remaining buffer space, fill the available space
                // completely.
                // todo: test with data_len of exactly 2048 characters, could cause buffer overflow
                copy_len = MIN(evt->data_len, (MAX_HTTP_OUTPUT_BUFFER - output_len));
                if (copy_len > 0) {
                    memcpy(evt->user_data + output_len, evt->data, (size_t)copy_len);
                }
            } else {
                ESP_LOGE(TAG, "No HTTP output buffer configured!");
                return ESP_FAIL;
            }
            output_len += copy_len;
        }

        break;

    case HTTP_EVENT_ON_FINISH:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
        output_len = 0;
        break;

    case HTTP_EVENT_DISCONNECTED:
        ESP_LOGD(TAG, "HTTP_EVENT_DISCONNECTED");
        int mbedtls_err = 0;
        esp_err_t err =
            esp_tls_get_and_clear_last_error((esp_tls_error_handle_t)evt->data, &mbedtls_err, NULL);
        if (err != ESP_OK) {
            ESP_LOGI(TAG, "Last esp err code: 0x%x", err);
            ESP_LOGI(TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
        }
        output_len = 0;
        break;
    case HTTP_EVENT_REDIRECT:
        ESP_LOGD(TAG, "HTTP_EVENT_REDIRECT");
        esp_http_client_set_redirection(evt->client);
        break;
    default:
        break;
    }
    return ESP_OK;
}

void https_get(void)
{
    char local_response_buffer[MAX_HTTP_OUTPUT_BUFFER + 1U] = {0};

    esp_http_client_config_t config = {
        .url = "https://api.telegram.org/bot" APP_BOT_TOKEN "/getMe",
        .event_handler = http_event_handler,
        .user_data = local_response_buffer,
        .crt_bundle_attach = esp_crt_bundle_attach,
        .cert_pem = telegram_root_cert_pem,
        .timeout_ms = 5000,
    };
    ESP_LOGD(TAG, "HTTPS GET request with url");
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        ESP_LOGD(TAG,
                 "HTTPS status = %d, content_length = %" PRId64,
                 esp_http_client_get_status_code(client),
                 esp_http_client_get_content_length(client));
        ESP_LOGI(TAG, "%s", local_response_buffer);
    } else {
        ESP_LOGE(TAG, "Error performing HTTPS request: %s", esp_err_to_name(err));
    }
}
