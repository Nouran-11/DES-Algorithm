/* 
Team names and IDs:

- Menna Tarik Mohamed Tawfik  2200600
- Menna Mohamed NasrEldin  2200806
- Fatma Mohamed Hany  2200558
- Nouran Atef AbdelGayed  2200165
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

//----------copying des_tables.c content----------------

#include "des_tables.h"

const uint8_t DES_IP[64] = {
    58, 50, 42, 34, 26, 18, 10,  2,
    60, 52, 44, 36, 28, 20, 12,  4,
    62, 54, 46, 38, 30, 22, 14,  6,
    64, 56, 48, 40, 32, 24, 16,  8,
    57, 49, 41, 33, 25, 17,  9,  1,
    59, 51, 43, 35, 27, 19, 11,  3,
    61, 53, 45, 37, 29, 21, 13,  5,
    63, 55, 47, 39, 31, 23, 15,  7
};

const uint8_t DES_FP[64] = {
    40,  8, 48, 16, 56, 24, 64, 32,
    39,  7, 47, 15, 55, 23, 63, 31,
    38,  6, 46, 14, 54, 22, 62, 30,
    37,  5, 45, 13, 53, 21, 61, 29,
    36,  4, 44, 12, 52, 20, 60, 28,
    35,  3, 43, 11, 51, 19, 59, 27,
    34,  2, 42, 10, 50, 18, 58, 26,
    33,  1, 41,  9, 49, 17, 57, 25
};

const uint8_t DES_E[48] = {
    32,  1,  2,  3,  4,  5,
     4,  5,  6,  7,  8,  9,
     8,  9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32,  1
};

const uint8_t DES_P[32] = {
    16,  7, 20, 21, 29, 12, 28, 17,
     1, 15, 23, 26,  5, 18, 31, 10,
     2,  8, 24, 14, 32, 27,  3,  9,
    19, 13, 30,  6, 22, 11,  4, 25
};

const uint8_t DES_PC1[56] = {
    57, 49, 41, 33, 25, 17,  9,
     1, 58, 50, 42, 34, 26, 18,
    10,  2, 59, 51, 43, 35, 27,
    19, 11,  3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
     7, 62, 54, 46, 38, 30, 22,
    14,  6, 61, 53, 45, 37, 29,
    21, 13,  5, 28, 20, 12,  4
};

const uint8_t DES_PC2[48] = {
    14, 17, 11, 24,  1,  5,
     3, 28, 15,  6, 21, 10,
    23, 19, 12,  4, 26,  8,
    16,  7, 27, 20, 13,  2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

const uint8_t DES_SHIFTS[16] = {
    1, 1, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 1
};

const uint8_t DES_SBOX[8][4][16] = {
    { /* S1 */
        {14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7},
        { 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8},
        { 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0},
        {15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13}
    },
    { /* S2 */
        {15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10},
        { 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5},
        { 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15},
        {13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9}
    },
    { /* S3 */
        {10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8},
        {13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1},
        {13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7},
        { 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12}
    },
    { /* S4 */
        { 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15},
        {13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9},
        {10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4},
        { 3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14}
    },
    { /* S5 */
        { 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9},
        {14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6},
        { 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14},
        {11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3}
    },
    { /* S6 */
        {12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11},
        {10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8},
        { 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6},
        { 4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13}
    },
    { /* S7 */
        { 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1},
        {13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6},
        { 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2},
        { 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12}
    },
    { /* S8 */
        {13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7},
        { 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2},
        { 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8},
        { 2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11}
    }
};

//----------copying bit_ops.h content----------------
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

//----------copying key_schedule.c content----------------
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

//----------copying des.c content----------------
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


//----------copying process_file.c content----------------
static int hex_value(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    return -1;
}

/* 
 * Function: read_key_file
 * Purpose: Reads a 16-character hexadecimal key file and converts it into an 8-byte array.
 */
