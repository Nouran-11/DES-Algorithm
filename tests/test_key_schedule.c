#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "../src/key_schedule.h"

int main(void)
{
    /* Standard DES test key */
    const uint8_t key[8] = {
        0x13, 0x34, 0x57, 0x79,
        0x9B, 0xBC, 0xDF, 0xF1
    };

    /* Expected 16 DES round keys */
    const uint64_t expected[16] = {
        UINT64_C(0x1B02EFFC7072),
        UINT64_C(0x79AED9DBC9E5),
        UINT64_C(0x55FC8A42CF99),
        UINT64_C(0x72ADD6DB351D),
        UINT64_C(0x7CEC07EB53A8),
        UINT64_C(0x63A53E507B2F),
        UINT64_C(0xEC84B7F618BC),
        UINT64_C(0xF78A3AC13BFB),
        UINT64_C(0xE0DBEBEDE781),
        UINT64_C(0xB1F347BA464F),
        UINT64_C(0x215FD3DED386),
        UINT64_C(0x7571F59467E9),
        UINT64_C(0x97C5D1FABA41),
        UINT64_C(0x5F43B7F2E73A),
        UINT64_C(0xBF918D3D3F0A),
        UINT64_C(0xCB3D8B0E17F5)
    };

    uint64_t subkeys[16] = {0};

    des_generate_subkeys(key, subkeys);

    int failures = 0;

    for (int i = 0; i < 16; i++) {

        if (subkeys[i] != expected[i]) {
            fprintf(
                stderr,
                "Round %d FAILED\n"
                "Expected: %012" PRIX64 "\n"
                "Actual:   %012" PRIX64 "\n",
                i + 1,
                expected[i],
                subkeys[i]
            );

            failures++;
        }
    }

    if (failures != 0) {
        fprintf(
            stderr,
            "%d round key tests failed.\n",
            failures
        );

        return 1;
    }

    /*
     * Changing only the parity bit in each byte
     * must not change the generated DES subkeys.
     */
    uint8_t parity_changed_key[8];

    for (int i = 0; i < 8; i++) {
        parity_changed_key[i] = key[i] ^ 0x01U;
    }

    uint64_t parity_subkeys[16] = {0};

    des_generate_subkeys(
        parity_changed_key,
        parity_subkeys
    );

    for (int i = 0; i < 16; i++) {
        if (parity_subkeys[i] != expected[i]) {
            fprintf(
                stderr,
                "Parity independence test failed.\n"
            );

            return 1;
        }
    }

    /*
     * An all-zero input key must generate
     * all-zero round subkeys.
     */
    const uint8_t zero_key[8] = {0};
    uint64_t zero_subkeys[16] = {0};

    des_generate_subkeys(zero_key, zero_subkeys);

    for (int i = 0; i < 16; i++) {
        if (zero_subkeys[i] != 0) {
            fprintf(
                stderr,
                "Zero-key test failed at round %d.\n",
                i + 1
            );

            return 1;
        }
    }

    printf("PASS: All 16 DES subkeys match.\n");
    printf("PASS: Parity independence test.\n");
    printf("PASS: Zero-key test.\n");

    return 0;
}
