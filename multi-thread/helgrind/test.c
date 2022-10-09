#include <pthread.h>
 
pthread_mutex_t s_mutex_a;
pthread_mutex_t s_mutex_b;
pthread_barrier_t s_barrier;
 
void lock() {
    pthread_mutex_lock(&s_mutex_b);
    {
        pthread_barrier_wait(&s_barrier);
 
        pthread_mutex_lock(&s_mutex_a);
        pthread_mutex_unlock(&s_mutex_a);
    }
    pthread_mutex_unlock(&s_mutex_b);
}
 
static void* thread_routine(void* arg) {
    pthread_mutex_lock(&s_mutex_a);
    {
        pthread_barrier_wait(&s_barrier);
 
        pthread_mutex_lock(&s_mutex_b);
        pthread_mutex_unlock(&s_mutex_b);
    }
    pthread_mutex_unlock(&s_mutex_a);
}
 
int main(int argc, char** argv) {
    pthread_t tid;
 
    pthread_mutex_init(&s_mutex_a, 0);
    pthread_mutex_init(&s_mutex_b, 0);
    pthread_barrier_init(&s_barrier, 0, 2);
 
    pthread_create(&tid, 0, &thread_routine, 0);
 
    lock();
 
    pthread_join(tid, 0);
    pthread_cancel(tid);
 
    pthread_barrier_destroy(&s_barrier);
    pthread_mutex_destroy(&s_mutex_a);
    pthread_mutex_destroy(&s_mutex_b);
 
    return 0;
}
