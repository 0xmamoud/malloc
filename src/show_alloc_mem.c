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

static void print_block_hexdump(t_block *block) {
  unsigned char *data;
  size_t i;
  size_t line_size;

  data = (unsigned char *)block_to_ptr(block);
  i = 0;
  while (i < block->size) {
    ft_putstr("    ");
    line_size = 0;
    while (i < block->size && line_size < 16) {
      if (data[i] < 16)
        ft_putchar('0');
      ft_puthex(data[i]);
      ft_putchar(' ');
      i++;
      line_size++;
    }
    ft_putchar('\n');
  }
}

static size_t print_heap_blocks(t_heap *heap, bool extended) {
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
      if (extended)
        print_block_hexdump(block);
      total += block->size;
    }
    block = block->next;
  }
  return total;
}

static size_t print_zone(const char *name, t_heap *head, bool extended) {
  t_heap *heap = NULL;
  uintptr_t min_addr = 0;
  size_t total = 0;

  heap = find_next_heap(head, min_addr);
  while (heap) {
    ft_putstr(name);
    ft_putstr(" : ");
    ft_putaddr(heap);
    ft_putchar('\n');
    total += print_heap_blocks(heap, extended);
    min_addr = (uintptr_t)heap;
    heap = find_next_heap(head, min_addr);
  }
  return total;
}

static void show_alloc_mem_impl(bool extended) {
  size_t total = 0;

  pthread_mutex_lock(&g_malloc_mutex);
  total += print_zone("TINY", g_malloc.tiny, extended);
  total += print_zone("SMALL", g_malloc.small, extended);
  total += print_zone("LARGE", g_malloc.large, extended);
  ft_putstr("Total : ");
  ft_putnbr(total);
  ft_putendl(" bytes");
  pthread_mutex_unlock(&g_malloc_mutex);
}

void show_alloc_mem(void) { show_alloc_mem_impl(false); }

void show_alloc_mem_ex(void) { show_alloc_mem_impl(true); }
