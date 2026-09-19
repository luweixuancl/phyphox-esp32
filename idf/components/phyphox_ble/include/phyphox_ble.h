#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Max floats notified in one packet (phyphox Arduino compatible). */
#define PHYPHX_MAX_CHANNELS 5

typedef struct {
    const char *title;
    const char *category;
    const char *description;
    const char *view_label;
    int channel_count; /**< 1..PHYPHX_MAX_CHANNELS */
    struct {
        const char *value_label;
        const char *unit;
        const char *graph_label;
        float min_y; /**< NAN = auto */
        float max_y;
    } channels[PHYPHX_MAX_CHANNELS];
} phyphox_experiment_desc_t;

/**
 * Start NimBLE peripheral advertising the phyphox services.
 * @param device_name BLE GAP name shown in phyphox scan (e.g. "Maker-C3-Hello")
 * @param experiment  Experiment metadata used to generate XML sent on connect
 */
esp_err_t phyphox_ble_start(const char *device_name, const phyphox_experiment_desc_t *experiment);

/** True after phone subscribed to the data characteristic. */
bool phyphox_ble_is_subscribed(void);

/** Notify up to PHYPHX_MAX_CHANNELS float32 little-endian values. */
esp_err_t phyphox_ble_write(const float *values, size_t count);

esp_err_t phyphox_ble_write1(float v0);
esp_err_t phyphox_ble_write2(float v0, float v1);
esp_err_t phyphox_ble_write3(float v0, float v1, float v2);
esp_err_t phyphox_ble_write4(float v0, float v1, float v2, float v3);
esp_err_t phyphox_ble_write5(float v0, float v1, float v2, float v3, float v4);

#ifdef __cplusplus
}
#endif
