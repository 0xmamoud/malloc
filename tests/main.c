#include "free/test_free.h"
#include "malloc/test_malloc.h"
#include <stdio.h>

int main(void) {
  run_malloc_happy_path_tests();
  test_reset_allocator();
  printf("\n");

  run_malloc_edge_case_tests();
  test_reset_allocator();
  printf("\n");

  run_malloc_invariant_tests();
  test_reset_allocator();
  printf("\n");

  run_malloc_stress_tests();
  test_reset_allocator();
  printf("\n");

  run_free_happy_path_tests();
  test_reset_allocator();
  printf("\n");

  run_free_edge_case_tests();
  test_reset_allocator();
  printf("\n");

  run_free_invariant_tests();
  test_reset_allocator();

  // printf("\n");
  // show_alloc_mem();

  printf("\n================ ALL TESTS PASSED ================\n");
  return 0;
}
