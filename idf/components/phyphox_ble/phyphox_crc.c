#include "phyphox_experiment.h"

#include <string.h>

uint32_t phyphox_crc32(const uint8_t *data, size_t len)
{
    uint32_t table[256];
    const uint32_t polynomial = 0xEDB88320u;
    for (uint32_t i = 0; i < 256; i++) {
        uint32_t c = i;
        for (size_t j = 0; j < 8; j++) {
            if (c & 1u) {
                c = polynomial ^ (c >> 1);
            } else {
                c >>= 1;
            }
        }
        table[i] = c;
    }

    uint32_t c = 0xFFFFFFFFu;
    for (size_t i = 0; i < len; i++) {
        c = table[(c ^ data[i]) & 0xFFu] ^ (c >> 8);
    }
    return c ^ 0xFFFFFFFFu;
}
