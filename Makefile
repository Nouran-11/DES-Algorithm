# Development build only. The graded build is:
#   gcc -O3 studentID.c -o studentID   (see `make submission`)
CC      = gcc
CFLAGS  = -O2 -Wall -Wextra -std=c11
ID     ?= studentID

.PHONY: all test test-tables test-keys submission verify clean

all: test

test: test-tables test-keys

test-tables: build/test_tables
	./build/test_tables
build/test_tables: tests/test_tables.c src/des_tables.c src/des_tables.h src/bit_ops.h | build
	$(CC) $(CFLAGS) tests/test_tables.c src/des_tables.c -o $@

test-keys: build/test_key_schedule
	./build/test_key_schedule
build/test_key_schedule: tests/test_key_schedule.c src/key_schedule.c src/key_schedule.h | build
	$(CC) $(CFLAGS) tests/test_key_schedule.c src/key_schedule.c -o $@

# Merge everything into build/$(ID).c (needs des.c, file_ecb.c, main.c to exist)
submission:
	bash scripts/merge.sh $(ID)

verify: submission
	bash scripts/verify_submission.sh build/$(ID).c

build:
	mkdir -p build

clean:
	rm -rf build
