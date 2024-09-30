#!/usr/bin/env bash

# sudo ./utsns.sh

script="hostname && hostname --nis || true"

# 创建新进程，并为该进程创建一个 UTS Namespace（-u）
# 更多参见：https://man7.org/linux/man-pages/man1/unshare.1.html

# 设置新的 hostname 和 domainname
unshare -u /bin/bash -c "hostname new-hostname && domainname new-domainname \
        && echo '=== new uts namespace process ===' && set -x && $script" &
pid1=$!

sleep 5
# 创建新的进程（不创建 Namespace），并执行测试命令
/bin/bash -c "echo '=== old namespace process ===' && set -x && $script" &
pid2=$!

wait $pid1
wait $pid2
