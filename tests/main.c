#include "malloc/test_malloc.h"
#include <stdio.h>

int main(void) {
  run_happy_path_tests();
  printf("\n");
  run_edge_case_tests();
  printf("\n");
  run_invariant_tests();
  printf("\n");
  run_stress_tests();

  // printf("\n");
  // show_alloc_mem();

  printf("\n================ ALL TESTS PASSED ================\n");
  return 0;
}
