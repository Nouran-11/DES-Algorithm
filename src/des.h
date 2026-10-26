#ifndef DES_H
#define DES_H

#include <stdint.h>

/*
 * DES Feistel round function.
 * Takes a 32-bit right half and a 48-bit round subkey.
 */
uint32_t des_feistel(uint32_t right, uint64_t subkey48);

/*
 * DES block transformation.
 * block64: 64-bit input block.
 * subkeys: 16 round keys in encryption order.
 * decrypt: 0 for encryption, nonzero for decryption.
 */
uint64_t des_block(
    uint64_t block64,
    const uint64_t subkeys[16],
    int decrypt
);

#endif