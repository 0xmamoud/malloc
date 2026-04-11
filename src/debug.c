#include "malloc.h"

static bool debug_env_enabled(const char *value) {
  if (!value)
    return false;
  return atoi(value) != 0;
}

void debug_init(void) {
  const char *value;

  if (g_malloc.debug_initialized)
    return;
  value = getenv("MALLOC_DEBUG");
  g_malloc.debug_enabled = debug_env_enabled(value);
  g_malloc.debug_initialized = true;
}

void debug_malloc(size_t size, void *result) {
  if (!g_malloc.debug_enabled)
    return;
  ft_putstr("[malloc] size=");
  ft_putnbr(size);
  ft_putstr(" result=");
  ft_putaddr(result);
  ft_putchar('\n');
}

void debug_free(void *ptr) {
  if (!g_malloc.debug_enabled)
    return;
  ft_putstr("[free] ptr=");
  ft_putaddr(ptr);
  ft_putchar('\n');
}

void debug_realloc(void *ptr, size_t size, void *result) {
  if (!g_malloc.debug_enabled)
    return;
  ft_putstr("[realloc] ptr=");
  ft_putaddr(ptr);
  ft_putstr(" size=");
  ft_putnbr(size);
  ft_putstr(" result=");
  ft_putaddr(result);
  ft_putchar('\n');
}
