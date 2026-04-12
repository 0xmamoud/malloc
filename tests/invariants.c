#include "test.h"

static void test_returned_pointers_are_aligned(void) {
  void *ptrs[5];
  size_t sizes[5];
  size_t i;
  int all_valid;

  sizes[0] = 1;
  sizes[1] = 7;
  sizes[2] = 8;
  sizes[3] = TINY_MAX + 1;
  sizes[4] = SMALL_MAX + 1;
  all_valid = 1;
  i = 0;
  while (i < 5) {
    ptrs[i] = malloc(sizes[i]);
    if (!ptrs[i] || !test_is_aligned(ptrs[i]))
      all_valid = 0;
    i++;
  }
  test_assert(all_valid, "invariants: returned pointers respect ALIGNMENT");

  i = 0;
  while (i < 5) {
    free(ptrs[i]);
    i++;
  }
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

  free(a);
  free(b);
}

static void test_adjacent_frees_merge_from_first_block(void) {
  unsigned char *left;
  unsigned char *middle_a;
  unsigned char *middle_b;
  unsigned char *right;
  unsigned char *merged;

  left = malloc(32);
  middle_a = malloc(48);
  middle_b = malloc(64);
  right = malloc(80);
  test_assert(left && middle_a && middle_b && right,
              "invariants: coalescing test allocations succeed");

  memset(left, 0x51, 32);
  memset(right, 0xA4, 80);
  free(middle_a);
  free(middle_b);
  merged = malloc(90);
  test_assert(merged == middle_a,
              "invariants: adjacent free range is reused from first block");
  test_assert(test_region_matches(left, 32, 0x51),
              "invariants: left live block survives coalescing");
  test_assert(test_region_matches(right, 80, 0xA4),
              "invariants: right live block survives coalescing");

  free(left);
  free(merged);
  free(right);
}

static void test_freeing_middle_large_keeps_other_large_alive(void) {
  unsigned char *first;
  unsigned char *middle;
  unsigned char *last;

  first = malloc(SMALL_MAX + 64);
  middle = malloc(SMALL_MAX + 128);
  last = malloc(SMALL_MAX + 256);
  test_assert(first && middle && last, "invariants: large allocations succeed");

  memset(first, 0x11, SMALL_MAX + 64);
  memset(last, 0x33, SMALL_MAX + 256);
  free(middle);
  test_assert(test_region_matches(first, SMALL_MAX + 64, 0x11),
              "invariants: first large allocation remains intact");
  test_assert(test_region_matches(last, SMALL_MAX + 256, 0x33),
              "invariants: last large allocation remains intact");

  free(first);
  free(last);
}

void run_invariant_tests(void) {
  printf("[invariants]\n");
  run_in_child(test_returned_pointers_are_aligned);
  run_in_child(test_active_regions_do_not_overlap);
  run_in_child(test_adjacent_frees_merge_from_first_block);
  run_in_child(test_freeing_middle_large_keeps_other_large_alive);
}
