#include "test.h"

#include <sys/wait.h>
#include <unistd.h>

void run_in_child(t_test_suite_fn fn) {
  pid_t pid;
  int status;

  pid = fork();
  assert(pid >= 0);
  if (pid == 0) {
    fn();
    _exit(0);
  }
  assert(waitpid(pid, &status, 0) == pid);
  assert(WIFEXITED(status) && WEXITSTATUS(status) == 0);
}

int test_region_matches(const unsigned char *ptr, size_t size,
                        unsigned char value) {
  size_t i;

  i = 0;
  while (i < size) {
    if (ptr[i] != value)
      return 0;
    i++;
  }
  return 1;
}
