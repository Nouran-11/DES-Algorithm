#ifndef BIT_OPS_H
#define BIT_OPS_H

#include <stdint.h>

/*
 * Shared bit helpers. Header-only (static inline) so -O3 can inline
 * them and so the single-file merge needs no extra .c file.
 *
 * BIT CONVENTION: table entries are 1-based positions counted from the
 * MOST significant bit of an input_width-bit value. The result is
 * right-aligned in the low output_width bits.
 * (Same argument order as the permute_bits() already in key_schedule.c.)
 */
static inline uint64_t des_permute(
    uint64_t input,
    int input_width,
    const uint8_t *table,
    int output_width
)
{
    uint64_t result = 0;
    for (int i = 0; i < output_width; i++) {
        result = (result << 1) |
                 ((input >> (input_width - table[i])) & 1ULL);
    }
    return result;
}

/* 8 bytes -> uint64, first byte becomes the most significant byte. */
static inline uint64_t des_load64_be(const uint8_t b[8])
{
    return ((uint64_t)b[0] << 56) | ((uint64_t)b[1] << 48) |
           ((uint64_t)b[2] << 40) | ((uint64_t)b[3] << 32) |
           ((uint64_t)b[4] << 24) | ((uint64_t)b[5] << 16) |
           ((uint64_t)b[6] <<  8) |  (uint64_t)b[7];
}

/* uint64 -> 8 bytes, most significant byte first. */
static inline void des_store64_be(uint64_t x, uint8_t b[8])
{
    for (int i = 7; i >= 0; i--) {
        b[i] = (uint8_t)x;
        x >>= 8;
    }
}

#endif
