#include "test.h"

static void test_free_heap_list(t_heap *heap) {
  t_heap *next;

  while (heap) {
    next = heap->next;
    heap_free(heap);
    heap = next;
  }
}

void test_reset_allocator(void) {
  test_free_heap_list(g_malloc.tiny);
  test_free_heap_list(g_malloc.small);
  test_free_heap_list(g_malloc.large);
  g_malloc.tiny = NULL;
  g_malloc.small = NULL;
  g_malloc.large = NULL;
}

int test_is_aligned(void *ptr) { return ((uintptr_t)ptr % ALIGNMENT) == 0; }

int test_region_matches(const unsigned char *ptr, size_t size,
                        unsigned char value) {
  size_t i;

  i = 0;
  while (i < size) {
    if (ptr[i] != value)
      return 0;
    i++;
  }
  return 1;
}

int test_regions_do_not_overlap(const unsigned char *a, size_t a_size,
                                const unsigned char *b, size_t b_size) {
  const unsigned char *a_end;
  const unsigned char *b_end;

  a_end = a + a_size;
  b_end = b + b_size;
  return (a_end <= b || b_end <= a);
}

size_t test_count_heaps(t_heap *head) {
  size_t count;

  count = 0;
  while (head) {
    count++;
    head = head->next;
  }
  return count;
}
