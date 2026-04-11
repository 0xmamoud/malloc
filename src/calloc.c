#include "malloc.h"

void *calloc(size_t nmemb, size_t size) {
  size_t total_size;
  void *ptr;

  if (nmemb != 0 && size > SIZE_MAX / nmemb)
    return NULL;

  total_size = nmemb * size;
  ptr = malloc(total_size);
  if (!ptr)
    return NULL;

  ft_bzero(ptr, total_size);
  return ptr;
}
