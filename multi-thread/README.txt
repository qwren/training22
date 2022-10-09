####
$ gcc -pthread two-loops.c -o test
$ ps -C test
$ top -H -p <pid>
$ top -H
$ htop

#### 线程退出
$ gcc -pthread texit.c -o test
$ ./test

#### halgrind
$ cd halgrind
$ gcc -g -pthread test.c -o test
$ valgrind --tool=halgrind ./test

#### threadsanitizer
$ cd threadsanitizer
$ gcc -g -fsanitize=thread simple_race.c -o test
$ ./test

#### thread stack
$ ulimit -a
$ cd threadstack
$ gcc -g -pthread stack.c -o test
$ ./test
$ gcc -g -pthread stackset.c -o test
$ ./test

#### scheduler
-- 编译two-loops.c, gcc two-loops.c -pthread，运行两份
$ gcc two-loops.c -pthread
$ ./a.out &
$ ./a.out &
$ top //命令观察CPU利用率
$ sudo renice -n -5 -g 13682 //renice其中之一，再观察CPU利用率

-- 编译two-loops.c, gcc two-loops.c -pthread，运行一份
* top发现其CPU利用率接近200%
* 把它的所有线程设置为SCHED_FIFO
$ chrt -f -a -p 50 进程PID
* 再观察它的CPU利用率
CPU利用率上升还是下降？电脑变快还是变慢？
