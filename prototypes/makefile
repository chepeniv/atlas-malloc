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
	${CCMD} 0.c -o 0.x

1build: clean
	${CCMD} 1.c -o 1.x

2build: clean
	${CCMD} 2.c -o 2.x

3build: clean
	${CCMD} 3.c -o 3.x

4build: clean
	${CCMD} 4.c -o 4.x

5build: clean
	${CCMD} 5.c -o 5.x

6build: clean
	${CCMD} 6.c -o 6.x

7build: clean
	${CCMD} 7.c -o 7.x

8build: clean
	${CCMD} 8.c -o 8.x

9build: clean
	${CCMD} 9.c -o 9.x

######## RUN

0run: 0build
	./0.x

1run: 1build
	./1.x

2run: 2build
	strace ./2.x

3run: 3build
	strace ./3.x

4run: 4build
	./4.x

5run: 5build
	./5.x

6run: 6build
	./6.x

7run: 7build
	./7.x

8run: 8build
	strace ./8.x

9run: 9build
	./9.x

######## TESTING

0test:
	# test suite

######## memory checks

0memtest:
	# ${MEMTEST} ./0.x

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
