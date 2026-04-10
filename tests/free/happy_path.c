#include "test_free.h"

static void test_reallocation_after_free_preserves_live_neighbors(void) {
  unsigned char *left;
  unsigned char *middle;
  unsigned char *right;
  unsigned char *replacement;

  left = malloc(32);
  middle = malloc(48);
  right = malloc(64);

  memset(left, 0x1A, 32);
  memset(right, 0x2B, 64);
  free(middle);
  replacement = malloc(40);
  test_assert(replacement != NULL,
              "free happy_path: allocation after free succeeds");
  memset(replacement, 0x3C, 40);
  test_assert(test_region_matches(left, 32, 0x1A),
              "free happy_path: left neighbor survives free and reuse");
  test_assert(test_region_matches(right, 64, 0x2B),
              "free happy_path: right neighbor survives free and reuse");
}

static void test_adjacent_frees_create_usable_larger_space(void) {
  void *guard;
  void *first;
  void *second;
  void *merged;

  guard = malloc(24);
  first = malloc(48);
  second = malloc(64);

  free(first);
  free(second);
  merged = malloc(96);
  test_assert(merged != NULL,
              "free happy_path: adjacent free space serves larger request");
  test_assert(merged == first,
              "free happy_path: merged allocation starts at first freed block");
  free(guard);
}

static void test_free_order_keeps_remaining_allocations_valid(void) {
  unsigned char *ptrs[8];
  size_t sizes[8];
  size_t i;
  int intact;

  i = 0;
  while (i < 8) {
    sizes[i] = 24 + (i * 8);
    ptrs[i] = malloc(sizes[i]);
    if (ptrs[i])
      memset(ptrs[i], (unsigned char)(0x40 + i), sizes[i]);
    i++;
  }

  free(ptrs[1]);
  free(ptrs[4]);

  intact = 1;
  i = 0;
  while (i < 8) {
    if (i != 1 && i != 4 &&
        !test_region_matches(ptrs[i], sizes[i], (unsigned char)(0x40 + i)))
      intact = 0;
    i++;
  }
  test_assert(intact,
              "free happy_path: freeing some blocks does not corrupt others");
}

void run_free_happy_path_tests(void) {
  printf("[free happy_path]\n");
  test_reallocation_after_free_preserves_live_neighbors();
  test_adjacent_frees_create_usable_larger_space();
  test_free_order_keeps_remaining_allocations_valid();
}
