#include "malloc.h"

void *calloc(size_t nmemb, size_t size) {
  if (nmemb != 0 && size > SIZE_MAX / nmemb)
    return NULL;

  size_t total_size = nmemb * size;
  void *ptr = malloc(total_size);
  if (!ptr)
    return NULL;

  ft_bzero(ptr, total_size);
  return ptr;
}
