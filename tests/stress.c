#include "test.h"

static void test_many_small_allocations_keep_their_patterns(void) {
  unsigned char *ptrs[1024];
  size_t sizes[1024];
  size_t i;
  int all_valid;

  all_valid = 1;
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
    if (ptrs[i] &&
        !test_region_matches(ptrs[i], sizes[i], (unsigned char)(i % UCHAR_MAX)))
      all_valid = 0;
    i++;
  }
  test_assert(all_valid,
              "stress: written patterns survive across many allocations");

  i = 0;
  while (i < 1024) {
    free(ptrs[i]);
    i++;
  }
}

void run_stress_tests(void) {
  printf("[stress]\n");
  run_in_child(test_many_small_allocations_keep_their_patterns);
}
