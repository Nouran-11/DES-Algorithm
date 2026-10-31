#include <stdio.h>
#include <stdint.h>

int main(void)
{
    /* 
     * The standard DES test vector plaintext: 0x0123456789ABCDEF
     * Represented as 8 raw binary bytes (64 bits total)
     */
    uint8_t plaintext[8] = {
        0x01, 0x23, 0x45, 0x67,
        0x89, 0xAB, 0xCD, 0xEF
    };

    /* Open the output file in binary write mode ("wb") */
    FILE *f = fopen("plaintext.bin", "wb");
    if (f == NULL) {
        perror("Error creating plaintext.bin");
        return 1;
    }

    /* Write the 8 raw bytes to the file */
    size_t written = fwrite(plaintext, 1, sizeof(plaintext), f);
    fclose(f);

    if (written != sizeof(plaintext)) {
        fprintf(stderr, "Error: Failed to write all 8 bytes to plaintext.bin\n");
        return 1;
    }

    printf("Successfully generated 'plaintext.bin' (8 raw binary bytes).\n");
    return 0;
}