#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int i=0;

void* test(void *s)
{
	int buffer[1024];
	printf("i=%d\n", i);
	i++;
	test(s);
}

int main()
{
	pthread_t p1;
	printf("pid=%d\n", getpid());
	pthread_create(&p1, NULL, test, NULL);
	sleep(100);
}
