#include "storage.h"
#include "nvs_flash.h"
#include <stdint.h>

#define NAMESPACE "storage"

esp_err_t nvs_init(void)
{
    esp_err_t err = nvs_flash_init();

    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    return err;
}

esp_err_t nvs_store_int32(const char *key, int32_t value)
{
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        return err;
    }

    err = nvs_set_i32(nvs_handle, key, value);
    if (err != ESP_OK) {
        goto cleanup;
    }
    err = nvs_commit(nvs_handle);
    if (err != ESP_OK) {
        goto cleanup;
    }

    nvs_close(nvs_handle);
    return ESP_OK;

cleanup:
    nvs_close(nvs_handle);
    return err;
}

esp_err_t nvs_read_int32(const char *key, int32_t *out_value)
{
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(NAMESPACE, NVS_READONLY, &nvs_handle);
    if (err != ESP_OK) {
        return err;
    }

    err = nvs_get_i32(nvs_handle, key, out_value);

    nvs_close(nvs_handle);

    return err;
}
