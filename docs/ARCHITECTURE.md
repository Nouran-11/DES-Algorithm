# DES — Interface Contract (frozen)

Changes to anything in this file need agreement from all four members.

## Conventions
| Topic | Rule |
|---|---|
| Bit numbering | Table entries are 1-based, counted from the **MSB**. Use `des_permute(input, in_width, table, out_width)` from `bit_ops.h`. |
| Byte order | 8 file bytes → `uint64_t` with `des_load64_be()`; back with `des_store64_be()`. First byte = most significant. |
| Subkeys | `uint64_t subkeys[16]`, each in the low 48 bits, order K1..K16. Decrypt uses K16..K1 **inside** `des_block`. |
| Key file | **8 raw bytes** (assumption — confirm with TA, see Open questions). |
| Output | Same length as input. No padding, no headers, no metadata. |
| Naming | Every non-`static` symbol is prefixed `des_` / `DES_`. File-local helpers are `static` and must have unique names across all files (they all end up in one `.c`). |

## Modules and owners
| File | Owner | Public API |
|---|---|---|
| `src/bit_ops.h` | M4 | `des_permute`, `des_load64_be`, `des_store64_be` (static inline) |
| `src/des_tables.h/.c` | M4 | `DES_IP, DES_FP, DES_E, DES_P, DES_PC1, DES_PC2, DES_SHIFTS, DES_SBOX[8][4][16]` |
| `src/key_schedule.h/.c` | M1 | `void des_generate_subkeys(const uint8_t key[8], uint64_t subkeys[16]);` |
| `src/des.h/.c` | M2 | `uint64_t des_block(uint64_t block, const uint64_t subkeys[16], int decrypt);` |
| `src/file_ecb.h/.c` | M3 | `int process_file(FILE *in, FILE *out, const uint64_t subkeys[16], int decrypt);` returns 0 on success |
| `src/main.c` | M3 | `int main(int argc, char **argv)` — exit 0 on success, non-zero on any error |

`feistel()` is internal to `des.c` and should be `static`.

## Build
- `make test` — unit tests (tables, key schedule).
- `make submission ID=<id>` — merges into `build/<id>.c` (order: headers, then `des_tables.c, key_schedule.c, des.c, file_ecb.c, main.c`).
- `make verify ID=<id>` — merge + full end-to-end check with the exact graded command.

## Open questions
1. **Key file format**: raw 8 bytes or 16 hex characters? Ask the TA. If no answer, `main` may accept both: exactly 8 bytes → raw; 16 hex chars (optionally followed by a newline) → parse hex.
