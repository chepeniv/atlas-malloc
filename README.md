# C - `malloc`

## Resources

## Objectives

- what is a programming break
- how to handle a program break to dynamically allocate memory
- how do `glibc`'s `malloc()` and `free()` work
- what is ASLR
- what is memory alignmen
- what is a memory page
- how to encapsulate memory management in order to hide it from the user

## Requirements

- all c files will be compiled with `gcc -Wall -Werror -Wextra -pedantic`
- all c code should be `betty` compliant
- no more than five functions per file
- all function declarations should be included in `malloc.h`
- all header files should be include-guarded
- `global` variables are _allowed_
- `static` variables are _allowed_
- unless specified otherwise, the c standard lib is _allowed_
	- the `malloc()` family of functions is _prohibited_

## Testing

- test functions against real programs
- keep function names compatible with `glibc`'s
- compile these into a shared library
- load the shared library with with `LD_LIBRARY_PATH` and `LD_PRELOAD`

## Info

it is not mandatory that these functions behave _exactly_ as their `glibc`
counterparts

- use any data structure that best suits your purposes
- handle the heap as best suits your needs, so long as returned pointers are
aligned as required and enough space is available
- extend the program break as needed, so long as it is by multiples of virtual
memory page size and it is reduced when needed
- full discretion on implementation is given -- ensure to handle large
allocations
