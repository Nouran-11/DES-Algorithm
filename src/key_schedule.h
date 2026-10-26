#ifndef KEY_SCHEDULE_H
#define KEY_SCHEDULE_H

#include <stdint.h>

/*
 * Generate the 16 DES round subkeys.
 *
 * key:
 *   Original 8-byte DES key.
 *
 * subkeys:
 *   Output array of 16 subkeys.
 *   Each subkey occupies the lowest 48 bits
 *   of a uint64_t.
 *
 * Subkeys are stored in encryption order:
 * K1, K2, ..., K16.
 */
void des_generate_subkeys(
    const uint8_t key[8],
    uint64_t subkeys[16]
);

#endif
