#include "test_malloc.h"

static void *malloc_runtime(size_t size) {
  volatile size_t runtime_size;

  runtime_size = size;
  return malloc(runtime_size);
}

static void test_zero_size_returns_null(void) {
  test_assert(malloc(0) == NULL, "edge_cases: malloc(0) returns NULL");
}

static void test_oversized_requests_fail_cleanly(void) {
  test_assert(malloc_runtime(SIZE_MAX) == NULL,
              "edge_cases: malloc(SIZE_MAX) returns NULL");
  test_assert(malloc_runtime(SIZE_MAX - 1) == NULL,
              "edge_cases: malloc(SIZE_MAX - 1) returns NULL");
  test_assert(malloc_runtime(SIZE_MAX - (ALIGNMENT - 1)) == NULL,
              "edge_cases: alignment-overflow boundary returns NULL");
}

static void test_zone_boundaries_are_allocatable(void) {
  void *tiny_max;
  void *tiny_next;
  void *small_max;
  void *small_next;

  tiny_max = malloc(TINY_MAX);
  tiny_next = malloc(TINY_MAX + 1);
  small_max = malloc(SMALL_MAX);
  small_next = malloc(SMALL_MAX + 1);
  test_assert(tiny_max != NULL, "edge_cases: TINY_MAX allocation succeeds");
  test_assert(tiny_next != NULL,
              "edge_cases: TINY_MAX + 1 allocation succeeds");
  test_assert(small_max != NULL, "edge_cases: SMALL_MAX allocation succeeds");
  test_assert(small_next != NULL,
              "edge_cases: SMALL_MAX + 1 allocation succeeds");
}

void run_malloc_edge_case_tests(void) {
  printf("[edge_cases]\n");
  test_zero_size_returns_null();
  test_oversized_requests_fail_cleanly();
  test_zone_boundaries_are_allocatable();
}
