#include "test.h"

static void test_basic_allocations_are_usable(void) {
  unsigned char *a;
  unsigned char *b;
  unsigned char *c;

  a = malloc(1);
  b = malloc(16);
  c = malloc(64);
  test_assert(a != NULL && b != NULL && c != NULL,
              "happy_path: allocations return non-NULL pointers");

  *a = 0x11;
  memset(b, 0x22, 16);
  memset(c, 0x33, 64);
  test_assert(*a == 0x11, "happy_path: single-byte allocation is writable");
  test_assert(test_region_matches(b, 16, 0x22),
              "happy_path: 16-byte allocation keeps its contents");
  test_assert(test_region_matches(c, 64, 0x33),
              "happy_path: 64-byte allocation keeps its contents");

  free(a);
  free(b);
  free(c);
}

static void test_neighbor_blocks_keep_independent_patterns(void) {
  unsigned char *left;
  unsigned char *middle;
  unsigned char *right;

  left = malloc(32);
  middle = malloc(48);
  right = malloc(80);
  test_assert(left && middle && right,
              "happy_path: pattern test allocations succeed");

  memset(left, 0xA1, 32);
  memset(middle, 0xB2, 48);
  memset(right, 0xC3, 80);
  test_assert(test_region_matches(left, 32, 0xA1),
              "happy_path: left block pattern remains intact");
  test_assert(test_region_matches(middle, 48, 0xB2),
              "happy_path: middle block pattern remains intact");
  test_assert(test_region_matches(right, 80, 0xC3),
              "happy_path: right block pattern remains intact");

  free(left);
  free(middle);
  free(right);
}

static void test_free_then_reuse_preserves_live_neighbors(void) {
  unsigned char *left;
  unsigned char *middle;
  unsigned char *right;
  unsigned char *replacement;

  left = malloc(32);
  middle = malloc(48);
  right = malloc(64);
  test_assert(left && middle && right,
              "happy_path: free/reuse allocations succeed");

  memset(left, 0x1A, 32);
  memset(right, 0x2B, 64);
  free(middle);
  replacement = malloc(40);
  test_assert(replacement != NULL,
              "happy_path: allocation after free succeeds");
  memset(replacement, 0x3C, 40);
  test_assert(test_region_matches(left, 32, 0x1A),
              "happy_path: left neighbor survives free and reuse");
  test_assert(test_region_matches(right, 64, 0x2B),
              "happy_path: right neighbor survives free and reuse");

  free(left);
  free(replacement);
  free(right);
}

static void test_adjacent_frees_create_usable_larger_space(void) {
  void *guard;
  void *first;
  void *second;
  void *merged;

  guard = malloc(24);
  first = malloc(48);
  second = malloc(64);
  test_assert(guard && first && second,
              "happy_path: coalescing allocations succeed");

  free(first);
  free(second);
  merged = malloc(96);
  test_assert(merged != NULL,
              "happy_path: adjacent free space serves larger request");
  test_assert(merged == first,
              "happy_path: merged allocation starts at first freed block");
  free(guard);
  free(merged);
}

static void test_realloc_grow_preserves_prefix(void) {
  unsigned char *ptr;
  size_t i;

  ptr = malloc(32);
  test_assert(ptr != NULL,
              "happy_path: realloc grow source allocation succeeds");

  i = 0;
  while (i < 32) {
    ptr[i] = (unsigned char)(0x40 + i);
    i++;
  }

  ptr = realloc(ptr, 96);
  test_assert(ptr != NULL, "happy_path: realloc grow succeeds");

  i = 0;
  while (i < 32) {
    if (ptr[i] != (unsigned char)(0x40 + i))
      break;
    i++;
  }
  test_assert(i == 32, "happy_path: realloc grow preserves existing bytes");

  free(ptr);
}

static void test_realloc_shrink_preserves_prefix(void) {
  unsigned char *ptr;
  size_t i;

  ptr = malloc(96);
  test_assert(ptr != NULL,
              "happy_path: realloc shrink source allocation succeeds");

  i = 0;
  while (i < 96) {
    ptr[i] = (unsigned char)(0x80 + (i % 32));
    i++;
  }

  ptr = realloc(ptr, 32);
  test_assert(ptr != NULL, "happy_path: realloc shrink succeeds");

  i = 0;
  while (i < 32) {
    if (ptr[i] != (unsigned char)(0x80 + (i % 32)))
      break;
    i++;
  }
  test_assert(i == 32, "happy_path: realloc shrink preserves prefix");

  free(ptr);
}

void run_happy_path_tests(void) {
  printf("[happy_path]\n");
  run_test_case(test_basic_allocations_are_usable);
  run_test_case(test_neighbor_blocks_keep_independent_patterns);
  run_test_case(test_free_then_reuse_preserves_live_neighbors);
  run_test_case(test_adjacent_frees_create_usable_larger_space);
  run_test_case(test_realloc_grow_preserves_prefix);
  run_test_case(test_realloc_shrink_preserves_prefix);
}
