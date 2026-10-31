#ifndef PROCESS_FILE_H
#define PROCESS_FILE_H

#include <stdio.h>
#include <stdint.h>

int read_key_file(const char *filename, uint8_t key[8]);

int process_file(
    FILE *in,
    FILE *out,
    const uint64_t subkeys[16],
    int decrypt
);

#endif