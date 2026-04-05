#ifndef MALLOC_H
#define MALLOC_H
#include <stdbool.h>
#include <stdlib.h>
#include <sys/mman.h>

#define TINY_HEAP_SIZE (4 * getpagesize())
#define TINY_MAX (TINY_HEAP_SIZE / 128)

#define SMALL_HEAP_SIZE (16 * getpagesize())
#define SMALL_MAX (SMALL_HEAP_SIZE / 128)

#define MIN_PAYLOAD 1

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
  size_t free_size;
} t_heap;

typedef struct s_malloc {
  t_heap *tiny;
  t_heap *small;
  t_heap *large;
} t_malloc;

// malloc management
t_malloc *malloc_get_instance(void);

// heap management
t_heap *heap_new(size_t size);
void heap_add(t_heap **head, t_heap *new_heap);
void heap_remove(t_heap **head, t_heap *heap);
void heap_free(t_heap *heap);

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

#endif
