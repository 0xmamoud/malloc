#ifndef MALLOC_H
#define MALLOC_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#define TINY_HEAP_SIZE ((size_t)4 * (size_t)getpagesize())
#define TINY_MAX (TINY_HEAP_SIZE / (size_t)128)

#define SMALL_HEAP_SIZE ((size_t)16 * (size_t)getpagesize())
#define SMALL_MAX (SMALL_HEAP_SIZE / (size_t)128)

#define ALIGNMENT 8
#define MIN_PAYLOAD 8

typedef struct s_block {
  struct s_block *prev;
  struct s_block *next;
  size_t size;
  bool is_free;
} t_block;

typedef struct s_heap {
  struct s_heap *prev;
  struct s_heap *next;
  size_t total_size;
} t_heap;

typedef struct s_malloc {
  t_heap *tiny;
  t_heap *small;
  t_heap *large;
} t_malloc;

extern t_malloc g_malloc;

// allocation management
void *malloc(size_t size);
void *malloc_alloc_from_zone(t_heap **heap, size_t heap_size, size_t size);
void *malloc_alloc_large(size_t size);
void free(void *ptr);
void show_alloc_mem(void);
void *realloc(void *ptr, size_t size);

// heap management
t_heap *heap_new(size_t size);
void heap_add(t_heap **head, t_heap *new_heap);
void heap_remove(t_heap **head, t_heap *heap);
void heap_free(t_heap *heap);
t_heap *heap_get_from_block(t_block *block);
t_heap *heap_get_head(t_heap *heap);

// block management
t_block *block_get_head(t_heap *heap);
t_block *block_new(t_heap *heap, size_t size);
t_block *block_first_fit(t_heap *heap, size_t size);
void block_add(t_block **head, t_block *new_block);
void block_remove(t_block **head, t_block *block);
void block_split(t_block *block, size_t size);
void block_coalesce(t_block *block);

// utils
void *ft_memset(void *b, int c, size_t len);
void ft_bzero(void *s, size_t n);
void *ft_memcpy(void *dst, const void *src, size_t n);
size_t ft_strlen(const char *s);
size_t align_size(size_t size);
void ft_putchar(char c);
void ft_putstr(const char *s);
void ft_putnbr(size_t n);
void ft_puthex(uintptr_t value);
void ft_putaddr(void *ptr);
void ft_putendl(const char *s);
void *block_to_ptr(t_block *block);
t_block *block_from_ptr(void *ptr);

#endif
