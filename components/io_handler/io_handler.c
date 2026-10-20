#include "io_handler.h"

#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "project_utils.h"

#define ALARM_PIN GPIO_NUM_33

static const char *TAG = "io_handler";

#define ESP_INTR_FLAG_DEFAULT 0

static QueueHandle_t gpio_evt_queue = NULL;

typedef struct {
    gpio_num_t pin;
    gpio_int_type_t mode;
    gpio_pulldown_t pull_down;
    gpio_pullup_t pull_up;
    gpio_int_type_t intr_type;
} pin_number;

static const pin_number pin_nums[] = {
    {ALARM_PIN, GPIO_MODE_OUTPUT, GPIO_PULLDOWN_DISABLE, GPIO_PULLUP_DISABLE, GPIO_INTR_DISABLE},

    {GPIO_NUM_15, GPIO_MODE_INPUT, GPIO_PULLDOWN_DISABLE, GPIO_PULLUP_ENABLE, GPIO_INTR_ANYEDGE},
    {GPIO_NUM_4, GPIO_MODE_INPUT, GPIO_PULLDOWN_DISABLE, GPIO_PULLUP_ENABLE, GPIO_INTR_ANYEDGE},
    {GPIO_NUM_16, GPIO_MODE_INPUT, GPIO_PULLDOWN_DISABLE, GPIO_PULLUP_ENABLE, GPIO_INTR_ANYEDGE},
    {GPIO_NUM_17, GPIO_MODE_INPUT, GPIO_PULLDOWN_DISABLE, GPIO_PULLUP_ENABLE, GPIO_INTR_ANYEDGE},
    {GPIO_NUM_5, GPIO_MODE_INPUT, GPIO_PULLDOWN_DISABLE, GPIO_PULLUP_ENABLE, GPIO_INTR_ANYEDGE},
    {GPIO_NUM_18, GPIO_MODE_INPUT, GPIO_PULLDOWN_DISABLE, GPIO_PULLUP_ENABLE, GPIO_INTR_ANYEDGE},
    {GPIO_NUM_19, GPIO_MODE_INPUT, GPIO_PULLDOWN_DISABLE, GPIO_PULLUP_ENABLE, GPIO_INTR_ANYEDGE},
    {GPIO_NUM_21, GPIO_MODE_INPUT, GPIO_PULLDOWN_DISABLE, GPIO_PULLUP_ENABLE, GPIO_INTR_ANYEDGE},
    {GPIO_NUM_22, GPIO_MODE_INPUT, GPIO_PULLDOWN_DISABLE, GPIO_PULLUP_ENABLE, GPIO_INTR_ANYEDGE},
    {GPIO_NUM_23, GPIO_MODE_INPUT, GPIO_PULLDOWN_DISABLE, GPIO_PULLUP_ENABLE, GPIO_INTR_ANYEDGE},
    {GPIO_NUM_25, GPIO_MODE_INPUT, GPIO_PULLDOWN_DISABLE, GPIO_PULLUP_ENABLE, GPIO_INTR_ANYEDGE},
    {GPIO_NUM_26, GPIO_MODE_INPUT, GPIO_PULLDOWN_DISABLE, GPIO_PULLUP_ENABLE, GPIO_INTR_ANYEDGE},
    {GPIO_NUM_27, GPIO_MODE_INPUT, GPIO_PULLDOWN_DISABLE, GPIO_PULLUP_ENABLE, GPIO_INTR_ANYEDGE},
    {GPIO_NUM_14, GPIO_MODE_INPUT, GPIO_PULLDOWN_DISABLE, GPIO_PULLUP_ENABLE, GPIO_INTR_ANYEDGE},
    {GPIO_NUM_39, GPIO_MODE_INPUT, GPIO_PULLDOWN_DISABLE, GPIO_PULLUP_DISABLE, GPIO_INTR_ANYEDGE},
    {GPIO_NUM_35, GPIO_MODE_INPUT, GPIO_PULLDOWN_DISABLE, GPIO_PULLUP_DISABLE, GPIO_INTR_ANYEDGE},
    {GPIO_NUM_34, GPIO_MODE_INPUT, GPIO_PULLDOWN_DISABLE, GPIO_PULLUP_DISABLE, GPIO_INTR_ANYEDGE},
};

const int PIN_AMOUNT = ARRAY_SIZE(pin_nums);

static esp_err_t io_intr_init(void);
static void io_task(void *arg);

static void IRAM_ATTR io_isr_handler(void *arg)
{
    uint32_t gpio_num = (uint32_t)arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

esp_err_t io_init(void)
{
    esp_err_t err = ESP_OK;

    for (int i = 0; i < PIN_AMOUNT; i++) {
        err = gpio_reset_pin(pin_nums[i].pin);
    }

    gpio_config_t io_config;

    for (int i = 0; i < PIN_AMOUNT; i++) {
        io_config.intr_type = pin_nums[i].intr_type;
        io_config.mode = pin_nums[i].mode;
        io_config.pin_bit_mask = (1ULL << pin_nums[i].pin);
        io_config.pull_down_en = pin_nums[i].pull_down;
        io_config.pull_up_en = pin_nums[i].pull_up;
        err |= gpio_config(&io_config);
    }

    err |= io_intr_init();
    return err;
}

static esp_err_t io_intr_init(void)
{
    gpio_install_isr_service(0);
    esp_err_t err = ESP_OK;
    for (int i = 0; i < PIN_AMOUNT; i++) {
        if (pin_nums[i].intr_type != GPIO_INTR_DISABLE) {
            err |= gpio_isr_handler_add(pin_nums[i].pin, io_isr_handler, (void *)pin_nums[i].pin);
        }
    }
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    xTaskCreate(io_task, "io_task", 2048, NULL, 10, NULL);
    return err;
}

esp_err_t io_alarm_set(io_t level)
{
    esp_err_t err = gpio_set_level(ALARM_PIN, (uint32_t)level);

    const char *level_status = level ? "HIGH" : "LOW";
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "io_alarm_set %s - FAILED with %d", level_status, err);
    } else {
        ESP_LOGD(TAG, "io_alarm_set %s - SUCCESS", level_status);
    }
    return err;
}

static void io_task(void *arg)
{
    uint32_t io_num;
    for (;;) {
        if (xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            printf("GPIO[%" PRIu32 "] intr, val: %d\n", io_num, gpio_get_level(io_num));
        }
    }
}
