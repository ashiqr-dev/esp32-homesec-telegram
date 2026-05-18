#include "telegram_bot.h"
#include "cJSON.h"
#include "esp_crt_bundle.h"
#include "esp_err.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "storage.h"
#include "string.h"
#include <stdint.h>
#include <stdio.h>
#include <sys/param.h>

#define TELEGRAM_BOT_TOKEN CONFIG_ESP_TELEGRAM_BOT_TOKEN
#define TELEGRAM_CHAT_ID   CONFIG_ESP_TELEGRAM_CHAT_ID

#define MAX_HTTP_OUTPUT_BUFFER 512

#define BASE_URL "https://api.telegram.org/bot" TELEGRAM_BOT_TOKEN

static const char *TAG = "telegram_bot";

static int32_t offset = -1;

static esp_err_t extract_data(const char *json_string, int32_t *out_update_id, char *out_text,
                              size_t out_text_size);

static esp_err_t get_request(const char *url, char *response, int timeout_ms);
static esp_err_t post_request(const char *url, const char *body, char *response);

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

static esp_err_t telegram_event_handler(const char *message)
{
    ESP_LOGI(TAG, "Incoming text: '%s'", message);
    if (!(message[0] == '/')) {
        return ESP_FAIL;
    }
    if (strcmp(message, "/arm") == 0) {
        return telegram_send_message("/arm received!");
    }
    return ESP_OK;
}

esp_err_t telegram_register_commands(void)
{
    ESP_LOGI(TAG, "setMyCommands started");
    const char *url = BASE_URL "/setMyCommands";
    const char *body = "{\"commands\":[{\"command\":\"arm\",\"description\":\"Arm the system\"}]}";

    static char response[MAX_HTTP_OUTPUT_BUFFER] = "";

    return post_request(url, body, response);
}

esp_err_t telegram_test_bot(void)
{
    ESP_LOGI(TAG, "getMe started");
    const char *url = BASE_URL "/getMe";

    static char response[MAX_HTTP_OUTPUT_BUFFER] = "";
    ESP_ERROR_CHECK(get_request(url, response, 0));

    ESP_LOGI(TAG, "response to getMe:\n%s\n", response);

    return ESP_OK;
}

esp_err_t telegram_send_message(const char *message)
{
    ESP_LOGI(TAG, "sendMessage started");
    const char *url = BASE_URL "/sendMessage";
    char body[256];
    snprintf(body, sizeof(body), "{\"chat_id\":\"%s\",\"text\":\"%s\"}", TELEGRAM_CHAT_ID, message);

    static char response[MAX_HTTP_OUTPUT_BUFFER] = "";
    esp_err_t err = post_request(url, body, response);
    if (err != ESP_OK) {
        return err;
    }

    ESP_LOGI(TAG, "response to sendMessage:\n%s\n", response);

    return ESP_OK;
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

    static char response[MAX_HTTP_OUTPUT_BUFFER] = "";
    ESP_LOGI(TAG, "reading at: %d", offset);
    esp_err_t err = get_request(url, response, (35) * 1000);
    if (err != ESP_OK) {
        return err;
    }

    ESP_LOGI(TAG, "response to getUpdates:\n%s\n", response);

    if (strlen(response) >= MAX_HTTP_OUTPUT_BUFFER - 1) {
        ESP_LOGI(TAG, "JSON is too long");
        offset += 1;
        (void)nvs_store_int32("offset_value", offset);
        ESP_LOGI(TAG, "NVS stored offset value: %d", offset);
        return ESP_OK;
    }

    char text[256] = "";
    err = extract_data(response, &offset, text, sizeof(text));
    (void)nvs_store_int32("offset_value", offset);
    ESP_LOGI(TAG, "NVS stored offset value: %d", offset);
    if (err != ESP_FAIL) { // push forward on corrupted or valid JSON. only stay if no update id.
        offset += 1;
    }

    telegram_event_handler(text);

    return ESP_OK;
}

static esp_err_t extract_data(const char *json_string, int32_t *out_update_id, char *out_text,
                              size_t out_text_size)
{
    cJSON *root = cJSON_Parse(json_string);
    if (root == NULL) { // parsing failed
        return ESP_ERR_INVALID_RESPONSE;
    }

    cJSON *result = cJSON_GetObjectItem(root, "result");
    cJSON *item = cJSON_GetArrayItem(result, 0);
    if (item == NULL) { // array is empty
        cJSON_Delete(root);
        return ESP_FAIL;
    }

    cJSON *id = cJSON_GetObjectItem(item, "update_id");
    if (id == NULL) { // corrupted json
        cJSON_Delete(root);
        return ESP_ERR_INVALID_RESPONSE;
    }

    *out_update_id = (int32_t)id->valueint;

    cJSON *message = cJSON_GetObjectItem(item, "message");
    cJSON *text = cJSON_GetObjectItem(message, "text");
    if (text == NULL) { // corrupted json or non text message
        cJSON_Delete(root);
        return ESP_ERR_INVALID_RESPONSE;
    }

    snprintf(out_text, out_text_size, "%s", text->valuestring);

    cJSON_Delete(root);
    return ESP_OK;
}

static esp_err_t get_request(const char *url, char *response, int timeout_ms)
{
    esp_http_client_config_t config = {
        .url = url,
        .method = HTTP_METHOD_GET,
        .crt_bundle_attach = esp_crt_bundle_attach,
        .timeout_ms = timeout_ms,
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

    int status_code = esp_http_client_get_status_code(client);
    if (status_code < 200 || status_code >= 300) {
        esp_http_client_cleanup(client);
        return ESP_FAIL;
    }

    esp_http_client_cleanup(client);
    return ESP_OK;
}

static esp_err_t post_request(const char *url, const char *body, char *response)
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

    int status_code = esp_http_client_get_status_code(client);
    if (status_code < 200 || status_code >= 300) {
        esp_http_client_cleanup(client);
        return ESP_FAIL;
    }

    esp_http_client_cleanup(client);
    return ESP_OK;
}
