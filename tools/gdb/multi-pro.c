/**************************************
*文件说明:process.c
*作者:段晓雪
*创建时间:2017年06月10日 星期六 10时59分14秒
*开发环境:Kali Linux/g++ v6.3.0
****************************************/

#include<stdio.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main()
{
    pid_t pid = fork();//创建子进程

    if(pid == -1)
    {
        perror("fork error");
        return -1;
    }
    else if(pid == 0)//child
    {
        printf("i am a child:my pid is %d,my father is %d\n",getpid(),getppid());
    }
    else//father
    {
        printf("i am a father:my pid is %d\n",getpid());
        wait(NULL);//等待子进程
    }

    return 0;

}
