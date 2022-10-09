#include <string.h>
#include <stdio.h>

int main(void)
{
	int i;
	int a[4];

	printf("&a=%p,&i=%p\n", &a, &i);
	printf("%d\n", a[4]);

	return 0;
}
