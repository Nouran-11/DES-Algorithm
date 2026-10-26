# Testing

| Level | Command | What it proves |
|---|---|---|
| Tables | `make test-tables` | Every permutation is valid, FP = IP⁻¹, S-box rows are permutations of 0..15, helpers round-trip |
| Key schedule | `make test-keys` | All 16 subkeys for key 133457799BBCDFF1 match the reference |
| Full program | `make verify ID=<id>` | Exact `gcc -O3` build, 3 known-answer vectors, empty/identical-block/binary edge cases, 32 MB round trip, byte-match vs OpenSSL DES-ECB, 6 bad-CLI cases, submission hygiene |

Known-answer vectors (hex → stored as raw bytes):

| Key | Plaintext | Ciphertext |
|---|---|---|
| 133457799BBCDFF1 | 0123456789ABCDEF | 85E813540F0AB405 |
| 0000000000000000 | 0000000000000000 | 8CA64DE9C1B123A7 |
| 0E329232EA6D0D73 | 8787878787878787 | 0000000000000000 |

Rule: re-run `make verify` after every merge and after every optimization.
