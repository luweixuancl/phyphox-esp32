#include "apps.h"
#include "board_pins.h"

#include "esp_log.h"
#include "nvs_flash.h"

static const char *TAG = "main";

void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    }

    board_status_led_init();
    board_status_led_blink_once();
    board_status_led_blink_once();

#if CONFIG_PHYPHX_APP_HELLO
    ESP_LOGI(TAG, "starting Hello Phyphox (ESP-IDF 5)");
    app_hello_run();
#elif CONFIG_PHYPHX_APP_ANALOG
    ESP_LOGI(TAG, "starting Analog ADC (ESP-IDF 5)");
    app_analog_run();
#elif CONFIG_PHYPHX_APP_ULTRASONIC
    ESP_LOGI(TAG, "starting Ultrasonic (ESP-IDF 5)");
    app_ultrasonic_run();
#else
#error "Select a PHYPHX_APP in menuconfig"
#endif
}
