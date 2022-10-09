#include<stdio.h>
#include<pthread.h>

void* thread1(void* arg)
{
    printf("i am thread1,my tid is %u\n",pthread_self());
    return NULL;
}

void* thread2(void* arg)
{
    printf("i am thread2,my tid is %u\n",pthread_self());
    return NULL;
}

int main()
{
    pthread_t tid1,tid2;
    pthread_create(&tid1,NULL,thread1,NULL);//创建线程1
    pthread_create(&tid2,NULL,thread2,NULL);//创建线程2

    pthread_join(tid1,NULL);//等待线程1
    pthread_join(tid2,NULL);//等待线程2 

    return 0;
}
