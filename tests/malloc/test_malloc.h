#ifndef TEST_MALLOC_H
#define TEST_MALLOC_H

#include "malloc.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

void run_happy_path_tests(void);
void run_edge_case_tests(void);
void run_invariant_tests(void);
void run_stress_tests(void);

#define test_assert(cond, name)                                                \
  do {                                                                         \
    assert(cond);                                                              \
    printf("[PASS] %s\n", name);                                               \
  } while (0)

static inline int test_is_aligned(void *ptr) {
  return ((uintptr_t)ptr % ALIGNMENT) == 0;
}

static inline int test_region_matches(const unsigned char *ptr, size_t size,
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

static inline int test_regions_do_not_overlap(const unsigned char *a,
                                              size_t a_size,
                                              const unsigned char *b,
                                              size_t b_size) {
  const unsigned char *a_end;
  const unsigned char *b_end;

  a_end = a + a_size;
  b_end = b + b_size;
  return (a_end <= b || b_end <= a);
}

#endif
