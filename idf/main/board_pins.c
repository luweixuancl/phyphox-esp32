#include "board_pins.h"

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void board_status_led_init(void)
{
    gpio_config_t io = {
        .pin_bit_mask = 1ULL << PIN_STATUS_LED,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io);
    gpio_set_level(PIN_STATUS_LED, 1);
}

void board_status_led_set(bool on)
{
    /* Many C3 boards use active-low LED */
    gpio_set_level(PIN_STATUS_LED, on ? 0 : 1);
}

void board_status_led_blink_once(void)
{
    board_status_led_set(true);
    vTaskDelay(pdMS_TO_TICKS(80));
    board_status_led_set(false);
}
