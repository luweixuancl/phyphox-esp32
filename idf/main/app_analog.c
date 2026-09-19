#include "apps.h"
#include "board_pins.h"
#include "phyphox_ble.h"
#include "sdkconfig.h"

#include <math.h>
#include <stdio.h>

#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "app_analog";

void app_analog_run(void)
{
    char name[40];
    snprintf(name, sizeof(name), "%s-Analog", CONFIG_PHYPHX_DEVICE_PREFIX);

    phyphox_experiment_desc_t exp = {
        .title = "模拟传感器 (IDF5)",
        .category = "创客活动",
        .description = "读取 GPIO 模拟量。可用于电位器、光敏、土壤湿度等。",
        .view_label = "数据",
        .channel_count = 2,
        .channels =
            {
                {.value_label = "ADC", .unit = "", .graph_label = "ADC", .min_y = NAN, .max_y = NAN},
                {.value_label = "百分比",
                 .unit = "%",
                 .graph_label = "百分比",
                 .min_y = 0.0f,
                 .max_y = 100.0f},
            },
    };

    adc_oneshot_unit_handle_t adc;
    adc_oneshot_unit_init_cfg_t init_cfg = {
        .unit_id = ADC_UNIT_1,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_cfg, &adc));

    adc_oneshot_chan_cfg_t chan_cfg = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTEN_DB_12,
    };
    /* GPIO2 on ESP32-C3 is ADC1_CH2 */
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc, ADC_CHANNEL_2, &chan_cfg));

    ESP_ERROR_CHECK(phyphox_ble_start(name, &exp));
    ESP_LOGI(TAG, "analog on GPIO%d / BLE %s", PIN_ANALOG, name);

    bool led = false;
    while (true) {
        int raw = 0;
        ESP_ERROR_CHECK(adc_oneshot_read(adc, ADC_CHANNEL_2, &raw));
        float adc_f = (float)raw;
        float percent = (adc_f / 4095.0f) * 100.0f;
        phyphox_ble_write2(adc_f, percent);

        led = !led;
        board_status_led_set(led);
        ESP_LOGI(TAG, "raw=%d percent=%.1f", raw, percent);
        vTaskDelay(pdMS_TO_TICKS(80));
    }
}
