#pragma once

#include <stddef.h>
#include <stdint.h>
#include "phyphox_ble.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Build phyphox XML into out buffer. Returns bytes written (excluding NUL), or 0 on failure. */
size_t phyphox_experiment_build(char *out, size_t out_cap,
                                const char *device_name,
                                const phyphox_experiment_desc_t *desc);

uint32_t phyphox_crc32(const uint8_t *data, size_t len);

#ifdef __cplusplus
}
#endif
