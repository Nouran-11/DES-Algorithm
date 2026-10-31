#include "process_file.h"
#include "des.h"
#include "bit_ops.h"
#include <stdlib.h>
#include <string.h>

/* 
 * Helper: Convert one hexadecimal character into its numerical value [0-15].
 * Declared as static so it is only visible inside this file.
 */
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