#!/usr/bin/env bash
# End-to-end check of the single-file submission.
# Usage: scripts/verify_submission.sh build/studentID.c [large_file_MB]
# Needs: gcc, python3, cmp. Uses openssl (legacy provider) as an
# independent DES reference when available.
set -uo pipefail

SRC="$(realpath "${1:?usage: verify_submission.sh path/to/studentID.c [MB]}")"
MB="${2:-32}"
T="$(mktemp -d)"; trap 'rm -rf "$T"' EXIT
cd "$T"
PASS=0; FAIL=0
ok()  { echo "  PASS  $1"; PASS=$((PASS+1)); }
bad() { echo "  FAIL  $1"; FAIL=$((FAIL+1)); }
hex2bin() { python3 -c "import sys;sys.stdout.buffer.write(bytes.fromhex(sys.argv[1]))" "$1" > "$2"; }
bin2hex() { python3 -c "import sys;print(open(sys.argv[1],'rb').read().hex().upper())" "$1"; }

echo "== 1. Exact build command"
cp "$SRC" studentID.c
if gcc -O3 studentID.c -o studentID 2> build.log; then ok "gcc -O3 studentID.c -o studentID"
else bad "compile (see below)"; cat build.log; exit 1; fi
[[ -s build.log ]] && { echo "  note: compiler warnings:"; sed 's/^/    /' build.log; }
P=./studentID

echo "== 2. Known-answer vectors (key file = 8 raw bytes)"
while read -r K PT CT; do
  hex2bin "$K" k.bin; hex2bin "$PT" p.bin
  $P e k.bin p.bin c.bin; got=$(bin2hex c.bin)
  [[ "$got" == "$CT" ]] && ok "E $K/$PT -> $CT" || bad "E $K/$PT expected $CT got $got"
  $P d k.bin c.bin r.bin
  cmp -s p.bin r.bin && ok "D back to $PT" || bad "D $K/$CT did not return $PT"
done <<'V'
133457799BBCDFF1 0123456789ABCDEF 85E813540F0AB405
0000000000000000 0000000000000000 8CA64DE9C1B123A7
0E329232EA6D0D73 8787878787878787 0000000000000000
V

echo "== 3. Edge cases"
hex2bin 133457799BBCDFF1 k.bin
: > empty.bin
$P e k.bin empty.bin out.bin; rc=$?
[[ $rc -lt 128 && -f out.bin && ! -s out.bin ]] && ok "empty input -> empty output" || bad "empty input (rc=$rc)"
hex2bin 0123456789ABCDEF0123456789ABCDEF same.bin
$P e k.bin same.bin same.enc
[[ "$(bin2hex same.enc)" == "85E813540F0AB40585E813540F0AB405" ]] && ok "ECB: identical blocks -> identical ciphertext" || bad "ECB identical blocks"
python3 -c "import sys;sys.stdout.buffer.write(bytes([0]*8+[255]*8+list(range(256))))" > edge.bin
$P e k.bin edge.bin edge.enc; $P d k.bin edge.enc edge.dec
cmp -s edge.bin edge.dec && ok "0x00/0xFF/all-byte round trip" || bad "binary round trip"

echo "== 4. Large file (${MB} MB) correctness + timing"
python3 -c "import os,sys;sys.stdout.buffer.write(os.urandom(int(sys.argv[1])*1048576))" "$MB" > big.bin
python3 -c "import os,sys;sys.stdout.buffer.write(os.urandom(8))" > rk.bin
s=$(date +%s.%N); $P e rk.bin big.bin big.enc; m=$(date +%s.%N); $P d rk.bin big.enc big.dec; e=$(date +%s.%N)
[[ $(stat -c%s big.enc) -eq $(stat -c%s big.bin) ]] && ok "output size == input size" || bad "output size changed (padding?)"
cmp -s big.bin big.dec && ok "large round trip byte-identical" || bad "large round trip differs"
printf "  time  encrypt %.3fs   decrypt %.3fs\n" "$(echo "$m-$s" | bc)" "$(echo "$e-$m" | bc)"
if openssl enc -des-ecb -nopad -K "$(bin2hex rk.bin)" -provider legacy -provider default -in big.bin -out ref.enc 2>/dev/null; then
  cmp -s big.enc ref.enc && ok "matches OpenSSL DES-ECB on random key + data" || bad "differs from OpenSSL DES-ECB"
else
  echo "  skip  OpenSSL reference not available"
fi

echo "== 5. Bad command lines must fail cleanly (non-zero, no crash)"
chk() { local name="$1"; shift; "$@" >/dev/null 2>&1; local rc=$?
  if [[ $rc -ne 0 && $rc -lt 128 ]]; then ok "$name (rc=$rc)"; else bad "$name (rc=$rc; 0=silent success, >=128=crash)"; fi; }
chk "no arguments"        $P
chk "too few arguments"   $P e k.bin
chk "invalid mode 'x'"    $P x k.bin same.bin o.bin
chk "missing key file"    $P e nokey.bin same.bin o.bin
chk "missing input file"  $P e k.bin nofile.bin o.bin
chk "unwritable output"   $P e k.bin same.bin /nonexistent_dir/o.bin

echo "== 6. Submission hygiene"
grep -q '\[Full name\]' studentID.c && bad "team header still has placeholders" || ok "team names/IDs filled in"
grep -qE 'int[[:space:]]+main[[:space:]]*\([[:space:]]*int[[:space:]]+argc[[:space:]]*,[[:space:]]*char[[:space:]]*\*\*[[:space:]]*argv' studentID.c && ok "main(int argc, char **argv)" || bad "main signature differs from brief"
grep -q '#include[[:space:]]*"' studentID.c && bad "still has local #include \"...\"" || ok "no local includes"

echo; echo "RESULT: $PASS passed, $FAIL failed"
[[ $FAIL -eq 0 ]]
