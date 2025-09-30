CC      := gcc
GDB     := cgdb

CFLAGS  := -Wall -Werror -Wextra -pedantic 
# -std=c99
CDEBUG  := -ggdb3
CCMD    := ${CC} ${CFLAGS} ${CDEBUG} 

BINDIR  := bin
TESTDIR := test
DEFDIR  := def
HEADDIR  := include

VALFL   := -s --track-origins=yes --leak-check=full --show-leak-kinds=all
MEMTEST := valgrind ${VALFL}

.DELETE_ON_ERROR:

.PHONY: clean clean-obj clean-lib betty

######## TASKS

0build: clean
	# ${CCMD} source.c -o 0bin.x

######## RUN

0run: 0build
	# ./0bin.x

######## TESTING

0test:
	# test suite

######## memory checks

0memtest:
	# ${MEMTEST} ./0bin.x

######## UTILITY

betty:
	@echo "running Betty CODING STYLE checks..."
	@betty-style *.c *.h
	@echo "running Betty DOCUMENTATION checks..."
	@betty-doc *.c *.h

clean: clean-obj clean-lib
	@$(RM) -v *~ *.exec *.out *.x

clean-obj:
	@$(RM) -v *.o

clean-lib:
	@$(RM) -v *.so
