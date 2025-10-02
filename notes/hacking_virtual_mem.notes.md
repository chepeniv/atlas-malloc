# hacking virtual memory : the heap, program break, and `malloc`

# the heap

- why doesn't allocated memory start at the very beginning of the heap ?
	- what are the leading bytes used for ?
- does the heap actually growing upwards ?

# `malloc`

this function dynamically allocates memory onto the heap. note that it is _not_
a system call.

## no `malloc`, no heap

```c
#include <stdlib.h>
#include <stdio.h>

/* gcc -Wall -Wextra -pedantic -Werror 0.c -o 0 */
int main(void)
{
	getchar();
	return (EXIT_SUCCESS);
}
```

run `./0`, and obtain the `pid` using `ps -C command`. the memory regions of a
process are listed within the `/proc/[pid]/maps` file. the format of each given
as `address permissions offset device inode pathname`. depending on the system
there may or may not be a listing for the heap even though there may not
actually be any.

# `malloc(n)`

```c
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
	void *p;
	p = malloc(1);
	printf("%p\n", p);
	getchar();
	return (EXIT_SUCCESS);
}
```

the `malloc`-returned address will fall within the \[heap\] memory region denoted
by `/proc/[pid]/maps`

# `strace`, `brk`, and `sbrk`

in order to manipulate the heap `malloc` must itself make some system calls. to
investigate this run `strace`. to determine which syscalls are made by `malloc`
inject a `write()` syscall right before and after `malloc()`

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	void *p;

	write(1, "BEFORE MALLOC\N", 14);
	p = malloc(1);
	write(1, "AFTER MALLOC\N", 13);

	printf("%p\n", p);
	getchar();

	return (EXIT_SUCCESS);
}
```

this allows us to deduce that `malloc` calls `getrandom()`, `brk(NULL)`, and
`brk(0x5b44dfe73000)` in order to manipulate the heap. the `brk` man page
reveals that it "changes a data segment's size"~

more accurately:
>> it changes the location of the program break which defines the end of a
process's data segment. increasing this in effect allocates more memory to the
process

- the program break is the address of the _first_ location _beyond_ the current
end of the data region of the program in _virtual_ memory.

...and this is how `malloc` allocates more memory

the heap is in actuality an extension of a program's data segment

the first call `break(NULL)` returns the current address of the program break to
`malloc`, and the second one actually creates new memory

notice how even thought we may request a single byte from `malloc`, the actual
allocated memory is greater

# many `malloc`s

what occurs whenever we invoke `malloc` several times ?

if we do this, we'll discover that `malloc` doesn't call `brk` each time. after
the first call wherein it creates new space, it uses that same space to allocate
for subsequent allocations until reaching the break, thereby reducing the need
to make system calls each time and thus improving performance. this also allows
`malloc` and `free` to optimize memory usage

# naive `malloc`

assuming we wont ever `free` anything we'll write a very naive version of
`malloc` that moves the program break each time (?) it is called

```c
#include <stdlib.h>
#include <unistd.h>

void *malloc(size_t size)
{
	void *prev_break;

	prev_break = sbrk(size);
	if (prev_break == (void *) -1) /* handle error */
		return (NULL);

	return (prev_break);
}
```

# the 0x10 (0x2a0 locally?) missing bytes

for every call, the start of the allocation given by the return of `malloc` is
0x10 bytes offset from what would otherwise be expected

the size of the of the allocated memory is found in the preceding 0x10 bytes.
for instance, if we call `malloc(1024)`, then it will allocate 1024 + 16 + 1 =
0x400 + 0x10 + 1 = 0x411 actual bytes in memory. this will be encoded by
`00 00 00 00 00 00 00 00 11 04 00 00 00 00 00 00` which we can translate by
reverse ordering the last eight bytes -> 0x0000 0000 0000 0411. these preceding
0x10 bytes are a bookkeeping data structure used by `malloc` to interact with
the heap. so long as we never invoke `free()` and we have the address of the
heaps beginning, then we can use this to navigate the heap

furthermore, the data stored in these 0x10 bytes is also used by `free` to
manage available space

so, we see that the last eight bytes store the size of the allocated memory, but
what do the preceding eight bytes encode ?

# RTFSC

reading the comments for the `malloc` source code in
`glibc-[version]/malloc/malloc.c` we see that these 0x10 bytes encode two
things. the first eight specify the amount of unallocated or free space
preceding this chunk. the second eight specify the size of the following chunk.
the first bit of the current-size bytes serves as a flag to indicate whether the
previous chunk is allocated (1) or not (0).

# heap growth

from what's documented in `man brk` we see that the heap brows upwards. for
`malloc` syscalls are only made whenever allowed heap space was too small to
accommodate any more allocations. run `make 8run` and analyze the `[heap]` via
its `/proc/[pid]/maps` file during its life time.

# address space layout randomization (ASLR)

a thing to notice is that although logically the heap should commence right
after the end of the executable region in memory, this is not the case. the only
thing consistently true is that the heap is always the next memory region
following the executable. also note that the heap is actually part of the data
segment of the executable itself. furthermore, the memory gap between these two
regions is never the same; it is, in fact, random. this is a computer security
technique known as "address space layout randomization" aimed at preventing
exploitation of memory corruption vulnerabilities. because ASLR randomly
arranges addresses of key data areas of a process, this can stop, for example,
an attacker from reliably jumping to a particular exploited function in memory

# vm diagram updated

what happens when we call `malloc(0)` ? running the code `prototypes/9.c` we see
that doing so allocates 32 bytes which include the header 0x10 bytes. from the
man page, this will not always be the case:
> NULL may also be returned by a successful call to malloc() with a size of zero

answering why the minimum chunk size is 32 bytes for `malloc(0)` vs 16 or 0,
provides a clue in understanding how `free` coordinates with `malloc`
