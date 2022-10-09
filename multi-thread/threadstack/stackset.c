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
	pthread_t tid;
	pthread_attr_t tattr;

	pthread_attr_init(&tattr);
	void *stack=malloc(16*1024*1024);
	pthread_attr_setstack(&tattr, stack, 16*1024*1024);
	pthread_create(&tid, &tattr, test, NULL);
	sleep(100);
}
