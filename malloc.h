#ifndef _MALLOC_
#define _MALLOC_

#include <stddef.h>

void *naive_malloc(size_t size);
void *_malloc(size_t size);
void _free(void *mem);

#endif /* _MALLOC_ */
