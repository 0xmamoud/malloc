#include "test_free.h"

static void test_free_null_is_noop(void) {
  free(NULL);
  test_assert(g_malloc.tiny == NULL && g_malloc.small == NULL &&
                  g_malloc.large == NULL,
              "free edge_cases: free(NULL) leaves allocator state unchanged");
}

static void test_free_last_allocation_releases_each_zone(void) {
  void *tiny;
  void *small;
  void *large;

  tiny = malloc(TINY_MAX);
  small = malloc(TINY_MAX + 1);
  large = malloc(SMALL_MAX + 1);

  free(tiny);
  free(small);
  free(large);

  test_assert(
      g_malloc.tiny == NULL && g_malloc.small == NULL && g_malloc.large == NULL,
      "free edge_cases: freeing the last allocation releases each zone");
}

void run_free_edge_case_tests(void) {
  printf("[free edge_cases]\n");
  test_free_null_is_noop();
  test_free_last_allocation_releases_each_zone();
}
