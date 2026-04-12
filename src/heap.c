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

  return heap;
}

t_heap *heap_get_from_block(t_block *block) {
  if (!block)
    return NULL;

  while (block->prev)
    block = block->prev;

  return (t_heap *)((char *)block - sizeof(t_heap));
}

t_heap *heap_get_head(t_heap *heap) {
  if (!heap)
    return NULL;

  while (heap->prev)
    heap = heap->prev;

  return heap;
}

void heap_add(t_heap **head, t_heap *new_heap) {
  if (!new_heap)
    return;

  if (!*head) {
    *head = new_heap;
    return;
  }

  t_heap *current = *head;
  while (current->next != NULL) {
    current = current->next;
  }

  current->next = new_heap;
  new_heap->prev = current;
}

void heap_remove(t_heap **head, t_heap *heap) {
  if (!heap)
    return;

  if (heap->prev)
    heap->prev->next = heap->next;
  else
    *head = heap->next;

  if (heap->next)
    heap->next->prev = heap->prev;
}

void heap_free(t_heap *heap) {
  if (!heap)
    return;

  if (munmap(heap, heap->total_size) == -1) {
    write(STDERR_FILENO, "Error: munmap failed\n", 22);
    _exit(EXIT_FAILURE);
  }
}
