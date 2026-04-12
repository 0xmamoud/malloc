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

void run_happy_path_tests(void);
void run_edge_case_tests(void);
void run_invariant_tests(void);
void run_stress_tests(void);
void test_reset_allocator(void);
void run_test_case(t_test_suite_fn fn);
void run_test_suite(const char *name, t_test_suite_fn fn);
int test_is_aligned(void *ptr);
int test_region_matches(const unsigned char *ptr, size_t size,
                        unsigned char value);
int test_regions_do_not_overlap(const unsigned char *a, size_t a_size,
                                const unsigned char *b, size_t b_size);
size_t test_count_heaps(t_heap *head);

#endif
