#include "test_malloc.h"

static void test_many_small_allocations_remain_usable(void) {
  unsigned char *ptrs[1024];
  size_t sizes[1024];
  size_t i;
  int all_valid = 1;

  i = 0;
  while (i < 1024) {
    sizes[i] = (i % 128) + 1;
    ptrs[i] = malloc(sizes[i]);
    if (!ptrs[i] || !test_is_aligned(ptrs[i]))
      all_valid = 0;
    else
      memset(ptrs[i], (unsigned char)(i % UCHAR_MAX), sizes[i]);
    i++;
  }
  test_assert(all_valid,
              "stress: many small allocations succeed and stay aligned");

  i = 0;
  while (i < 1024) {
    if (ptrs[i] && !test_region_matches(ptrs[i], sizes[i],
                                         (unsigned char)(i % UCHAR_MAX)))
      all_valid = 0;
    i++;
  }
  test_assert(all_valid,
              "stress: written patterns survive across many allocations");
}

static void test_many_boundary_crossing_allocations(void) {
  void *ptrs[96];
  size_t i;
  int all_valid = 1;

  i = 0;
  while (i < 32) {
    ptrs[i] = malloc(TINY_MAX - 8 + (i % 8));
    ptrs[i + 32] = malloc(TINY_MAX + 1 + (i % 32));
    ptrs[i + 64] = malloc(SMALL_MAX + 1 + (i * 17));
    if (!ptrs[i] || !ptrs[i + 32] || !ptrs[i + 64])
      all_valid = 0;
    i++;
  }
  test_assert(all_valid,
              "stress: repeated allocations across zone thresholds succeed");
}

void run_malloc_stress_tests(void) {
  printf("[stress]\n");
  test_many_small_allocations_remain_usable();
  test_many_boundary_crossing_allocations();
}
