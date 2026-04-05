#include "../includes/malloc.h"

void *ft_memset(void *b, int c, size_t len) {
  unsigned char *ptr = (unsigned char *)b;

  for (size_t i = 0; i < len; i++) {
    ptr[i] = (unsigned char)c;
  }

  return (b);
}

void ft_bzero(void *s, size_t n) {
  unsigned char *ptr = (unsigned char *)s;

  for (size_t i = 0; i < n; i++) {
    ptr[i] = 0;
  }
}

void *ft_memcpy(void *dst, const void *src, size_t n) {
  unsigned char *dst_ptr = (unsigned char *)dst;
  const unsigned char *src_ptr = (const unsigned char *)src;

  if (!dst && !src)
    return (NULL);

  for (size_t i = 0; i < n; i++) {
    dst_ptr[i] = src_ptr[i];
  }

  return (dst);
}

size_t ft_strlen(const char *s) {
  size_t len = 0;

  while (s[len]) {
    len++;
  }

  return (len);
}

size_t align_size(size_t size) {
  return ((size + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1));
}
