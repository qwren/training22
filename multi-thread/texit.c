#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *func() {
	printf("strat exec\n");
	execl("/bin/ls", "ls", NULL);
	//exit(0);
}

int main(){
	pthread_t tid;

	pthread_create(&tid, NULL, func, NULL);

	while(1) {
		printf("1\n");
	}

	return 0;
}
