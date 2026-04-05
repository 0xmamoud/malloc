#include "malloc.h"

#define MIN_PAYLOAD 1

static void block_merge_next(t_block *block) {

  if (!block || !block->next)
    return;

  if (!block->is_free || !block->next->is_free)
    return;

  t_block *next_block = block->next;
  block->size += sizeof(t_block) + next_block->size;
  block->next = next_block->next;

  if (block->next)
    block->next->prev = block;
}

t_block *block_get_head(t_heap *heap) {
  if (!heap)
    return NULL;

  return (t_block *)((char *)heap + sizeof(t_heap));
}

t_block *block_new(t_heap *heap, size_t size) {
  if (!heap)
    return NULL;
  if (heap->free_size < sizeof(t_block) + size)
    return NULL;

  // Calculate the address of the new block
  t_block *block =
      (t_block *)((char *)heap + heap->total_size - heap->free_size);
  block->prev = NULL;
  block->next = NULL;
  block->size = size;
  block->is_free = false;

  heap->free_size -= (sizeof(t_block) + size);

  return block;
}

t_block *block_first_fit(t_heap *heap, size_t size) {
  if (!heap)
    return NULL;

  t_block *current = block_get_head(heap);

  while (current) {
    if (current->is_free && current->size >= size)
      return current;
    current = current->next;
  }

  return NULL;
}

void block_add(t_block **head, t_block *new_block) {

  if (!head || !new_block)
    return;

  if (!*head) {
    *head = new_block;
    new_block->prev = NULL;
    new_block->next = NULL;
    return;
  }

  t_block *current = *head;
  while (current->next)
    current = current->next;

  current->next = new_block;
  new_block->prev = current;
  new_block->next = NULL;
}

void block_remove(t_block **head, t_block *block) {
  if (!head || !*head || !block)
    return;

  if (block->prev)
    block->prev->next = block->next;
  else
    *head = block->next;

  if (block->next)
    block->next->prev = block->prev;

  block->prev = NULL;
  block->next = NULL;
}

void block_split(t_block *block, size_t size) {

  if (!block || block->size < size)
    return;

  if (block->size < size + sizeof(t_block) + MIN_PAYLOAD)
    return;

  t_block *new_block = (t_block *)((char *)block + sizeof(t_block) + size);
  new_block->prev = block;
  new_block->next = block->next;
  new_block->size = block->size - size - sizeof(t_block);
  new_block->is_free = true;

  if (block->next)
    block->next->prev = new_block;

  block->next = new_block;
  block->size = size;
}

void block_coalesce(t_block *block) {
  if (!block || !block->is_free)
    return;

  while (block->prev && block->prev->is_free)
    block = block->prev;

  while (block->next && block->next->is_free)
    block_merge_next(block);
}
