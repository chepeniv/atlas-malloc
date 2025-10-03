# implementing your own dynamic memory allocation library

# preliminaries

read the holberton blogpost, the `sbrk (2)` and the `malloc (3)` manpages

here we'll build a simple implementation of `gilbc`'s `malloc`

# starting break

printout the address of the starting break using `sbrk`

```c
/* print_break.c */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
	void *ptr;

	ptr = sbrk(0);
	printf("Starting break is %p\n", ptr);

	return (EXIT_SUCCESS);
}
```

# segfault

attempting to access the return address of `sbrk(0)` results in a segmentation
fault.

write a program that prints out the program break and then increments the
program break by one byte, store a character in the new space, and then print it

# custom malloc

with the previous exercise completed, write a naive version of `malloc` only
using `sbrk` that simply extends the program break by the size it is given

although fully optimized (no extra bytes and everything is contiguous), this
naive `malloc` doesn't permit management of memory chunks (`free` and `realloc`)

# improvements

in comparison to actual `malloc` (3) there three things to note:
1. an initial call to `malloc` returns an address 0x10 bytes displaced after the
   old program break
2. the program break is incremented by a large amount subsequent calls may not
   alter it for a while
3. it reserves more memory than asked for

## 1. memory chunk header

write a new version of `naive_malloc` that allocates a bit more space in order
to store a header in front of the requested memory space. in here, we'll store
the size of the allocated memory in a `size_t` -- meaning, that the total
allocation will be `requested_size + sizeof(size_t)` and the returned address
will be `alloc_addr + sizeof(size_t)` (the address returned must be after the
header)

why would this be useful ? it's critical to keep track of which chunks to free
and to reallocate them when needed.

## 2. memory pages

`malloc` by reserving a large enough space reduces the number of system calls
needed, thereby making allocation much faster in subsequent invocations. the
standard `glibc` `malloc` extends the program break by multiples of the _virtual
memory page size_, which on most 64bit architectures will be 4Kib (0x1000 = 4096
bytes). use `sysconf` (3) to determine this.

### complications

not every time we call `malloc` should we extend the program break, but instead
use the pre-allocated space of a previous call. to properly implement this we'll
need to do bookkeeping on the available and used space already reserved. this
will require a `static` variable to record the beginning of the heap which will
be initialized on the very first call. since this is where the first allocation
lives, this will allow us to navigate from chunk to chunk just by analyzing the
headers of each.

to keep track of available space we'll need to preface it with headers as well.
every time we insert a chunk into an unused chunk we'll need to split that
unused space in order to accommodate for the new used chunk -- that is, we'll
have to update the header at the start and add a new one at the end of the new
used chunk to maintain track of the modified free space.

how is it determined whether a chunk is available or used ? we'll need to add a
flag to indicate whether a chunk is occupied or available.

another thing to maintain track of is the number of chunks allocated. we can do
this with another `static` variable as well

to recap, after the first call well need to iterate through the chain of chunks in order
to locate an unused chunks. afterwards, we'll need to split it into two and
return the foremost partition and leave the other as excess memory. if the heap
space is not full, there's no need to extend it further.

besides all this we will need to be able to extend the program break whenever
the heap becomes full. we also need the ability to allocate memory larger than
the page size to prevent segfaults. another issue is that even if the chunk
requested and its header fit perfectly into the remaining space up to the
program break will still cause a segfault if we attempt to append a new header
to denote the following free space: `reserved_space - (pre_header + request +
post_header) >= 0`. before allotting any more memory to the program we need to
check that there is enough remaining continuous reserve to accommodate the
requested space plus the tail-end headers -- if not, then request the necessary
number of pages by extending the break further. that is, every time a page fills
up, request more as needed before returning.

## 3. alignment

glibc's `malloc` may actually grant more memory than asked. this is because it
returns addresses that are _aligned_ -- specifically, as stated in the manpage,
"a pointer to the allocated memory that is suitably aligned for any kind of
variable". meaning that the returned address will be aligned for _all_ types and
this is dictated by the _largest_ type. this is because `malloc` doesn't know
what kind of variable the space will be used for. to implement this simply round
up to the next multiple of `alignment_requirement` for the `size` given. by
allocating this extra space, the next address after the chunk will be aligned.
note that the first call to `sbrk` will return a _page-aligned_ random address.

so the total memory manipulated will be `pre_header + requested_space +
align_adjust + post_header`

# progressing further

possible improvements to make:

- the static counter is not all that necessary and so it can be removed
- to implement `free`, `malloc` will have to be modified in order to analyze the
  chunk chain for available space rather than just appending new chunks to the
  end
- the header can be expanded for better optimization. this must be balanced
  between needs and alignment
- add error handling
