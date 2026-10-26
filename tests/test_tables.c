/*
 * Member 4 self-test: structural audit of every DES table
 * and the shared bit helpers.
 */
#include <stdio.h>
#include <stdint.h>
#include "../src/des_tables.h"
#include "../src/bit_ops.h"

static int failures = 0;
#define CHECK(cond, msg) do { if (!(cond)) { fprintf(stderr, "FAIL: %s\n", msg); failures++; } } while (0)

/* every value 1..n appears exactly once */
static int is_permutation(const uint8_t *t, int n)
{
    int seen[65] = {0};
    for (int i = 0; i < n; i++) {
        if (t[i] < 1 || t[i] > n || seen[t[i]]) return 0;
        seen[t[i]] = 1;
    }
    return 1;
}

int main(void)
{
    CHECK(is_permutation(DES_IP, 64), "IP is not a permutation of 1..64");
    CHECK(is_permutation(DES_FP, 64), "FP is not a permutation of 1..64");
    CHECK(is_permutation(DES_P, 32),  "P is not a permutation of 1..32");

    /* FP must be the exact inverse of IP */
    for (int i = 0; i < 64; i++)
        CHECK(DES_FP[DES_IP[i] - 1] == i + 1, "FP is not the inverse of IP");

    /* E: 48 entries in 1..32, each input bit used once or twice */
    int ecount[33] = {0};
    for (int i = 0; i < 48; i++) {
        CHECK(DES_E[i] >= 1 && DES_E[i] <= 32, "E entry out of range");
        ecount[DES_E[i]]++;
    }
    for (int i = 1; i <= 32; i++)
        CHECK(ecount[i] == 1 || ecount[i] == 2, "E uses a bit 0 or >2 times");

    /* PC-1 never selects parity bits 8,16,...,64 */
    for (int i = 0; i < 56; i++)
        CHECK(DES_PC1[i] % 8 != 0, "PC-1 selects a parity bit");

    /* PC-2: 48 distinct entries in 1..56; it drops 9,18,22,25,35,38,43,54 */
    int pc2seen[57] = {0};
    for (int i = 0; i < 48; i++) {
        CHECK(DES_PC2[i] >= 1 && DES_PC2[i] <= 56 && !pc2seen[DES_PC2[i]], "PC-2 bad/duplicate entry");
        pc2seen[DES_PC2[i]] = 1;
    }

    /* shifts must total 28 so C and D return to start after 16 rounds */
    int total = 0;
    for (int i = 0; i < 16; i++) total += DES_SHIFTS[i];
    CHECK(total == 28, "shift schedule does not total 28");

    /* each S-box row is a permutation of 0..15 */
    for (int s = 0; s < 8; s++)
        for (int r = 0; r < 4; r++) {
            int seen[16] = {0}, ok = 1;
            for (int c = 0; c < 16; c++) {
                if (DES_SBOX[s][r][c] > 15 || seen[DES_SBOX[s][r][c]]) ok = 0;
                else seen[DES_SBOX[s][r][c]] = 1;
            }
            if (!ok) { fprintf(stderr, "FAIL: S%d row %d not a permutation of 0..15\n", s + 1, r); failures++; }
        }

    /* spot values from the FIPS 46-3 tables */
    CHECK(DES_SBOX[0][0][0] == 14 && DES_SBOX[7][3][15] == 11, "S-box corner values wrong");

    /* helpers: byte round trip and big-endian order */
    uint8_t in[8] = {0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF}, out[8];
    uint64_t v = des_load64_be(in);
    CHECK(v == 0x0123456789ABCDEFULL, "load64_be byte order wrong");
    des_store64_be(v, out);
    for (int i = 0; i < 8; i++) CHECK(in[i] == out[i], "store64_be round trip wrong");

    /* known IP output for plaintext 0123456789ABCDEF (Grabbe worked example) */
    CHECK(des_permute(v, 64, DES_IP, 64) == 0xCC00CCFFF0AAF0AAULL, "IP(0123456789ABCDEF) wrong");
    /* IP then FP is identity */
    CHECK(des_permute(des_permute(v, 64, DES_IP, 64), 64, DES_FP, 64) == v, "FP(IP(x)) != x");

    if (failures) { fprintf(stderr, "%d table/helper checks failed.\n", failures); return 1; }
    printf("PASS: all DES tables and bit helpers verified.\n");
    return 0;
}
