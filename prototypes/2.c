#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
	void * p;

	write(1, "BEFORE MALLOC\n", 14);
	p = malloc(1);
	write(1, "AFTER MALLOC\n", 13);
	printf("%p\n", p);

	getchar(); /* keep program running */
	free(p);

	return (EXIT_SUCCESS);
}
