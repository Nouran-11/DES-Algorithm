#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "process_file.h"
#include "key_schedule.h"

/* 
 * Function: main
 * Purpose: Validates command-line arguments, loads key/subkeys, handles file I/O, 
 * and invokes file processing[cite: 3, 8].
 */
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