# TASKS

# 0. Naive malloc (6pts)

files: `naive_malloc.c`, `malloc.h`

prototype: `void *naive_malloc(size_t size);`
	- `size` is the requested amount by the caller
	- return a pointer to the allocated memory that is suitably aligned for any
	  kind of variable
	- this implementation must be able to
		- allocate enough memory to store
			- a `size_t` as the chunk header
			- the size requested as parameter
		- _only_ allocate __memory pages__

# 1. malloc (20pts)

files: `malloc.c`, `malloc.h`

implement the `malloc()` function

prototype: `void *_malloc(size_t size);`
	- `size` is the requested amount by the caller
	- return a pointer to the allocated memory that is suitably aligned for any
	  kind of variable

# 2. free (24pts)

files: `free.c`, `malloc.c`, `malloc.h`

implement the `free()` function

prototype: `void _free(void *mem);`
	- `mem` is a pointer to the memory to be freed
