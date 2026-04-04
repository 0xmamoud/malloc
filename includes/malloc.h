#ifndef MALLOC_H
#define MALLOC_H
#include <stdbool.h>
#include <stdlib.h>
#include <sys/mman.h>

#define TINY_HEAP_SIZE (4 * getpagesize())
#define TINY_MAX (TINY_HEAP_SIZE / 128)

#define SMALL_HEAP_SIZE (16 * getpagesize())
#define SMALL_MAX (SMALL_HEAP_SIZE / 128)

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

t_malloc *get_malloc_instance(void);
t_block *get_first_block(t_heap *heap);

// utils
void *ft_memset(void *b, int c, size_t len);
void ft_bzero(void *s, size_t n);
void *ft_memcpy(void *dst, const void *src, size_t n);
size_t ft_strlen(const char *s);

#endif
