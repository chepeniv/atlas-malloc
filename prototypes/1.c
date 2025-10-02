#include <stdlib.h>
#include <stdio.h>

int main(void)
{
	void * p;

	p = malloc(256);
	printf("%p\n", p);
	getchar();
	free(p);

	return (EXIT_SUCCESS);
}
