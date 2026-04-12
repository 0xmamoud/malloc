#include "test.h"
#include <stdio.h>

int main(void) {
  setvbuf(stdout, NULL, _IONBF, 0);

  run_test_suite("happy_path", run_happy_path_tests);
  run_test_suite("edge_cases", run_edge_case_tests);
  run_test_suite("invariants", run_invariant_tests);
  run_test_suite("stress", run_stress_tests);

  printf("\n================ ALL TESTS PASSED ================\n");
  return 0;
}
