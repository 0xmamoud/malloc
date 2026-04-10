#include "test_free.h"

static void test_free_marks_block_as_free(void) {
  void *ptr;
  void *guard;
  t_block *block;

  ptr = malloc(64);
  guard = malloc(32);

  block = block_from_ptr(ptr);
  free(ptr);
  test_assert(block->is_free,
              "free invariants: freeing a block marks it as free");
  free(guard);
}

static void test_adjacent_freed_blocks_are_coalesced(void) {
  unsigned char *left;
  unsigned char *middle_a;
  unsigned char *middle_b;
  unsigned char *right;
  unsigned char *merged;

  left = malloc(32);
  middle_a = malloc(48);
  middle_b = malloc(64);
  right = malloc(80);

  memset(left, 0x51, 32);
  memset(right, 0xA4, 80);
  free(middle_a);
  free(middle_b);
  merged = malloc(90);
  test_assert(
      merged == middle_a,
      "free invariants: adjacent free range is reused from first block");
  test_assert(test_region_matches(left, 32, 0x51),
              "free invariants: left live block survives coalescing");
  test_assert(test_region_matches(right, 80, 0xA4),
              "free invariants: right live block survives coalescing");
}

static void test_non_adjacent_frees_preserve_live_blocks(void) {
  unsigned char *a;
  unsigned char *b;
  unsigned char *c;
  unsigned char *d;
  unsigned char *reuse_a;
  unsigned char *reuse_c;

  a = malloc(80);
  b = malloc(96);
  c = malloc(112);
  d = malloc(128);

  memset(b, 0x27, 96);
  memset(d, 0x4C, 128);
  free(a);
  free(c);
  reuse_a = malloc(80);
  reuse_c = malloc(112);
  test_assert(reuse_a != NULL && reuse_c != NULL,
              "free invariants: non-adjacent free slots remain reusable");
  test_assert(test_region_matches(b, 96, 0x27),
              "free invariants: live middle block survives separated frees");
  test_assert(test_region_matches(d, 128, 0x4C),
              "free invariants: live tail block survives separated frees");
}

static void test_middle_large_free_updates_large_list_only(void) {
  unsigned char *first;
  unsigned char *middle;
  unsigned char *last;
  size_t before_count;

  first = malloc(SMALL_MAX + 64);
  middle = malloc(SMALL_MAX + 128);
  last = malloc(SMALL_MAX + 256);

  memset(first, 0x11, SMALL_MAX + 64);
  memset(last, 0x33, SMALL_MAX + 256);
  before_count = test_count_heaps(g_malloc.large);
  test_assert(before_count == 3,
              "free invariants: large allocations create three tracked heaps");

  free(middle);
  test_assert(test_count_heaps(g_malloc.large) == before_count - 1,
              "free invariants: freeing middle large block removes one heap");
  test_assert(test_region_matches(first, SMALL_MAX + 64, 0x11),
              "free invariants: first large allocation remains intact");
  test_assert(test_region_matches(last, SMALL_MAX + 256, 0x33),
              "free invariants: last large allocation remains intact");
}

static void test_releasing_one_tiny_heap_keeps_other_tiny_heaps_valid(void) {
  void *ptrs[256];
  int keep_flags[256];
  size_t i;
  size_t tiny_heap_count;
  t_heap *heap_to_release;

  i = 0;
  while (i < 256) {
    ptrs[i] = malloc(TINY_MAX);
    keep_flags[i] = 1;
    i++;
  }
  tiny_heap_count = test_count_heaps(g_malloc.tiny);

  test_assert(tiny_heap_count >= 2,
              "free invariants: tiny workload spans multiple heaps");

  heap_to_release = heap_get_from_block(block_from_ptr(ptrs[0]));
  i = 0;
  while (i < 256) {
    if (heap_get_from_block(block_from_ptr(ptrs[i])) == heap_to_release)
      keep_flags[i] = 0;
    i++;
  }

  i = 0;
  while (i < 256) {
    if (!keep_flags[i])
      free(ptrs[i]);
    i++;
  }

  test_assert(
      test_count_heaps(g_malloc.tiny) == tiny_heap_count - 1,
      "free invariants: freeing one full tiny heap removes only that heap");
}

void run_free_invariant_tests(void) {
  printf("[free invariants]\n");
  test_reset_allocator();
  test_free_marks_block_as_free();
  test_reset_allocator();
  test_adjacent_freed_blocks_are_coalesced();
  test_reset_allocator();
  test_non_adjacent_frees_preserve_live_blocks();
  test_reset_allocator();
  test_middle_large_free_updates_large_list_only();
  test_reset_allocator();
  test_releasing_one_tiny_heap_keeps_other_tiny_heaps_valid();
}
