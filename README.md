# malloc

A dynamic memory allocator.

This project reimplements the standard `malloc`, `free`, and `realloc` functions and can be injected into existing programs with `LD_PRELOAD`.

## Features

- `malloc`, `free`, and `realloc`
- `TINY`, `SMALL`, and `LARGE` allocation zones
- 8-byte alignment
- block splitting
- block merging
- defragmentation through coalescing
- thread safety with `pthread_mutex_t`
- `show_alloc_mem()`
- `show_alloc_mem_ex()`
- debug logs with `MALLOC_DEBUG`

## How It Works

The allocator splits allocations into three categories:

- `TINY`
- `SMALL`
- `LARGE`

`TINY` and `SMALL` allocations are stored inside preallocated heaps.
`LARGE` allocations are handled separately.

Each heap contains allocated blocks managed by the allocator.

When a free block is large enough, it can be split to satisfy a smaller allocation.
When adjacent blocks become free, they are merged back together.
This reduces fragmentation and helps reuse memory efficiently.

`realloc()` tries to resize a block in place when possible.
If it cannot, it allocates a new block, copies the data, frees the old one, and returns the new pointer.

## Thread Safety

The allocator is protected by a global mutex, making it safe to use in multi-threaded programs.

## Debug

Set `MALLOC_DEBUG=1` to enable debug logs for:

- `malloc`
- `free`
- `realloc`

Example:

```sh
MALLOC_DEBUG=1 LD_PRELOAD=./libft_malloc.so ./your_program
```

## Build

```sh
make
```

This builds:

- `libft_malloc_$HOSTTYPE.so`
- `libft_malloc.so` as a symlink

## Usage

Run a program with your allocator using `LD_PRELOAD`:

```sh
LD_PRELOAD=./libft_malloc.so ./your_program
```

Example:

```sh
LD_PRELOAD=./libft_malloc.so ls
```

## Introspection

The allocator provides:

- `show_alloc_mem()`
- `show_alloc_mem_ex()`

`show_alloc_mem()` displays allocated memory by zone.
`show_alloc_mem_ex()` displays the same information with an extended hexadecimal dump.
