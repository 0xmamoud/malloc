#include "test.h"

static void *malloc_runtime(size_t size) {
  volatile size_t runtime_size;

  runtime_size = size;
  return malloc(runtime_size);
}

static void *realloc_runtime(void *ptr, size_t size) {
  volatile size_t runtime_size;

  runtime_size = size;
  return realloc(ptr, runtime_size);
}

static void test_malloc_zero_returns_null(void) {
  test_assert(malloc(0) == NULL, "edge_cases: malloc(0) returns NULL");
}

static void test_realloc_null_behaves_like_malloc(void) {
  unsigned char *ptr;

  ptr = realloc(NULL, 64);
  test_assert(ptr != NULL,
              "edge_cases: realloc(NULL, size) behaves like malloc");
  memset(ptr, 0x5C, 64);
  test_assert(test_region_matches(ptr, 64, 0x5C),
              "edge_cases: realloc(NULL, size) returns writable memory");

  free(ptr);
}

static void test_realloc_zero_behaves_like_free(void) {
  unsigned char *ptr;
  unsigned char *guard;

  ptr = malloc(64);
  guard = malloc(32);
  test_assert(ptr && guard, "edge_cases: realloc allocations succeed");

  memset(guard, 0x2D, 32);
  test_assert(realloc(ptr, 0) == NULL,
              "edge_cases: realloc(ptr, 0) returns NULL");
  test_assert(test_region_matches(guard, 32, 0x2D),
              "edge_cases: realloc(ptr, 0) preserves neighbors");

  free(guard);
}

static void test_oversized_malloc_fails_cleanly(void) {
  test_assert(malloc_runtime(SIZE_MAX) == NULL,
              "edge_cases: malloc(SIZE_MAX) returns NULL");
  test_assert(malloc_runtime(SIZE_MAX - 1) == NULL,
              "edge_cases: malloc(SIZE_MAX - 1) returns NULL");
  test_assert(malloc_runtime(SIZE_MAX - (ALIGNMENT - 1)) == NULL,
              "edge_cases: alignment-overflow boundary returns NULL");
}

static void test_oversized_realloc_fails_without_corrupting_source(void) {
  unsigned char *ptr;

  ptr = malloc(64);
  test_assert(ptr != NULL,
              "edge_cases: realloc overflow source allocation succeeds");
  memset(ptr, 0x6E, 64);
  test_assert(realloc_runtime(ptr, SIZE_MAX) == NULL,
              "edge_cases: realloc(SIZE_MAX) returns NULL");
  test_assert(test_region_matches(ptr, 64, 0x6E),
              "edge_cases: realloc failure preserves original block");

  free(ptr);
}

static void test_zone_boundaries_are_supported(void) {
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

  free(tiny_max);
  free(tiny_next);
  free(small_max);
  free(small_next);
}

static void test_realloc_crosses_zone_thresholds(void) {
  unsigned char *tiny_to_small;
  unsigned char *small_to_large;

  tiny_to_small = malloc(TINY_MAX);
  small_to_large = malloc(SMALL_MAX);
  test_assert(tiny_to_small && small_to_large,
              "edge_cases: threshold realloc setup allocations succeed");

  memset(tiny_to_small, 0x14, TINY_MAX);
  memset(small_to_large, 0x28, SMALL_MAX);

  tiny_to_small = realloc(tiny_to_small, TINY_MAX + 1);
  small_to_large = realloc(small_to_large, SMALL_MAX + 1);
  test_assert(tiny_to_small != NULL,
              "edge_cases: realloc tiny-to-small succeeds");
  test_assert(small_to_large != NULL,
              "edge_cases: realloc small-to-large succeeds");
  test_assert(test_region_matches(tiny_to_small, TINY_MAX, 0x14),
              "edge_cases: realloc tiny-to-small preserves bytes");
  test_assert(test_region_matches(small_to_large, SMALL_MAX, 0x28),
              "edge_cases: realloc small-to-large preserves bytes");

  free(tiny_to_small);
  free(small_to_large);
}

void run_edge_case_tests(void) {
  printf("[edge_cases]\n");
  run_in_child(test_malloc_zero_returns_null);
  run_in_child(test_realloc_null_behaves_like_malloc);
  run_in_child(test_realloc_zero_behaves_like_free);
  run_in_child(test_oversized_malloc_fails_cleanly);
  run_in_child(test_oversized_realloc_fails_without_corrupting_source);
  run_in_child(test_zone_boundaries_are_supported);
  run_in_child(test_realloc_crosses_zone_thresholds);
}
