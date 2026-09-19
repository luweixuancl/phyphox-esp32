#include "apps.h"
#include "board_pins.h"
#include "phyphox_ble.h"
#include "sdkconfig.h"

#include <math.h>
#include <stdio.h>

#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "app_hello";

void app_hello_run(void)
{
    char name[40];
    snprintf(name, sizeof(name), "%s-Hello", CONFIG_PHYPHX_DEVICE_PREFIX);

    phyphox_experiment_desc_t exp = {
        .title = "创客入门 (IDF5)",
        .category = "创客活动",
        .description =
            "ESP32-C3 ESP-IDF 5 + NimBLE 演示。无需外接传感器。",
        .view_label = "数据",
        .channel_count = 1,
        .channels =
            {
                {
                    .value_label = "演示值",
                    .unit = "",
                    .graph_label = "演示曲线",
                    .min_y = 0.0f,
                    .max_y = 100.0f,
                },
            },
    };

    ESP_ERROR_CHECK(phyphox_ble_start(name, &exp));
    ESP_LOGI(TAG, "open phyphox -> + -> Bluetooth -> %s", name);

    bool led = false;
    while (true) {
        float wave = 50.0f + 35.0f * sinf((float)(xTaskGetTickCount() * portTICK_PERIOD_MS) / 1500.0f);
        float value = fminf(fmaxf(wave, 0.0f), 100.0f);
        phyphox_ble_write1(value);

        led = !led;
        board_status_led_set(led);
        ESP_LOGI(TAG, "value=%.2f subscribed=%d", value, phyphox_ble_is_subscribed());
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
