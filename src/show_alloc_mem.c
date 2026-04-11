#include "malloc.h"

static t_heap *find_next_heap(t_heap *head, uintptr_t min_addr) {
  t_heap *current = head;
  t_heap *next = NULL;

  while (current) {
    if ((uintptr_t)current > min_addr &&
        (!next || (uintptr_t)current < (uintptr_t)next))
      next = current;
    current = current->next;
  }
  return next;
}

static size_t print_heap_blocks(t_heap *heap) {
  t_block *block = block_get_head(heap);
  size_t total = 0;

  while (block) {
    if (!block->is_free) {
      ft_putaddr(block_to_ptr(block));
      ft_putstr(" - ");
      ft_putaddr((char *)block_to_ptr(block) + block->size);
      ft_putstr(" : ");
      ft_putnbr(block->size);
      ft_putendl(" bytes");
      total += block->size;
    }
    block = block->next;
  }
  return total;
}

static size_t print_zone(const char *name, t_heap *head) {
  t_heap *heap = NULL;
  uintptr_t min_addr = 0;
  size_t total = 0;

  heap = find_next_heap(head, min_addr);
  while (heap) {
    ft_putstr(name);
    ft_putstr(" : ");
    ft_putaddr(heap);
    ft_putchar('\n');
    total += print_heap_blocks(heap);
    min_addr = (uintptr_t)heap;
    heap = find_next_heap(head, min_addr);
  }
  return total;
}

void show_alloc_mem(void) {
  size_t total = 0;

  pthread_mutex_lock(&g_malloc_mutex);
  total += print_zone("TINY", g_malloc.tiny);
  total += print_zone("SMALL", g_malloc.small);
  total += print_zone("LARGE", g_malloc.large);
  ft_putstr("Total : ");
  ft_putnbr(total);
  ft_putendl(" bytes");
  pthread_mutex_unlock(&g_malloc_mutex);
}
