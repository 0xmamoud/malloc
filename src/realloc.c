#include "malloc.h"

static size_t min_size(size_t a, size_t b) {
  if (a < b)
    return a;
  return b;
}

void *realloc(void *ptr, size_t size) {
  if (!ptr)
    return malloc(size);

  if (size == 0) {
    free(ptr);
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

  void *new_ptr = malloc(size);
  if (!new_ptr)
    return NULL;

  ft_memcpy(new_ptr, ptr, min_size(block->size, size));
  free(ptr);
  return new_ptr;
}
