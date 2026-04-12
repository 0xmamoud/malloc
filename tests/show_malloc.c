#include "test.h"

void run_show_malloc_tests(void) {
  unsigned char *tiny_a;
  unsigned char *tiny_b;
  unsigned char *small_a;
  unsigned char *small_b;
  unsigned char *large_a;
  unsigned char *large_b;

  printf("[show_malloc]\n");

  tiny_a = malloc(100);
  tiny_b = malloc(96);
  small_a = malloc(TINY_MAX + 32);
  small_b = malloc(TINY_MAX + 200);
  large_a = malloc(SMALL_MAX + 64);
  large_b = malloc(SMALL_MAX + 256);

  test_assert(tiny_a && tiny_b && small_a && small_b && large_a && large_b,
              "show_malloc: allocations for all zones succeed");

  memset(tiny_a, 0x11, 100);
  memset(tiny_b, 0x22, 96);
  memset(small_a, 0x33, TINY_MAX + 32);
  memset(small_b, 0x44, TINY_MAX + 200);
  memset(large_a, 0x55, SMALL_MAX + 64);
  memset(large_b, 0x66, SMALL_MAX + 256);

  show_alloc_mem_ex();

  free(tiny_a);
  free(tiny_b);
  free(small_a);
  free(small_b);
  free(large_a);
  free(large_b);
}
