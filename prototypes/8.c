#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(void)
{
	int i;
	void *mem;

	write(1, "START\n", 6);
	mem = malloc(1);
	getchar();

	write(1, "LOOP\n", 5);
	for (i = 0; i < 0x25000 / 1024; i++)
		mem = malloc(1024);
	write(1, "END\n", 4);

	(void) mem;

	getchar();

	return (EXIT_SUCCESS);
}
