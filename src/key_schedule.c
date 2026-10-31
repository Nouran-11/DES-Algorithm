#include "key_schedule.h"
#include "des_tables.h"
#include "bit_ops.h"

/*
 * Convert an 8-byte key into a 64-bit integer.
 *
 * Bytes are interpreted in DES bit order:
 * the first byte contains the most significant bits.
 */
static uint64_t load_key64(const uint8_t key[8])
{
    uint64_t result = 0;

    for (int i = 0; i < 8; i++) {
        result = (result << 8) | key[i];
    }

    return result;
}

/*
 * Generic bit permutation.
 *
 * Table positions are 1-based and count from
 * the most significant bit of the input.
 *
 * The result is right-aligned.
 */
static uint64_t permute_bits(
    uint64_t input,
    int input_width,
    const uint8_t *table,
    int output_width
)
{
    uint64_t result = 0;

    for (int i = 0; i < output_width; i++) {

        int position = table[i];

        uint64_t bit =
            (input >> (input_width - position)) & 1ULL;

        result = (result << 1) | bit;
    }

    return result;
}

/*
 * Circular left rotation of a 28-bit value.
 *
 * DES rotations are always 1 or 2 positions.
 */
static uint32_t rotate_left28(uint32_t value, int shift)
{
    return (
        (value << shift) |
        (value >> (28 - shift))
    ) & 0x0FFFFFFFU;
}

/*
 * Generate all 16 DES round subkeys.
 */
void des_generate_subkeys(
    const uint8_t key[8],
    uint64_t subkeys[16]
)
{
    /* Step 1: Load original 64-bit key */
    uint64_t key64 = load_key64(key);

    /* Step 2: Apply PC-1 (64 -> 56 bits) */
    uint64_t key56 =
        permute_bits(key64, 64, DES_PC1, 56);

    /* Step 3: Split into two 28-bit halves */
    uint32_t C =
        (uint32_t)((key56 >> 28) & 0x0FFFFFFFU);

    uint32_t D =
        (uint32_t)(key56 & 0x0FFFFFFFU);

    /* Step 4: Generate 16 round subkeys */
    for (int round = 0; round < 16; round++) {

        /* Rotate each half */
        C = rotate_left28(C, DES_SHIFTS[round]);
        D = rotate_left28(D, DES_SHIFTS[round]);

        /* Recombine C and D into 56 bits */
        uint64_t combined =
            ((uint64_t)C << 28) | D;

        /* Apply PC-2 (56 -> 48 bits) */
        subkeys[round] =
            permute_bits(combined, 56, DES_PC2, 48);
    }
}
