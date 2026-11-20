
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

/*
 * Include the complete submission source.
 * Rename its main() temporarily to avoid having two main functions.
 */
#define main student_program_main
#include "2200600.c"
#undef main


/* Convert one hexadecimal character to its value */
static int table_hex_value(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';

    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;

    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;

    return -1;
}


/*
 * Convert eight 6-bit hexadecimal values
 * into one packed 48-bit DES subkey.
 */
static int parse_table_subkey(const char *hex, uint64_t *result)
{
    if (strlen(hex) != 16)
        return 0;

    uint64_t value = 0;

    for (int i = 0; i < 8; i++) {

        int high = table_hex_value(hex[2 * i]);
        int low  = table_hex_value(hex[2 * i + 1]);

        if (high < 0 || low < 0)
            return 0;

        uint8_t chunk = (uint8_t)((high << 4) | low);

        if (chunk > 0x3F)
            return 0;

        value = (value << 6) | chunk;
    }

    *result = value;
    return 1;
}


int main(void)
{
    /* The 16 subkeys from Table 4.2 */
    const char *round_keys[16] = {
        "1e030f03080d2930",
        "0a31293432242318",
        "23072318201d0c1d",
        "05261d3824311a20",
        "3325340136002c25",
        "123a2d0d04262a1c",
        "021f120b1c130611",
        "1c10372a2832002b",
        "04292a380c341f03",
        "2703212607280403",
        "2826390c31261504",
        "12071c241a0a0f08",
        "300935393c0d100b",
        "311e09231321182a",
        "283d3e0227072528",
        "2921080b13143025"
    };

    uint64_t subkeys[16];

    /* Step 1: Convert all round subkeys */
    for (int i = 0; i < 16; i++) {

        if (!parse_table_subkey(round_keys[i], &subkeys[i])) {
            printf("Invalid subkey at round %d\n", i + 1);
            return 1;
        }
    }

    printf("All 16 subkeys loaded successfully.\n");


    /* Step 2: Initial permutation result from the table */
    uint64_t initial = UINT64_C(0x5A005A003CF03C0F);

    /*
     * Step 3: Reconstruct the original plaintext.
     * IP inverse = DES_FP.
     */
    uint64_t plaintext = des_permute(
        initial,
        64,
        DES_FP,
        64
    );

    printf("Plaintext:  %016" PRIX64 "\n", plaintext);


    /* Step 4: Encrypt using the supplied round subkeys */
    uint64_t ciphertext = des_block(
        plaintext,
        subkeys,
        0
    );

    printf("Ciphertext: %016" PRIX64 "\n", ciphertext);


    /* Step 5: Compare against Table 4.2 */
    uint64_t expected = UINT64_C(0xDA02CE3A89ECAC3B);

    if (ciphertext == expected) {
        printf("\nTable 4.2 test PASSED!\n");
    }
    else {
        printf("\nTable 4.2 test FAILED!\n");
        printf("Expected: %016" PRIX64 "\n", expected);
        printf("Got:      %016" PRIX64 "\n", ciphertext);
        return 1;
    }

    return 0;
}
