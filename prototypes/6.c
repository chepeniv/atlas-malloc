#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void
print_bytes(void *addr, unsigned int count)
{
	unsigned char *ptr;
	unsigned int i;

	ptr = (unsigned char *) addr;
	for (i = 0; i < count; i++)
	{
		if (i != 0)
		{
			printf(" ");
		}
		printf("%02x", *(ptr + i));
	}
	printf("\n");
}

int
main(void)
{
	int i;
	void *mem, *chunks[10];
	char *mem_info;
	size_t chunk_size, prev_chunk_size;

	for (i = 0; i < 10; i++)
	{
		mem = malloc(1024 * (i + 1));
		chunks[i] = mem;
		printf("%p\n", mem);
	}

	free(chunks[3]);
	free(chunks[7]);
	for (i = 0; i < 10; i++)
	{
		mem_info = (char *) chunks[i] - 0x10;
		printf("chunks[%d]: ", i);
		print_bytes(mem_info, 0x10);

		chunk_size = *((size_t *) (mem_info + 0x08)) - 1;
		prev_chunk_size = *((size_t *) mem_info);
		printf("chunks[%d]: %p, size = %li, prev = %li\n",
			i, mem_info + 0x10, chunk_size, prev_chunk_size);
	}

	return (EXIT_SUCCESS);
}
