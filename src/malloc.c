#include "malloc.h"

t_malloc g_malloc;
pthread_mutex_t g_malloc_mutex = PTHREAD_MUTEX_INITIALIZER;

void *malloc(size_t size) {
  void *ptr;

  pthread_mutex_lock(&g_malloc_mutex);
  debug_init();
  ptr = malloc_impl(size);
  debug_malloc(size, ptr);
  pthread_mutex_unlock(&g_malloc_mutex);

  return ptr;
}

void *malloc_impl(size_t size) {

  if (size == 0)
    return NULL;

  if (size > SIZE_MAX - (ALIGNMENT - 1))
    return NULL;

  size_t aligned_size = align_size(size);
  if (aligned_size <= TINY_MAX) {
    return malloc_alloc_from_zone(&g_malloc.tiny, TINY_HEAP_SIZE, aligned_size);
  } else if (aligned_size <= SMALL_MAX) {
    return malloc_alloc_from_zone(&g_malloc.small, SMALL_HEAP_SIZE,
                                  aligned_size);
  } else {
    return malloc_alloc_large(aligned_size);
  }

  return NULL;
}

void *malloc_alloc_from_zone(t_heap **heap, size_t heap_size, size_t size) {
  t_block *block;
  t_heap *current_heap = *heap;

  while (current_heap) {
    block = block_first_fit(current_heap, size);
    if (block) {
      block->is_free = false;
      block_split(block, size);
      return block_to_ptr(block);
    }

    current_heap = current_heap->next;
  }

  t_heap *new_heap = heap_new(heap_size);
  if (!new_heap)
    return NULL;

  heap_add(heap, new_heap);

  block = block_new(new_heap,
                    new_heap->total_size - sizeof(t_heap) - sizeof(t_block));
  if (!block) {
    heap_remove(heap, new_heap);
    heap_free(new_heap);
    return NULL;
  }

  block->is_free = false;
  block_split(block, size);

  return block_to_ptr(block);
}

void *malloc_alloc_large(size_t size) {
  if (size > SIZE_MAX - sizeof(t_heap) - sizeof(t_block))
    return NULL;

  size_t total_size = sizeof(t_heap) + sizeof(t_block) + size;

  t_heap *heap = heap_new(total_size);
  if (!heap)
    return NULL;

  heap_add(&g_malloc.large, heap);

  t_block *block = block_new(heap, size);
  if (!block) {
    heap_remove(&g_malloc.large, heap);
    heap_free(heap);
    return NULL;
  }

  block->is_free = false;

  return block_to_ptr(block);
}
