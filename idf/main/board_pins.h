#pragma once

#include <stdbool.h>

/** Default GPIO map for ESP32-C3 DevKit / SuperMini style boards. */
#define PIN_STATUS_LED 8
#define PIN_ANALOG     2
#define PIN_US_TRIG    6
#define PIN_US_ECHO    7

void board_status_led_init(void);
void board_status_led_set(bool on);
void board_status_led_blink_once(void);
