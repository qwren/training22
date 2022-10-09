#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
	int i;
	unsigned int *p = malloc(10);

	printf("malloc buffer: %p\n", p);

	for (i = 0; i < 20; i++) {
		p[i] = 123;
	}
	return 0;
}
