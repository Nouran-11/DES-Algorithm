#ifndef DES_TABLES_H
#define DES_TABLES_H

#include <stdint.h>

/*
 * Standard DES tables (FIPS 46-3).
 *
 * CONVENTION (all permutation tables):
 *   Entries are 1-based bit positions counted from the
 *   MOST significant bit of the input value.
 *   Use them with des_permute() from bit_ops.h.
 *
 * Names are prefixed DES_ so they never collide with
 * file-local tables when everything is merged into one .c file.
 */

extern const uint8_t DES_IP[64];       /* initial permutation       64 -> 64 */
extern const uint8_t DES_FP[64];       /* final permutation (IP^-1) 64 -> 64 */
extern const uint8_t DES_E[48];        /* expansion                 32 -> 48 */
extern const uint8_t DES_P[32];        /* round permutation         32 -> 32 */
extern const uint8_t DES_PC1[56];      /* key permuted choice 1     64 -> 56 */
extern const uint8_t DES_PC2[48];      /* key permuted choice 2     56 -> 48 */
extern const uint8_t DES_SHIFTS[16];   /* left rotations per round           */

/*
 * S-boxes: DES_SBOX[box][row][col], box 0..7 = S1..S8.
 * For a 6-bit chunk b1..b6 (b1 = MSB):
 *   row = b1b6, col = b2b3b4b5
 */
extern const uint8_t DES_SBOX[8][4][16];

#endif
