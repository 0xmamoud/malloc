#include "malloc.h"
#include <stddef.h>

t_heap *heap_new(size_t size) {
  t_heap *heap = mmap(NULL, size, PROT_READ | PROT_WRITE,
                      MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

  if (heap == MAP_FAILED)
    return NULL;

  heap->prev = NULL;
  heap->next = NULL;
  heap->total_size = size;
  heap->free_size = size - sizeof(t_heap);

  return heap;
}
