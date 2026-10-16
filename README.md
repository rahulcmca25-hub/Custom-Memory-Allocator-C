# Custom Memory Allocator in C

A custom dynamic memory allocator implemented in C. It provides custom versions of `malloc()`, `calloc()`, `realloc()`, and `free()` using a first-fit allocation strategy.

The allocator manages its own heap using linked memory blocks and includes block splitting, block coalescing, heap-integrity checking, double-free detection, invalid-pointer detection, and basic allocation statistics.

## Features

- Custom implementations of:
  - `malloc_clone()`
  - `calloc_clone()`
  - `realloc_clone()`
  - `free_clone()`
- First-fit free-block search
- Block splitting
- Block coalescing
- 8-byte memory alignment
- Heap integrity checking using magic numbers
- Double-free detection
- Invalid-pointer detection
- Basic allocator statistics
- Simple test suite

## Allocation Strategy

This allocator uses the **first-fit strategy**.

When `malloc_clone()` is called, the allocator searches the linked list of blocks from the beginning and selects the **first free block** whose size is greater than or equal to the requested size.

If no suitable free block is found, the allocator requests more memory from the operating system using `sbrk()`.

### Why First-Fit?

First-fit is simple and fast because it stops searching as soon as it finds a usable block. It is a common strategy for educational and basic heap allocators.

## How It Works

Each allocation is stored as a block containing metadata followed by user memory:

```text
+----------------------+----------------------------+
| Block metadata       | User memory                |
+----------------------+----------------------------+
^                      ^
block_t                pointer returned to user
```

The block metadata contains:

- Usable block size
- Allocation status: free or allocated
- Pointer to the previous block
- Pointer to the next block
- Magic number for block validation

### Block Splitting

If a free block is much larger than the requested allocation size, it is split into two parts:

1. An allocated block of the requested size  
2. A remaining free block  

This reduces wasted space inside large free blocks.

### Block Coalescing

When a block is freed, the allocator checks whether neighboring blocks are also free. If they are, it merges them into one larger free block.

This reduces external fragmentation and makes larger allocations more likely to succeed later.

### Safety Checks

Before freeing or reallocating a pointer, the allocator checks its metadata. It can detect:

- Invalid or corrupted pointers
- Double-free attempts
- Heap corruption

## Project Structure

```text
.
├── include/
│   ├── Allocator.h
│   └── Allocator_internal.h
├── src/
│   ├── Allocator.c
│   └── Debug.c
├── tests/
│   └── test_allocator.c
├── Makefile
├── README.md
└── .gitignore
```

## Build and Run

### Prerequisites

- GCC or Clang
- Make
- Linux or another POSIX-compatible system, since the allocator uses `sbrk()`

### Clone the Repository

```bash
git clone [https://github.com/rahulcmca25-hub/Custom-Memory-Allocator-C.git](https://github.com/rahulcmca25-hub/Custom-Memory-Allocator-C.git)
cd Custom-Memory-Allocator-C
```

### Build

```bash
make
```

### Run Tests

```bash
./test
```

### Clean Build Files

```bash
make clean
```

## Usage

```c
#include "Allocator.h"
#include <stdio.h>

int main(void) {
    int *arr = (int *)malloc_clone(10 * sizeof(int));

    if (arr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    for (int i = 0; i < 10; i++) {
        arr[i] = i * i;
    }

    arr = (int *)realloc_clone(arr, 20 * sizeof(int));

    if (arr == NULL) {
        fprintf(stderr, "Memory reallocation failed\n");
        return 1;
    }

    free_clone(arr);

    return 0;
}
```

### Using `calloc_clone()`

```c
char *buffer = (char *)calloc_clone(100, sizeof(char));

if (buffer != NULL) {
    /* The allocated memory is initialized to zero. */
    free_clone(buffer);
}
```

## Function Behavior

| Function | Behavior |
|---|---|
| `malloc_clone(size)` | Allocates aligned memory of the requested size |
| `calloc_clone(n, size)` | Allocates memory for `n` elements and initializes it to zero |
| `realloc_clone(ptr, size)` | Resizes an existing allocation; allocates and copies data when needed |
| `free_clone(ptr)` | Marks a block as free and merges adjacent free blocks |

## Current Limitations

- Not thread-safe
- Uses `sbrk()`, so it is platform-dependent
- Uses first-fit search, which may lead to fragmentation in some workloads
- Does not return memory at the end of the heap back to the operating system
- `realloc_clone()` allocates a new block and copies data instead of expanding the block in place

## Future Improvements

- Add thread-safety using mutexes
- Implement best-fit or next-fit allocation strategies
- Add aligned allocation support
- Use `mmap()` for large allocations
- Add detailed memory-leak reports
- Add Valgrind and AddressSanitizer test targets

## Author

**Rahul Chadar**  
GitHub: [rahulcmca25-hub](https://github.com/rahulcmca25-hub)
