# DES Encryption and Decryption

## 1. Project Overview

This project implements the **Data Encryption Standard (DES)** algorithm in C, supporting both encryption and decryption.

The program accepts a hexadecimal encryption key, reads an input file, processes its contents in 64-bit blocks, and writes the resulting ciphertext or decrypted data to an output file.

The implementation is written in standard C and combines the DES algorithm, key scheduling, bit manipulation, and file handling into a single source file for submission.

## 2. Features

- DES encryption and decryption.
- Electronic Codebook (ECB) mode.
- 64-bit block processing.
- Hexadecimal-only key input.
- DES key schedule generation (16 round subkeys).
- Initial and Final Permutations (IP and FP).
- Expansion, S-box substitution, and P permutation.
- Binary file input and output.
- Command-line interface for encryption and decryption.

## 3. Project Structure

```text
DES-Algorithm/
│
├── src/
│   └── 2200600.c
│
└── tests/
    ├── key.txt
    ├── plaintext.bin
    ├── ciphertext.bin
    └── decrypted.bin
```

The final submission consists of a single C source file: `2200600.c`.

## 4. Requirements

- GCC compiler.
- Windows PowerShell or another command-line terminal.
- No external libraries are required.

## 5. Compilation

Navigate to the project root directory and compile the source file using the required command:

```bash
gcc -O3 src/2200600.c -o 2200600
```

On Windows, this generates:

```text
2200600.exe
```

The `-O3` flag enables compiler optimizations.

## 6. Key Format

The program accepts the DES key in hexadecimal format only.

The key must contain exactly 16 hexadecimal characters, representing 8 bytes (64 bits).

Example `key.txt`:

```text
133457799BBCDFF1
```

The key is read from the specified key file and converted into the byte array required by the DES key schedule.

## 7. Usage

The program accepts four command-line arguments:

```text
2200600.exe <mode> <key_file> <input_file> <output_file>
```

| Argument | Description |
|---|---|
| `mode` | `e` for encryption or `d` for decryption |
| `key_file` | Path to the hexadecimal key file |
| `input_file` | Path to the file to be processed |
| `output_file` | Path where the result will be saved |

### 7.1 Encryption

To encrypt a file, use `e` as the operation mode:

```powershell
.\2200600.exe e tests\key.txt tests\plaintext.bin tests\ciphertext.bin
```

This reads the key and plaintext files, encrypts the input using DES in ECB mode, and saves the ciphertext to `ciphertext.bin`.

### 7.2 Decryption

To decrypt a previously encrypted file, use `d` as the operation mode:

```powershell
.\2200600.exe d tests\key.txt tests\ciphertext.bin tests\decrypted.bin
```

The program decrypts the ciphertext using the same key and saves the resulting plaintext to `decrypted.bin`.

## 8. Testing

### 8.1 Encryption and Decryption Verification

After encryption and decryption, compare the original plaintext file with the decrypted file:

```powershell
fc.exe tests\plaintext.bin tests\decrypted.bin
```

Expected output:

```text
FC: no differences encountered
```

This confirms that the decrypted file matches the original input file.

### 8.2 Standard DES Test Vector

The implementation can also be verified using the standard DES test vector:

| Parameter | Hexadecimal Value |
|---|---|
| Key | `133457799BBCDFF1` |
| Plaintext | `0123456789ABCDEF` |
| Expected Ciphertext | `85E813540F0AB405` |

### 8.3 Intermediate Round Verification

DES intermediate results can be compared against the DES example in Table 4.2, including:

- Initial permutation results.
- The 16 round subkeys.
- Left and right halves after each round.
- Final permutation result.

Expected ciphertext:

```text
DA02CE3A89ECAC3B
```

## 9. Important Notes

- The key must be provided as 16 hexadecimal characters.
- Encryption and decryption must use the same key.
- ECB processes data in independent 64-bit blocks.
- ECB mode does not provide the same security properties as modern authenticated encryption modes.
- The program should be tested with valid input files and key formats.

## 10. Build Command for Submission

The required compilation command is:

```bash
gcc -O3 src/2200600.c -o 2200600
```

The final submission source file is:

```text
src/2200600.c
```

