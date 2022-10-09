#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>

void *test() {
    int i = 0;
    while(i < 5000000000) i++;
    return NULL;
}

int main() {
    pthread_t th[2];
    for(int i = 0; i < 2; i++) {
	pthread_create(&th[i], NULL, test, NULL);
    }
    for(int i = 0; i < 2; i++) {
	pthread_join(th[i], NULL);
    }
    return 0;
}
