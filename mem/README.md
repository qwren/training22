### 内存管理层次

1. 查看/proc/zoneinfo, /proc/buddyinfo, /proc/slabinfo

2. 查看/proc/vmallocinfo, grep ioremap

### 进程地址空间

1. smem
$ gcc leak-example.c -o test
$ ./test &
$ smem -P test

2. valgrind                                                       
$ gcc -g leak-example.c
$ valgrind --tool=memcheck --leak-check=yes ./a.out

3. oom
$ sudo swapoff -a
$ sudo sh -c 'echo 1 > /proc/sys/vm/overcommit_memory'
$ gcc oom.c
$ ./a.out
$ dmesg

### pagecache/DMA/swap/...

1. 理解page cache对程序速度影响
    $ sudo sh -c 'echo 3 > /proc/sys/vm/drop_caches'
    $ time python hello.py 
    Hello World! Love, Python

    real    0m0.142s
    user    0m0.000s
    sys 0m0.020s
    
    $ time python hello.py
    ...
    real    0m0.018s
    ...
* 两次时间差很大！

2. memory cgroup
* 本实验要关闭swap, 运行swapoff -a，另外允许overcommit                                                               
    $ sudo swapoff -a
    $ sudo sh -c 'echo 1 > /proc/sys/vm/overcommit_memory'
    $ sudo sh -c 'echo 0 > memory.swappiness' //禁止当前cgroup使用swap
* 创建memory cgroup
    $ cd /sys/fs/cgroup/memory
    $ sudo mkdir A
    $ cd A
    $ sudo sh -c 'echo 200M > memory.limit_in_bytes'
* 把oom.c用cgexec放到新的group执行
    $ sudo cgexec -g memory:A ./test


