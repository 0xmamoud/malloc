#include "test_malloc.h"

static void test_returned_pointers_are_aligned(void) {
  void *ptrs[5];
  size_t sizes[5];
  size_t i;
  int all_valid = 1;

  sizes[0] = 1;
  sizes[1] = 7;
  sizes[2] = 8;
  sizes[3] = TINY_MAX + 1;
  sizes[4] = SMALL_MAX + 1;
  i = 0;
  while (i < 5) {
    ptrs[i] = malloc(sizes[i]);
    if (!ptrs[i] || !test_is_aligned(ptrs[i]))
      all_valid = 0;
    i++;
  }
  test_assert(all_valid, "invariants: returned pointers respect ALIGNMENT");
}

static void test_active_regions_do_not_overlap(void) {
  unsigned char *a;
  unsigned char *b;

  a = malloc(96);
  b = malloc(128);
  test_assert(a && b, "invariants: overlap test allocations succeed");

  memset(a, 0x5A, 96);
  memset(b, 0xC7, 128);
  test_assert(test_regions_do_not_overlap(a, 96, b, 128),
              "invariants: active user regions do not overlap");
  test_assert(
      test_region_matches(a, 96, 0x5A),
      "invariants: first region remains unchanged after writing second");
  test_assert(
      test_region_matches(b, 128, 0xC7),
      "invariants: second region remains unchanged after writing first");
}

static void test_zone_routing_matches_thresholds(void) {
  void *tiny_ptr;
  void *small_ptr;
  void *large_ptr;
  t_block *tiny_block;
  t_block *small_block;
  t_block *large_block;

  tiny_ptr = malloc(TINY_MAX);
  small_ptr = malloc(TINY_MAX + 1);
  large_ptr = malloc(SMALL_MAX + 1);
  test_assert(tiny_ptr && small_ptr && large_ptr,
              "invariants: zone routing allocations succeed");

  tiny_block = block_from_ptr(tiny_ptr);
  small_block = block_from_ptr(small_ptr);
  large_block = block_from_ptr(large_ptr);
  test_assert(tiny_block->size <= TINY_MAX,
              "invariants: tiny request stays in tiny-sized block");
  test_assert(small_block->size > TINY_MAX && small_block->size <= SMALL_MAX,
              "invariants: small request stays in small-sized block");
  test_assert(large_block->size > SMALL_MAX,
              "invariants: large request stays in large-sized block");
}

void run_invariant_tests(void) {
  printf("[invariants]\n");
  test_returned_pointers_are_aligned();
  test_active_regions_do_not_overlap();
  test_zone_routing_matches_thresholds();
}
