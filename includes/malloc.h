#ifndef MALLOC_H
#define MALLOC_H
#include <stdlib.h>
#include <stdbool.h>

typedef struct s_block {
    struct s_block *prev;
    struct s_block *next;
    size_t size;
    bool is_free;
} t_block;

typedef struct s_heap {
    struct s_heap *prev;
    struct s_heap *next;
    size_t total_size;
    size_t free_size;
    t_block *blocks;
} t_heap;

typedef struct s_malloc {
    t_heap *tiny;
    t_heap *small;
    t_heap *large;
} t_malloc;

t_malloc *get_malloc_instance(void);

 
// utils
void	*ft_memset(void *b, int c, size_t len);
void	ft_bzero(void *s, size_t n);
void	*ft_memcpy(void *dst, const void *src, size_t n);
size_t	ft_strlen(const char *s);



#endif
