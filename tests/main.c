#include "test.h"
#include <stdio.h>

int main(void) {
  setvbuf(stdout, NULL, _IONBF, 0);

  run_in_child(run_happy_path_tests);
  printf("[PASS] suite: happy_path\n\n");
  run_in_child(run_edge_case_tests);
  printf("[PASS] suite: edge_cases\n\n");
  run_in_child(run_invariant_tests);
  printf("[PASS] suite: invariants\n\n");
  run_in_child(run_stress_tests);
  printf("[PASS] suite: stress\n\n");

  printf("\n================ ALL TESTS PASSED ================\n");
  return 0;
}
