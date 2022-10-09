### 进程生命周期(就绪、运行、睡眠、停止、僵死)

1. 编译运行life_period.c，
* 在if 1改为if 0的情况下，

     21 #if 1 /* define 1 to make child process always a zomie */
     22                 printf("ppid:%d\n", getpid());
     23                 while(1);
     24 #endif
杀死子进程，观察父进程能监控到子进程死亡原因。

* 在if 1不改为if 0的情况下，
杀死子进程，观察父进程的僵尸态。
杀死父进程，观察拖孤。

2. cpulimit
编译cpulimit.c这个死循环程序，gcc cpulimit.c，运行之./a.out，通过pidof a.out命令获得它的pid，然后开一个terminal观察top，发现a.out这个进程的CPU利用率接近100%，之后通过cpulimit控制它的CPU利用率，再看top
* cpulimit -l 20 -p  a.out的pid
* cpulimit -l 30 -p  a.out的pid
...

3. clone
编译two-loops.c, gcc two-loops.c -pthread -o test
  $ strace -e clone ./test  //跟踪clone系统调用

4. CPU cgroup
编译two-loops.c, gcc two-loops.c -pthread，运行三份
* 用top观察CPU利用率，大概各自66%。
* 创建A,B两个cgroup

        :/sys/fs/cgroup/cpu$ sudo mkdir A
        :/sys/fs/cgroup/cpu$ sudo mkdir B
* 把3个a.out中的2个加到A，1个加到B。

        :/sys/fs/cgroup/cpu/A$ sudo sh -c 'echo 14995 > cgroup.procs'
        :/sys/fs/cgroup/cpu/A$ sudo sh -c 'echo 14998 > cgroup.procs'
        :/sys/fs/cgroup/cpu/A$ cd ..
        :/sys/fs/cgroup/cpu$ cd B/
        :/sys/fs/cgroup/cpu/B$ sudo sh -c 'echo 15001 > cgroup.procs'

* 这次发现3个a.out的CPU利用率大概是50%, 50%, 100%。
* 杀掉第2个和第3个a.out，然后调整cgroup A的quota，观察14995的CPU利用率变化

        :/sys/fs/cgroup/cpu/B$ kill 14998
        :/sys/fs/cgroup/cpu/B$ kill 15001
* 设置A group的quota为20ms：

        :/sys/fs/cgroup/cpu/A$ sudo sh -c 'echo 20000 > cpu.cfs_quota_us'
* 设置A group的quota为40ms：

        :/sys/fs/cgroup/cpu/A$ sudo sh -c 'echo 40000 > cpu.cfs_quota_us'
* 以上各自情况，用top观察a.out CPU利用率。


### 调度

1. 编译two-loops.c, gcc two-loops.c -pthread，运行两份
* top命令观察CPU利用率

* renice其中之一，再观察CPU利用率
$ sudo renice -n -5 -g 13682

2. 编译two-loops.c, gcc two-loops.c -pthread，运行一份
* top发现其CPU利用率接近200%
* 把它的所有线程设置为SCHED_FIFO

        chrt -f -a -p 50 进程PID
* 再top观察它的CPU利用率

3. cyclictest
* cyclictest -p 80 -t5 -n
* 观察min, max, act, avg时间，分析hard realtime问题
* 加到系统负载，运行一些硬盘访问，狂收发包的程序，观察cyclictest的max变化

