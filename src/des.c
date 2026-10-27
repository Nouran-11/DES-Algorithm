//
// Created by menna on 10/26/2026.
//
#include <stdint.h>

#include "bit_ops.h"
#include "des_tables.h"
#include "des.h"

/*
 * DES Feistel function:
 * Expansion -> XOR -> S-boxes -> P permutation.
 */
uint32_t des_feistel(uint32_t right, uint64_t subkey48)
{
    /* Expand 32 bits to 48 bits. */
    uint64_t expanded = des_permute(
        (uint64_t)right,
        32,
        DES_E,
        48
    );

    /* Mix the expanded block with the round key. */
    uint64_t mixed = expanded ^ subkey48;

    /* Apply the eight S-boxes: 48 bits -> 32 bits. */
    uint32_t sbox_output = 0;

    for (int i = 0; i < 8; i++) {
        uint8_t chunk = (uint8_t)(
            (mixed >> (42 - 6 * i)) & 0x3F
        );

        int row = ((chunk & 0x20) >> 4) |
                  (chunk & 0x01);

        int col = (chunk >> 1) & 0x0F;

        uint8_t value = DES_SBOX[i][row][col];

        sbox_output = (sbox_output << 4) | value;
    }

    /* Apply the P permutation. */
    return (uint32_t)des_permute(
        (uint64_t)sbox_output,
        32,
        DES_P,
        32
    );
}

/*
 * Encrypt or decrypt one 64-bit DES block.
 */
uint64_t des_block(
    uint64_t block64,
    const uint64_t subkeys[16],
    int decrypt
)
{
    /* Initial permutation. */
    uint64_t permuted = des_permute(
        block64,
        64,
        DES_IP,
        64
    );

    /* Split into 32-bit halves. */
    uint32_t L = (uint32_t)(permuted >> 32);
    uint32_t R = (uint32_t)permuted;

    /* Perform the 16 Feistel rounds. */
    for (int round = 0; round < 16; round++) {
        int key_index = decrypt ? (15 - round) : round;

        uint32_t nextL = R;

        uint32_t nextR =
            L ^ des_feistel(R, subkeys[key_index]);

        L = nextL;
        R = nextR;
    }

    /* Swap the halves before the final permutation. */
    uint64_t combined = ((uint64_t)R << 32) | L;

    /* Final permutation: inverse of IP. */
    return des_permute(
        combined,
        64,
        DES_FP,
        64
    );
}