#### strace
$ strace cat hello.py //查看库依赖
$ strace -e read cat hello.py 
$ strace -e trace=open,read cat hello.py

$ ps -C firefox
$ strace -p <pid>

$ strace -T -tt -e read cat hello.py //统计系统调用开始时间以及花费的时间

$ strace -C cat hello.py //生成系统调用的统计报告，查看耗时的操作

$ strace vim 2>&1 | grep .viminfo //查看被实际加载的配置文件

$ gcc life_period.c -o test
$ strace -f ./test //跟踪fork或clone
$ strace -p <pid> //查看父进程和子进程为什么卡住
$ kill -2 <child_pid>
$ strace -p <child_pid> //strace没有权限访问僵尸进程

#### ltrace
$ ltrace cat hello.py
$ ltrace -S cat hello.py //类似于strace
$ ltrace -c cat hello.py
$ ltrace -T -tt cat hello.py
$ ltrace -c dd if=/dev/urandom of=/dev/null count=1000
$ strace -c dd if=/dev/urandom of=/dev/null count=1000 //strace把性能提高了很多倍

#### smem & 内存泄漏
$ gcc leak-example.c -o test
$ smem -P test
$ valgrind --tool=memcheck --leak-check=yes ./test

#### buffer overflow
$ gcc buffer_overflow.c -o test
$ ./test
$ gcc -fno-stack-protector buffer_overflow.c -o test
$ gcc -fstack-protector-all buffer_overflow.c -o test

查看栈的布局
$ gcc buffer_overflow1.c -o test //a[3] a[2] a[1] a[0] i, 在a[4]的位置会放一个保护变量 
$ gcc -fno-stack-protector buffer_overflow1.c -o test

#### 文件系统 proc,sysfs,dev,...
$ cat /proc/meminfo
$ cat /proc/cpuinfo
$ cat /proc/<pid>/maps

#### cyclictest实时性
$ cyclictest -p 80 -t5 -n //测试内核的调度延迟

#### LD_PRELOAD
$ cd preload
$ gcc test.c -o test
$ ldd test
$ gcc -fpic -c hack.c -o hack.o
$ gcc -shared hack.o -o hack.so
$ export LD_PRELOAD=./hack.so  //加载hack库
$ ldd test //与加载hack库之前比较
$ export LD_PRELOAD=NULL or unset LD_PRELOAD //卸载hack库

#### lttng
$ sudo apt-get install lttng-tools lttng-modules-dkms liblttng-ust-dev
$ cd lttng
$ gcc -c -I. hello-tp.c
$ gcc -c hello.c
$ gcc -o hello hello.o hello-tp.o -llttng-ust -ldl
$ ./hello world and beyond

new terminal
$ lttng-sessiond --daemonize
$ lttng list --userspace
$ lttng create my-session
$ lttng enable-event --userspace hello_world:my_first_tracepoint
$ lttng start
then go back to the running hello application and press Enter. The program executes all tracepoint() instrumentation points and exits.
$ lttng stop 
$ lttng destroy

View and analyze the recorded events
$ babeltrace ~/lttng-traces/my-session*

#### -Wl,--wrap=
$ cd wrapper
$ gcc -c wrap.c test.c
$ gcc -Wl,--wrap,malloc -o test wrap.o test.o

#### addr2line
$ cd addr2line
$ gcc -o test -g test.c
$ dmesg | tail
$ addr2line -e test <ip>

#### gprof
$ cd gprof
$ gcc -pg main.c func.c -o test
$ ./test //执行后生成一个新的gmon.out文件
$ gprof test gmon.out
// flat profile: 函数调用次数与时间
// call graph: 包括调用关系

#### nm
$ cd nm
$ gcc -g -c test.c
$ nm -n test.o
$ gcc -g test.c -o test
$ nm -n test

#### objcopy
$ cd objcopy
$ objcopy -I binary -O elf64-x86-64 -B i386 image.png image.o
$ objcopy -I binary -O elf32-i386 -B i386 image.png image.o
$ objdump -ht image.o or nm -n image.o
$ gcc -c test.c
$ gcc test.o image.o -o test

#### readelf
$ cd readelf
$ gcc -c test.c
$ gcc test.c -o test
$ readelf -h test.o or test
$ readelf -S test.o
$ readelf -a test > test.info

#### gcov
$ cd gcov
$ gcc -fprofile-arcs -ftest-coverage test.c -o test //生成test.gcno文件
$ ./test //生成test.gcda文件
$ gcov test.c //生成test.c.gcov文件
$ cat test.c.gcov
$ gcov -a test.c

#### gcc process
$ gcc -Wall -save-temps test.c -o test

#### gcc: O2, Os, ...
$ gcc -c -Q -Os --help=optimizers > /tmp/Os-opts
$ gcc -c -Q -O2 --help=optimizers > /tmp/O2-opts
$ meld /tmp/O2-opts /tmp/Os-opts
$ gcc -c -Q -O0 --help=optimizers > /tmp/O0-opts
$ gcc -c -Q -O1 --help=optimizers > /tmp/O1-opts
$ meld /tmp/O0-opts /tmp/O1-opts /tmp/O2-opts

$ cd gcc-optimization
main()
{
        int a=1;
        if (a>2)
                f();
}
$ gcc -O0 test.c
$ gcc -O2 test.c
change a to 3
$ gcc -O2 test.c
add __attribute__((optimize("O0"))) before main()
$ gcc -O2 test.c

#### ptrace
$ gcc hello.c -o hello
$ gcc debugger.c -o debugger
$ ./debugger hello

#### LD_debug
$ LD_DEBUG=libs ls
$ LD_DEBUG=help ls

#### gdb

#### core dump
$ ulimit -a
$ ulimit -c unlimited or 1024

$ cat /proc/sys/kernel/core_uses_pid //控制core文件是否添加pid作为扩展
$ sudo sh -c 'echo 1 > /proc/sys/kernel/core_uses_pid'

$ cd addr2line
$ gcc -g test.c -o test
$ readelf core.pid
$ gdb -c core.pid test
