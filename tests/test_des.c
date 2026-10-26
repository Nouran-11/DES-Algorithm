#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#include "../src/des.h"
#include "../src/key_schedule.h"

int main(void)
{
    uint8_t key[8] = {
        0x13, 0x34, 0x57, 0x79,
        0x9B, 0xBC, 0xDF, 0xF1
    };

    uint64_t plaintext = UINT64_C(0x0123456789ABCDEF);
    uint64_t expected  = UINT64_C(0x85E813540F0AB405);

    uint64_t subkeys[16];
    des_generate_subkeys(key, subkeys);

    uint64_t ciphertext = des_block(plaintext, subkeys, 0);

    printf("Ciphertext: %016" PRIX64 "\n", ciphertext);

    assert(ciphertext == expected);

    uint64_t decrypted = des_block(ciphertext, subkeys, 1);

    printf("Decrypted:  %016" PRIX64 "\n", decrypted);

    assert(decrypted == plaintext);

    printf("All DES tests passed!\n");

    return 0;
}