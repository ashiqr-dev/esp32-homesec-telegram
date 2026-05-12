#include "telegram_bot.h"
#include "cJSON.h"
#include "esp_crt_bundle.h"
#include "esp_err.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "nvs.h"
#include "storage.h"
#include <stdint.h>
#include <stdio.h>
#include <sys/param.h>

#define TELEGRAM_BOT_TOKEN CONFIG_ESP_TELEGRAM_BOT_TOKEN
#define TELEGRAM_CHAT_ID   CONFIG_ESP_TELEGRAM_CHAT_ID

#define MAX_HTTP_OUTPUT_BUFFER 2048

#define BASE_URL "https://api.telegram.org/bot" TELEGRAM_BOT_TOKEN

static const char *TAG = "telegram_bot";

static int32_t offset = -1;

esp_err_t extract_update_id(const char *json_string, int32_t *out_value);

static esp_err_t get_updates_request(const char *url, int *status_code, char *response);
static esp_err_t get_request(const char *url, int *status_code, char *response);
static esp_err_t post_request(const char *url, const char *body, int *status_code, char *response);

static esp_err_t http_event_handler(esp_http_client_event_t *evt)
{
    static int bytes_written = 0;

    switch (evt->event_id) {
    case HTTP_EVENT_ON_DATA:
        // clean buffer
        if (bytes_written == 0) {
            memset(evt->user_data, 0, MAX_HTTP_OUTPUT_BUFFER);
        }

        int bytes_to_copy = MIN(evt->data_len, (MAX_HTTP_OUTPUT_BUFFER - bytes_written) - 1);
        memcpy((char *)evt->user_data + bytes_written, evt->data, bytes_to_copy);
        bytes_written += bytes_to_copy;
        break;
    case HTTP_EVENT_DISCONNECTED:
        bytes_written = 0;
        break;
    default:
        break;
    }
    return ESP_OK;
}

esp_err_t telegram_test_bot(void)
{
    ESP_LOGI(TAG, "getMe started");
    const char *url = BASE_URL "/getMe";

    int status_code = 0;
    char response[MAX_HTTP_OUTPUT_BUFFER];
    ESP_ERROR_CHECK(get_request(url, &status_code, response));

    ESP_LOGI(TAG, "response to getMe:\n%s\n", response);

    return (status_code >= 200 && status_code < 300) ? ESP_OK : ESP_FAIL;
}

esp_err_t telegram_send_message(const char *message)
{
    ESP_LOGI(TAG, "sendMessage started");
    const char *url = BASE_URL "/sendMessage";
    char body[256];
    snprintf(body, sizeof(body), "{\"chat_id\":\"%s\",\"text\":\"%s\"}", TELEGRAM_CHAT_ID, message);

    int status_code = 0;
    char response[MAX_HTTP_OUTPUT_BUFFER];
    post_request(url, body, &status_code, response);

    ESP_LOGI(TAG, "response to sendMessage:\n%s\n", response);

    return (status_code >= 200 && status_code < 300) ? ESP_OK : ESP_FAIL;
}

esp_err_t telegram_get_updates(void)
{
    ESP_LOGI(TAG, "getUpdates started");

    if (offset == -1) {
        (void)nvs_read_int32("offset_value", &offset);
        ESP_LOGI(TAG, "NVS read offset value: %d", offset);
        offset += 1;
    }

    char url[128];
    snprintf(url,
             sizeof(url),
             BASE_URL "/getUpdates?offset=%" PRId32 "&limit=1&timeout=30",
             offset);

    int status_code = 0;
    char response[MAX_HTTP_OUTPUT_BUFFER];
    get_updates_request(url, &status_code, response);

    ESP_LOGI(TAG, "response to getUpdates:\n%s\n", response);

    (void)extract_update_id(response, &offset);
    nvs_store_int32("offset_value", offset);
    ESP_LOGI(TAG, "NVS stored offset value: %d", offset);
    offset += 1;

    return (status_code >= 200 && status_code < 300) ? ESP_OK : ESP_FAIL;
}

esp_err_t extract_update_id(const char *json_string, int32_t *out_value)
{
    cJSON *root = cJSON_Parse(json_string);
    if (root == NULL) {
        return ESP_FAIL;
    }

    cJSON *result = cJSON_GetObjectItem(root, "result");
    cJSON *item = cJSON_GetArrayItem(result, 0);
    cJSON *id = cJSON_GetObjectItem(item, "update_id");

    if (id == NULL) {
        cJSON_Delete(root);
        return ESP_FAIL;
    }

    *out_value = (int32_t)id->valueint;
    cJSON_Delete(root);
    return ESP_OK;
}

static esp_err_t get_updates_request(const char *url, int *status_code, char *response)
{
    esp_http_client_config_t config = {
        .url = url,
        .method = HTTP_METHOD_GET,
        .crt_bundle_attach = esp_crt_bundle_attach,
        .timeout_ms = 35000,
        .user_data = response,
        .event_handler = http_event_handler,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (client == NULL) {
        return ESP_FAIL;
    }

    esp_err_t err = esp_http_client_perform(client);
    if (err != ESP_OK) {
        esp_http_client_cleanup(client);
        return ESP_FAIL;
    }

    *status_code = esp_http_client_get_status_code(client);

    esp_http_client_cleanup(client);
    return ESP_OK;
}

static esp_err_t get_request(const char *url, int *status_code, char *response)
{
    esp_http_client_config_t config = {
        .url = url,
        .method = HTTP_METHOD_GET,
        .crt_bundle_attach = esp_crt_bundle_attach,
        .user_data = response,
        .event_handler = http_event_handler,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (client == NULL) {
        return ESP_FAIL;
    }

    esp_err_t err = esp_http_client_perform(client);
    if (err != ESP_OK) {
        esp_http_client_cleanup(client);
        return ESP_FAIL;
    }

    *status_code = esp_http_client_get_status_code(client);

    esp_http_client_cleanup(client);
    return ESP_OK;
}

static esp_err_t post_request(const char *url, const char *body, int *status_code, char *response)
{
    esp_http_client_config_t config = {
        .url = url,
        .method = HTTP_METHOD_POST,
        .crt_bundle_attach = esp_crt_bundle_attach,
        .user_data = response,
        .event_handler = http_event_handler,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (client == NULL) {
        return ESP_FAIL;
    }

    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client, body, strlen(body));

    esp_err_t err = esp_http_client_perform(client);
    if (err != ESP_OK) {
        esp_http_client_cleanup(client);
        return ESP_FAIL;
    }

    *status_code = esp_http_client_get_status_code(client);

    esp_http_client_cleanup(client);
    return ESP_OK;
}
