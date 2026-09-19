#include "apps.h"
#include "board_pins.h"
#include "phyphox_ble.h"
#include "sdkconfig.h"

#include <math.h>
#include <stdio.h>

#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_rom_sys.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "app_us";

static float read_distance_cm(void)
{
    gpio_set_level(PIN_US_TRIG, 0);
    esp_rom_delay_us(2);
    gpio_set_level(PIN_US_TRIG, 1);
    esp_rom_delay_us(10);
    gpio_set_level(PIN_US_TRIG, 0);

    int64_t start_wait = esp_timer_get_time();
    while (gpio_get_level(PIN_US_ECHO) == 0) {
        if (esp_timer_get_time() - start_wait > 30000) {
            return NAN;
        }
    }
    int64_t t0 = esp_timer_get_time();
    while (gpio_get_level(PIN_US_ECHO) == 1) {
        if (esp_timer_get_time() - t0 > 30000) {
            return NAN;
        }
    }
    int64_t t1 = esp_timer_get_time();
    float us = (float)(t1 - t0);
    return us * 0.0343f / 2.0f;
}

void app_ultrasonic_run(void)
{
    char name[40];
    snprintf(name, sizeof(name), "%s-Sonic", CONFIG_PHYPHX_DEVICE_PREFIX);

    phyphox_experiment_desc_t exp = {
        .title = "超声波测距 (IDF5)",
        .category = "创客活动",
        .description = "HC-SR04 距离。ECHO 请分压到 3.3V。",
        .view_label = "数据",
        .channel_count = 1,
        .channels =
            {
                {
                    .value_label = "距离",
                    .unit = "cm",
                    .graph_label = "距离曲线",
                    .min_y = 0.0f,
                    .max_y = 200.0f,
                },
            },
    };

    gpio_config_t trig = {
        .pin_bit_mask = 1ULL << PIN_US_TRIG,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config_t echo = {
        .pin_bit_mask = 1ULL << PIN_US_ECHO,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&trig);
    gpio_config(&echo);
    gpio_set_level(PIN_US_TRIG, 0);

    ESP_ERROR_CHECK(phyphox_ble_start(name, &exp));
    ESP_LOGI(TAG, "TRIG=%d ECHO=%d BLE=%s", PIN_US_TRIG, PIN_US_ECHO, name);

    bool led = false;
    while (true) {
        float cm = read_distance_cm();
        if (!isnan(cm)) {
            if (cm < 0.0f) {
                cm = 0.0f;
            }
            if (cm > 400.0f) {
                cm = 400.0f;
            }
            phyphox_ble_write1(cm);
            ESP_LOGI(TAG, "distance=%.1f cm", cm);
        } else {
            ESP_LOGW(TAG, "no echo");
        }
        led = !led;
        board_status_led_set(led);
        vTaskDelay(pdMS_TO_TICKS(80));
    }
}
