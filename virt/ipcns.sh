#!/usr/bin/env bash

# sudo ./ipcns.sh

script="ipcs -q"

# unshare -m -i -p 创建新进程，并为该进程创建一个 ipc Namespace（-i）
# 更多参见：https://man7.org/linux/man-pages/man1/unshare.1.html
# 为了测试方便，同时创建 Mount Namespace (-m) 和 PID Namespace (-p)

# 注意 unshare 会自动取消进程的所有共享，因此不需要手动执行：mount --make-rprivate /
# 更多参见：https://man7.org/linux/man-pages/man1/unshare.1.html 的 --propagation 参数说明

# mount -t proc proc /proc 挂载 proc 文件系统，等价于 mount("proc", "/proc", "proc", 0, NULL) 系统调用
# 更多参见：https://man7.org/linux/man-pages/man8/mount.8.html

# ipcmk -Q 创建一个 System V 消息队列

# 注意：bash 的最后一条命令将不会 fork 进程，所以在最后补充一个 sleep ，让命令在新的进程执行！
# https://unix.stackexchange.com/questions/466496/why-is-there-no-apparent-clone-or-fork-in-simple-bash-command-and-how-its-done
unshare -m -i -p /bin/bash -c "/bin/bash -c 'mount -t proc proc /proc \
        && ipcmk -Q \
            && echo \"=== new ipc namespace process ===\" && set -x && $script' && sleep 10" &
pid1=$!

sleep 5
# 创建新的进程（不创建 Namespace），并执行测试命令
/bin/bash -c "echo '=== old namespace process ===' && set -x && $script" &
pid2=$!

wait $pid1
wait $pid2
