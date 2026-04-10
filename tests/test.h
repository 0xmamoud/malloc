#ifndef TEST_H
#define TEST_H

#include "malloc.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#define test_assert(cond, name)                                                \
  do {                                                                         \
    assert(cond);                                                              \
    printf("[PASS] %s\n", name);                                               \
  } while (0)

void test_reset_allocator(void);
int test_is_aligned(void *ptr);
int test_region_matches(const unsigned char *ptr, size_t size,
                        unsigned char value);
int test_regions_do_not_overlap(const unsigned char *a, size_t a_size,
                                const unsigned char *b, size_t b_size);
size_t test_count_heaps(t_heap *head);

#endif
