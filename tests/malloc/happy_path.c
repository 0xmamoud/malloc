#include "test_malloc.h"

static void test_small_allocations_are_usable(void) {
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
}

static void test_multiple_allocations_are_distinct(void) {
  void *ptrs[6];
  size_t i;
  size_t j;
  int all_distinct = 1;

  i = 0;
  while (i < 6) {
    ptrs[i] = malloc((i + 1) * 24);
    if (ptrs[i] == NULL)
      all_distinct = 0;
    i++;
  }
  test_assert(all_distinct, "happy_path: allocations return non-NULL pointers");

  i = 0;
  while (i < 6) {
    j = i + 1;
    while (j < 6) {
      if (ptrs[i] == ptrs[j])
        all_distinct = 0;
      j++;
    }
    i++;
  }
  test_assert(all_distinct,
              "happy_path: successive allocations produce distinct pointers");
}

static void test_sequential_blocks_keep_independent_patterns(void) {
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
}

void run_malloc_happy_path_tests(void) {
  printf("[happy_path]\n");
  test_small_allocations_are_usable();
  test_multiple_allocations_are_distinct();
  test_sequential_blocks_keep_independent_patterns();
}
