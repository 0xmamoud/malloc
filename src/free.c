#include "malloc.h"

void free(void *ptr) {
  pthread_mutex_lock(&g_malloc_mutex);
  free_impl(ptr);
  pthread_mutex_unlock(&g_malloc_mutex);
}

void free_impl(void *ptr) {
  if (!ptr)
    return;

  t_block *block = block_from_ptr(ptr);
  block->is_free = true;

  t_heap *heap = heap_get_from_block(block);
  block_coalesce(block);

  t_block *block_head = block_get_head(heap);
  if (block_head && block_head->is_free && !block_head->next) {
    t_heap *heap_head = heap_get_head(heap);

    if (heap_head == g_malloc.tiny)
      heap_remove(&g_malloc.tiny, heap);
    else if (heap_head == g_malloc.small)
      heap_remove(&g_malloc.small, heap);
    else if (heap_head == g_malloc.large)
      heap_remove(&g_malloc.large, heap);

    heap_free(heap);
  }
}
