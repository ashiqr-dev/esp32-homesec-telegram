#include "io_handler.h"

#include "driver/gpio.h"
#include "esp_err.h"

#define GPIO_26      26
#define GPIO_26_MASK (1ULL << 26)

#define ALARM_PIN      GPIO_26
#define ALARM_PIN_MASK GPIO_26_MASK


esp_err_t io_init(void)
{
    gpio_config_t alarm_io_config = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = ALARM_PIN_MASK,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
    };
    return gpio_config(&alarm_io_config);
}

esp_err_t io_alarm_set(io_t level)
{
    esp_err_t err = gpio_set_level(ALARM_PIN, (uint32_t)level);
    return err;
}
