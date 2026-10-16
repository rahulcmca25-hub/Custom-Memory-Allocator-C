# Custom Memory Allocator in C

A custom implementation of dynamic memory management in C, providing `malloc()`, `calloc()`, `realloc()`, and `free()` with built-in debugging, integrity checking, and leak-detection features.

This project is designed to demonstrate how a heap allocator works internally, including block metadata, splitting, coalescing, and safe memory diagnostics.

## Features

- Custom implementations of:
  - `malloc()`
  - `calloc()`
  - `realloc()`
  - `free()`
- Block splitting for efficient use of heap space
- Block coalescing to reduce external fragmentation
- Heap integrity checking
- Memory leak detection
- Double-free detection
- Invalid pointer detection
- Heap usage statistics
- Debug utilities for allocator diagnostics

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

## Getting Started

### Prerequisites

- GCC or Clang
- Make

### Build

Clone the repository:

```bash
git clone [https://github.com/rahulcmca25-hub/Custom-Memory-Allocator-C.git](https://github.com/rahulcmca25-hub/Custom-Memory-Allocator-C.git)
cd Custom-Memory-Allocator-C
```

Build the project:

```bash
make
```

### Run Tests

```bash
./test
```

### Clean Build Artifacts

```bash
make clean
```

## Usage Example

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

    my_free(arr);

    return 0;
}
```

> Replace `my_malloc`, `my_realloc`, and `my_free` with the actual function names defined in `include/Allocator.h`.

## How It Works

The allocator manages a custom heap using memory blocks. Each allocation stores metadata that allows the allocator to track block size, allocation status, and neighboring blocks.

### Block Splitting

When a requested allocation is smaller than an available free block, the allocator splits the block. One part is returned to the caller, while the remaining part stays in the free list.

### Block Coalescing

When memory is freed, adjacent free blocks are merged into a larger block. This helps reduce fragmentation and improves the chances of satisfying future allocation requests.

### Debugging and Safety Checks

The allocator includes checks for common memory-management mistakes:

- Freeing a pointer that was not allocated by the allocator
- Freeing the same pointer more than once
- Detecting memory that was allocated but never freed
- Checking heap metadata for corruption or inconsistency

## Allocator Statistics

The debug module can report information such as:

- Total allocated memory
- Total free memory
- Number of active allocations
- Number of detected leaks
- Heap integrity status

## Testing

The test suite in `tests/test_allocator.c` validates core allocator behavior, including allocation, reallocation, freeing, and error detection.

Run the tests with:

```bash
make
./test
```

## Possible Improvements

- Add thread-safety using mutexes
- Implement aligned allocations
- Add segregated free lists for different block sizes
- Support mmap-backed large allocations
- Add Valgrind and AddressSanitizer test targets
- Improve heap visualization and detailed diagnostics


## Author

**Rahul Chadar**  
GitHub: [rahulcmca25-hub](https://github.com/rahulcmca25-hub)
