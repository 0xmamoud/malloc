#ifndef TEST_H
#define TEST_H

#include "malloc.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

typedef void (*t_test_suite_fn)(void);

#define test_assert(cond, name)                                                \
  do {                                                                         \
    assert(cond);                                                              \
    printf("[PASS] %s\n", name);                                               \
  } while (0)

#define test_is_aligned(ptr) (((uintptr_t)(ptr) % ALIGNMENT) == 0)

#define test_regions_do_not_overlap(a, a_size, b, b_size)                      \
  (((const unsigned char *)(a) + (a_size)) <= (const unsigned char *)(b) ||   \
   ((const unsigned char *)(b) + (b_size)) <= (const unsigned char *)(a))

void run_happy_path_tests(void);
void run_edge_case_tests(void);
void run_invariant_tests(void);
void run_stress_tests(void);
void run_show_malloc_tests(void);
void run_in_child(t_test_suite_fn fn);
int test_region_matches(const unsigned char *ptr, size_t size,
                        unsigned char value);

#endif
