#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
	void *prog_break;

	prog_break = sbrk(0);
	printf("program break is %p\n", prog_break);

	return (EXIT_SUCCESS);
}
