#include "malloc.h"

static size_t min_size(size_t a, size_t b) {
  if (a < b)
    return a;
  return b;
}

void *realloc(void *ptr, size_t size) {
  void *new_ptr;

  pthread_mutex_lock(&g_malloc_mutex);
  new_ptr = realloc_impl(ptr, size);
  pthread_mutex_unlock(&g_malloc_mutex);
  return new_ptr;
}

void *realloc_impl(void *ptr, size_t size) {
  void *new_ptr;

  if (!ptr)
    return malloc_impl(size);

  if (size == 0) {
    free_impl(ptr);
    return NULL;
  }

  if (size > SIZE_MAX - (ALIGNMENT - 1))
    return NULL;

  size = align_size(size);

  t_block *block = block_from_ptr(ptr);
  if (block->size >= size) {
    block_split(block, size);
    return ptr;
  }

  t_block *next_block = block->next;
  if (next_block && next_block->is_free &&
      block->size + sizeof(t_block) + next_block->size >= size) {

    block->size += sizeof(t_block) + next_block->size;
    block->next = next_block->next;
    if (block->next)
      block->next->prev = block;
    block_split(block, size);
    return ptr;
  }

  new_ptr = malloc_impl(size);
  if (!new_ptr)
    return NULL;

  ft_memcpy(new_ptr, ptr, min_size(block->size, size));
  free_impl(ptr);
  return new_ptr;
}