int read_key_file(const char *filename, uint8_t key[8])
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Cannot open key file");
        return 0;
    }

    char buffer[19];
    size_t n = fread(buffer, 1, sizeof(buffer), file);
    int read_error = ferror(file);
    fclose(file);

    if (read_error) {
        fprintf(stderr, "Error reading key file.\n");
        return 0;
    }

    /* Accept an optional trailing newline */
    if (n == 17 && buffer[16] == '\n') {
        n = 16;
    } else if (n == 18 && buffer[16] == '\r' && buffer[17] == '\n') {
        n = 16;
    }

    /* Key must contain exactly 16 hex characters */
    if (n != 16) {
        fprintf(stderr, "Invalid key: expected 16 hexadecimal characters.\n");
        return 0;
    }

    /* Convert every two hex characters into one byte */
    for (int i = 0; i < 8; i++) {
        int high = hex_value(buffer[2 * i]);
        int low  = hex_value(buffer[2 * i + 1]);

        if (high == -1 || low == -1) {
            fprintf(stderr, "Invalid hexadecimal character in key file.\n");
            return 0;
        }

        key[i] = (uint8_t)((high << 4) | low);
    }

    return 1;
}

/* 
 * Function: process_file
 * Purpose: Reads the input file in 8-byte blocks, processes each block via ECB mode, 
 * and writes the result to the output file.
 */
int process_file(
    FILE *in,
    FILE *out,
    const uint64_t subkeys[16],
    int decrypt
)
{
    uint8_t input_buffer[8];
    uint8_t output_buffer[8];

    while (1) {
        /* Read 8 bytes from input file[cite: 1, 8] */
        size_t bytes_read = fread(input_buffer, 1, 8, in);

        /* Check for end of file */
        if (bytes_read == 0) {
            if (ferror(in)) {
                fprintf(stderr, "Error reading input file.\n");
                return 0;
            }
            break;
        }

        /* Check for incomplete block (no padding allowed)[cite: 1, 8] */
        if (bytes_read != 8) {
            fprintf(stderr, "Incomplete DES block size (must be a multiple of 8 bytes).\n");
            return 0;
        }

        /* Convert bytes into a 64-bit block using big-endian helper[cite: 4] */
        uint64_t block = des_load64_be(input_buffer);

        /* Encrypt or decrypt using DES block function (Member 2)[cite: 2, 4] */
        uint64_t result = des_block(block, subkeys, decrypt);

        /* Convert result back into 8 bytes[cite: 4] */
        des_store64_be(result, output_buffer);

        /* Write 8 bytes into output file[cite: 1, 8] */
        size_t bytes_written = fwrite(output_buffer, 1, 8, out);

        if (bytes_written != 8) {
            fprintf(stderr, "Error writing output file.\n");
            return 0;
        }
    }

    return 1;
}
//----------copying main.c content----------------

int main(int argc, char **argv)
{
    /* Step 1: Validate argument count */
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <e/d> <key_file> <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    /* Step 2: Determine mode (e for encrypt, d for decrypt)[cite: 1, 8] */
    int decrypt;
    if (strcmp(argv[1], "e") == 0) {
        decrypt = 0;
    } else if (strcmp(argv[1], "d") == 0) {
        decrypt = 1;
    } else {
        fprintf(stderr, "Invalid mode. Use 'e' for encryption or 'd' for decryption[cite: 1, 8].\n");
        return 1;
    }

    /* Step 3: Prevent input and output files from being the exact same path */
    if (strcmp(argv[3], argv[4]) == 0) {
        fprintf(stderr, "Input and output files must be different.\n");
        return 1;
    }

    /* Step 4: Read the 16-char hex key file into an 8-byte array */
    uint8_t key[8];
    if (!read_key_file(argv[2], key)) {
        return 1;
    }

    /* Step 5: Generate the 16 DES round subkeys (Member 1)[cite: 1, 2] */
    uint64_t subkeys[16];
    des_generate_subkeys(key, subkeys);

    /* Step 6: Open input file in binary mode ("rb")[cite: 7] */
    FILE *in = fopen(argv[3], "rb");
    if (in == NULL) {
        perror("Cannot open input file");
        return 1;
    }

    /* Step 7: Open output file in binary mode ("wb")[cite: 7] */
    FILE *out = fopen(argv[4], "wb");
    if (out == NULL) {
        perror("Cannot open output file");
        fclose(in);
        return 1;
    }

    /* Step 8: Process the file using ECB mode */
    int success = process_file(in, out, subkeys, decrypt);

    /* Step 9: Close files and check for stream errors */
    if (fclose(in) != 0) {
        fprintf(stderr, "Error closing input file.\n");
        success = 0;
    }

    if (fclose(out) != 0) {
        fprintf(stderr, "Error closing output file.\n");
        success = 0;
    }

    /* Step 10: Exit with appropriate code */
    if (!success) {
        return 1;
    }

    return 0;
}