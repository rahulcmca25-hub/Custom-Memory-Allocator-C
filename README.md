# Custom Memory Allocator in C

A custom implementation of:

- malloc()
- calloc()
- realloc()
- free()

Features:

- Block splitting
- Block coalescing
- Heap integrity checking
- Memory leak detection
- Double free detection
- Invalid pointer detection
- Heap statistics

## Project Structure

```text
include/
    Allocator.h
    Allocator_internal.h

src/
    Allocator.c
    Debug.c

tests/
    test_allocator.c

Makefile
```

## Build

```bash
make
```

## Run

```bash
./test
```

## Clean

```bash
make clean
```

## Author

Rahul Chadar