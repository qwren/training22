1. libvirt
// kvm-qemu, virt-manager
$ virsh help
$ virsh help vol-clone  //check specific command.
$ virsh list  //list all actived guests.
$ virsh list --all

$ service libvirtd start
$ service libvirtd status
$ vim /etc/libvirt/libvirt.conf
$ virsh connect qemu:///system
$ virsh capabilities

$ qemu-img create -f qcow2 /home/rqw/work/virt/libvirt/demo.qcow2 20G
$ qemu-img /home/rqw/work/virt/libvirt/demo.qcow2
$ vim demo.xml
$ virsh create demo.xml
$ virsh dominfo demo

2. network
2.1 tap/tun
$ modinfo tun
$ modinfo tap
$ ls /dev/net/tun

$ gcc mytun.c -o mytun
# 创建并监听 tun 设备需要 root 权限
$ sudo mytun
# 创建的设备并不会以文件的形式出现在 /dev/ 下
$ ls /sys/class/net/tun0/
$ ip addr ls
7: tun0: <POINTOPOINT,MULTICAST,NOARP> mtu 1500 qdisc noop state DOWN group default qlen 500
    link/none   //状态为 down
# 为 tun0 设置 ip 地址
$ sudo ip addr add 192.168.3.11/24 dev tun0
# 启动 tun0 这个接口，这一步会自动向路由表中添加将 192.168.3.11/24 路由到 tun0 的策略
$ sudo ip link set tun0 up
# 确认上一步添加的路由策略是否存在
$ ip route ls
192.168.3.0/24 dev tun0 proto kernel scope link src 192.168.3.11
# 此时再查看接口，发现 tun0 状态为 unknown
$ ip addr ls
# 使用 tcpdump 尝试抓下 tun0 的数据，会阻塞在这里，等待数据到达
$ sudo tcpdump -i tun0
# 使用 ping 命令，制造一个该网段的流量,流量就会被转发给 mytun 程序，因为 mytun 啥数据也没回，自然丢包率 100%
$ ping -c 4 192.168.3.12
PING 192.168.3.12 (192.168.3.12) 56(84) bytes of data.

--- 192.168.3.12 ping statistics ---
4 packets transmitted, 0 received, 100% packet loss, time 3060ms

# 除了在应用中通过克隆设备 /dev/net/tun 和 ioctl 系统调用创建虚拟设备，还可以通过 ip tuntap 命令创建
# 创建 tun/tap 设备
$ ip tuntap add dev tap0 mod tap # 创建 tap
$ ip tuntap add dev tun0 mod tun # 创建 tun
# 删除tun/tap设备
$ ip tuntap del dev tap0 mod tap # 删除 tap
$ ip tuntap del dev tun0 mod tun # 删除 tun
# 设置 ip 地址，up 设备
$ ip address add dev tap0 192.168.3.0/24
$ ip link set dev tap0 up

2.2 veth
$ sudo ip netns add ns1
$ sudo ip netns add ns2
$ ip netns list
# Create a link between the two namespaces.
$ sudo ip link add veth_ns1 type veth peer name veth_ns2
$ ls /sys/class/net/
# Attach the veth pair to the corresponding network namespace
$ sudo ip link set veth_ns1 netns ns1
$ sudo ip link set veth_ns2 netns ns2
$ sudo ip netns exec ns1 ip addr list
$ sudo ip netns exec ns2 ip addr list
# Bring all interfaces up
$ sudo ip netns exec ns1 ip link set veth_ns1 up
$ sudo ip netns exec ns2 ip link set veth_ns2 up
# Configure IP for namespaces
$ sudo ip netns exec ns1 ip addr add 10.1.1.1/24 dev veth_ns1
$ sudo ip netns exec ns2 ip addr add 10.1.1.2/24 dev veth_ns2
$ sudo ip netns exec ns1 ip addr list
# Verify the connectivity between namespaces
$ sudo ip netns exec ns1 ping 10.1.1.2
$ sudo ip netns exec ns2 ping 10.1.1.1
# loopback in namespace
$ sudo ip netns exec ns1 ip link set lo up
$ sudo ip netns exec ns1 ping localhost

2.3 linux bridge
$ sudo ip netns add ns1
$ sudo ip netns add ns2
$ ip netns list
# Create a Linux bridge
$ sudo ip link add br0 type bridge
# Bring the bridge up
$ sudo ip link set dev br0 up
$ ip addr list
# Create veth pairs
$ sudo ip link add veth_ns1 type veth peer name veth_ns1_br0
$ sudo ip link add veth_ns2 type veth peer name veth_ns2_br0
$ ip addr list
# Attach veth pairs to the corresponding network namespace and the bridge
$ sudo ip link set veth_ns1 netns ns1
$ sudo ip link set veth_ns1_br0 master br0
$ sudo ip link set veth_ns2 netns ns2
$ sudo ip link set veth_ns2_br0 master br0
# Bring all interfaces up
$ sudo ip netns exec ns1 ip link set veth_ns1 up
$ sudo ip link set veth_ns1_br0 up
$ sudo ip netns exec ns2 ip link set veth_ns2 up
$ sudo ip link set veth_ns2_br0 up
# Configure IP for namespaces
$ sudo ip netns exec ns1 ip addr add 10.1.1.1/24 dev veth_ns1
$ sudo ip netns exec ns2 ip addr add 10.1.1.2/24 dev veth_ns2
$ ip link show master br0
# Test connectivity between the namespaces
$ sudo ip netns exec ns1 ping 10.1.1.2
$ sudo ip netns exec ns2 ping 10.1.1.1

# Connect Host to Network Namespaces
$ sudo ip addr add 10.1.1.4/24 dev br0
$ route
$ cat /proc/sys/net/ipv4/ip_forward
$ sudo sysctl -w net.ipv4.ip_forward=1
or
$ sudo echo 1 > /proc/sys/net/ipv4/ip_forward
$ ping 10.1.1.1
$ ping 10.1.1.2
# Connect Network Namepsaces to Host
$ sudo ip netns exec ns1 route
$ sudo ip netns exec ns1 route add default gw 10.1.1.4
$ sudo ip netns exec ns1 route
$ sudo ip netns exec ns1 ping <host_ip>

# Connect another host from Network Namespaces
# 在NAT表中添加一个规则，将源IP地址为10.1.1.9/24的数据包进行源地址转换
$ sudo iptables -s 10.1.1.0/24 -t nat -A POSTROUTING -j MASQUERADE
$ sudo ip netns exec ns1 ping <ip>

3. namespace
3.1 pid namespace
$ sudo unshare --fork --pid --mount-proc /bin/bash
$ ps aux
$ top
// switch to another teminal
$ ps aux
$ sudo kill -9 <top_pid>
$ sudo kill -9 <unshare_pid>

3.2 mount namespace
$ sudo unshare --mount --fork /bin/bash
$ mkdir /tmp/tmpfs
$ df -h
$ mount -t tmpfs -o size=20m tmpfs /tmp/tmpfs
$ vim /proc/mounts
$ df -h
$ ls /proc/self/ns -l
// switch to another teminal
$ df -h
$ ls /proc/self/ns -l
$ kill -9 <pid>

3.3 IPC namespace
$ ls /proc/self/ns -l
$ ipcs
$ sudo unshare --ipc --fork /bin/bash
$ ls /proc/self/ns -l
$ ipcmk -Q
$ ipcs
// switch to another teminal
$ ipcs

3.4 uts namespace
$ sudo unshare --fork --uts /bin/bash
$ hostname -b test
$ hostname
// switch to another teminal
$ hostname
$ kill -9 <pid>

3.5 net namespace
$ sudo unshare --net --fork /bin/bash
$ ip a
// switch to another teminal
$ ip a
$ kill -9 <pid>

3.6 user namespace
$ echo $USER
$ echo $HOME
$ id
$ unshare -U –fork /bin/bash
$ echo $USER
$ echo $HOME
$ whoami
$ id
# ns1的bash进程号为
$ echo $$
# 新建一个会话窗口，在父级user namespace中执行
$ echo '0 1000 500' | sudo tee /proc/<id>/uid_map >/dev/null
$ echo '0 1000 500' | sudo tee /proc/<id>/gid_map >/dev/null
# 在 user namespace中
$ id

4. cgroup
$ mount | grep cgroup
$ vim /proc/self/cgroup
# hierarchy_id:controller_list:cgroup_path
$ vim /proc/cgroups
4.1 CPU cgroup
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

4.2 memory cgroup
# 该值会影响 Swap 分区的使用，假设该参数设置为 100MB，但是 swap 还剩余 100MB。
# 则在该 cgroup 的进程看来，只要申请的内存总量 >= 200MB 才会被 Kill。因此，
# 在测试该参数时，使用 sudo swapoff -a 先关闭 Swap 以排除 Swap 的干扰。
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

5. docker
$ sudo apt-get update
$ sudo apt-get install apt-transport-https ca-certificates curl software-properties-common
$ sudo mkdir -p /etc/apt/keyrings
$ sudo curl -fsSL http://mirrors.ustc.edu.cn/docker-ce/linux/ubuntu/gpg -o /etc/apt/keyrings/docker.asc
$ sudo chmod a+r /etc/apt/keyrings/docker.asc
$ echo "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.asc] http://mirrors.ustc.edu.cn/docker-ce/linux/ubuntu $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
$ sudo apt update
$ sudo apt install docker-ce docker-ce-cli containerd.io
$ sudo vim /etc/docker/daemon.json
{
    "registry-mirrors": ["https://ccr.ccs.tencentyun.com","https://ccr.cs.tencentyun.com"]
}
$ sudo service docker restart
$ sudo docker run hello-world

6. OCI
6.1 image
# 拉取 nginx:latest 镜像到本地
$ docker pull nginx:latest
# 以 tarball 形式保存 nginx 镜像
$ docker image save -o nginx.tar nginx:latest
# 创建并解压保存下来的 tarball 到指定文件夹
$ mkdir nginx && tar -xvf nginx.tar -C ./nginx
$ cd nginx
$ tree
$ cat index.json | jq
# 观察下第一个 layer 
$ mkdir layer0
$ tar -xvf blobs/sha256/... -C layer0/
$ tree -L 1 layer0

6.2. runtime/runc
$ mkdir rootfs
# 获得容器的 rootfs 并放置在指定文件夹
$ sudo docker export $(sudo docker create nginx:latest) | tar -C rootfs -xvf -
# 通过 Runtime 指令生成默认的 config.json 文件
$ runc spec
$ tree -L 2
$ cat config.json

# 以foreground模式运行容器
$ sudo runc run testid
# 最终进入了一个新创建的容器内的 shell
# 切换到另一个终端
$ pstree | grep runc

# 将config.json里terminal字段改成false, 把 "args": ["sh"] 改为 "args": ["sleep", “infinity"]"]
# 以分离模式运行容器
$ sudo runc run testid --detach
$ pstree
 
$ sudo runc create testid
$ sudo runc list
$ sudo runc start testid
$ sudo runc state testid
$ sudo runc exec -t testid /bin/sh
$ sudo runc kill testid 9
$ runc delete testid

7. LXC
$ sudo apt-get install lxc
# 检查当前内核是否支持 LXC。如果所有结果都是“enable”，说明内核支持
$ lxc-checkconfig
$ sudo vim /etc/default/lxc-net
# LXC 自动创建一块桥接网卡lxcbr0，可以在 /etc/lxc/default.conf 中设置
$ ifconfig lxcbr0

### 全局默认配置
/etc/lxc/default.conf

### 全局USER默认配置
$ mkdir -p ~/.config/lxc
$ cp /etc/lxc/default.conf ~/.config/lxc/default.conf
# 添加user用户相关权限，比如允许用户创建两个设备连接到lxcbr0网桥
$ echo "$USER veth lxcbr0 2" | sudo tee -a /etc/lxc/lxc-usernet

### 单个容器配置
# 每创建好一个容器，会在以下默认路径生成配置文件
# 容器文件路径
/var/lib/lxc/<container>
# 配置文件路径
/var/lib/lxc/<container>/config

### 默认创建的容器为特权容器
# 默认情况下，下面命令会创建一个最小的 Ubuntu 环境，版本号与你的宿主机一致
# 容器被放到 /var/lib/lxc/<容器名> 这个目录下，容器的根文件系统放在 /var/lib/lxc/<容器名>/rootfs 目录下。
# 创建过程中下载的软件包保存在 /var/cache/lxc 目录下面，当另外建一个一样的容器时，可以省去很多下载时间。
$ sudo lxc-create -n <container-name> -t ubuntu
# 检查容器状态
$ sudo lxc-ls -f
$ sudo lxc-info -n <container-name>
# 启动后的容器，状态更新为RUNNING
$ sudo lxc-start -n <container-name>
$ sudo lxc-ls -f
# 可以看到容器的网络接口（比如veth5ptnDA）自动与 LXC 内部网桥（lxcbr0）连上
$ brctl show lxcbr0

###进入容器的方式有三种
# 打开容器控制台
$ sudo lxc-console -n <container-name>
# Spawn bash directly in the container (bypassing the console login)
$ sudo lxc-attach -n <container-name>
# 也可以ssh到容器
$ ssh ubuntu@<container_ip>
# 在容器内查看路由和ip
$ routel
$ ip a
$ ping <lxcbr0 ip>
# 使用“Crtl+a q”组合键退出控制台
# 退出容器
$ sudo lxc-stop -n <container-name> <-k>

#先停掉容器然后进行删除
$ sudo lxc-stop --name <container-name>
$ sudo lxc-destroy --name <container-name>

### 创建非特权容器，需要在配置文件里添加
lxc.idmap = u 0 100000 65536
lxc.idmap = g 0 100000 65536

### 使用libvirt管理lxc容器
$ virsh -c lxc:/// create lxc1.xml
$ virsh -c lxc:/// list --all
$ virsh -c lxc:/// console lxc1

8. container storage
### 临时存储
# 检查内核支持的fs
$ vim /proc/filesystems
# 查看容器当前使用的存储驱动
$ sudo docker info

8.1 AUFS
# Ubuntu 22.04没有了AUFS
$ sudo apt-get install aufs-tools
# 创建目录结构
$ mkdir base mnt top
$ cd base
$ touch common.txt
$ touch hello.txt
$ cd ../top
$ touch foo.txt
$ cp ../base/common.txt .
$ cd ..
$ tree
.
├── base
│   ├── common.txt
│   └── hello.txt
├── mnt
└── top
    ├── common.txt
    └── foo.txt
# 使用 aufs，把 base 和 top 一起 mount 到 ./mnt 目录
# 默认情况下，-o 后面的第一个目录是以可读写模式挂载的，剩下的目录都是只读模式（和 docker 容器模型非常一致）
$ sudo mount -t aufs -o br=./top:./base none ./mnt
$ tree
.
├── base
│   ├── common.txt
│   └── hello.txt
├── mnt
│   ├── common.txt
│   ├── foo.txt
│   └── hello.txt
└── top
    ├── common.txt
    └── foo.txt
# 修改 common.txt 文件，会发现只有 top 目录对应的内容发生了变化，base 下面的内容会保持不动
$ echo changed > ./mnt/common.txt
$ cat top/common.txt
changed
$ cat base/common.txt
# 修改 hello.txt 文件，base/hello.txt 并没有变化，新建了一个 top/hello.txt 文件，所有的操作都是在这个文件进行
$ echo hello, world > mnt/hello.txt
$ tree
.
├── base
│   ├── common.txt
│   └── hello.txt
├── mnt
│   ├── common.txt
│   ├── foo.txt
│   └── hello.txt
└── top
    ├── common.txt
    ├── foo.txt
    └── hello.txt

8.2 OverlayFS
# 创建目录结构
$ mkdir upper lower merged work
$ mkdir upper lower merged work
$ echo "I'm from lower!" > lower/in_lower.txt 
$ echo "I'm from upper!" > upper/in_upper.txt
# `in_both` is in both directories
$ echo "I'm from lower!" > lower/in_both.txt 
$ echo "I'm from upper!" > upper/in_both.txt
$ tree
.
├── lower
│   ├── in_both.txt
│   └── in_lower.txt
├── merged
├── upper
│   ├── in_both.txt
│   └── in_upper.txt
└── work
# 将lower, upper目录联合挂载到merged目录
$ sudo mount -t overlay overlay -o lowerdir=lower,upperdir=upper,workdir=work merged
$ mount | grep overlay
$ ls merged/
in_both.txt  in_lower.txt  in_upper.txt
# in_both.txt文件来自upper目录(覆盖了lower目录中的同名文件)
$ cat merged/in_both.txt
from upper
# merged目录中创建一个文件，会发现新建的文件出现在upper目录，即upper目录为读写层
$ echo 'new file' > merged/new_file
$ ls -l */new_file
-rw-rw-r-- 1 rqw rqw 9 9月  25 23:08 merged/new_file
-rw-rw-r-- 1 rqw rqw 9 9月  25 23:08 upper/new_file
# 删除merged目录中的in_both.txt文件，lower目录里的in_both.txt文件不变,upper目录里的变成character device?
$ ls -l upper/in_both.txt  lower/in_both.txt  merged/in_both.txt
ls: cannot access 'merged/in_both.txt': No such file or directory
-rw-rw-r-- 1 rqw  rqw    16 9月  25 23:01 lower/in_both.txt
c--------- 1 root root 0, 0 9月  25 23:12 upper/in_both.txt

### 持久存储
8.3 bind mount
$ mkdir source target
$ touch source/foo target/bar
$ tree     
.
├── source
│   └── foo
└── target
    └── bar
# Bind mount source to target
$ sudo mount --bind source target
# Now two directories have the same structure
$ tree    
.
├── source
│   └── foo
└── target
    └── foo
# Add a new file in /tmp/source
$ echo "Hello" > source/hello
# It appears in /tmp/target
$ cat target/hello
Hello
# Vice versa
$ echo "World" > target/world
$ cat source/world
World

8.4 volume
$ sudo docker volume create myvol
$ sudo docker volume ls
DRIVER             VOLUME NAME
local              myvol
$ sudo docker volume inspect myvol
# Driver和Scope都是 local 。这意味着卷使用默认 local 驱动创建，只能用于当前Docker主机上的容器。
# Mountpoint 属性说明卷位于Docker主机上的位置。
[
    {
    "CreatedAt": "2024-01-12T12:12:10Z",
        "Driver": "local",
        "Labels": null,
        "Mountpoint": "/var/lib/docker/volumes/myvol/_data",
        "Name": "myvol",
        "Options": null,
        "Scope": "local"

    }
]
$ sudo docker volume rm myvol
# 使用v标志启动容器
$ sudo docker run -d --name voltest -v myvol2:/app nginx:latest
# 使用mount标志启动容器
$ sudo docker run -dit --name voltest --mount source=myvol2,target=/app nginx:latest
# 执行 exec 连接到容器并向卷中写入一部分数据
$ sudo docker container exec -it voltainer sh
# echo "volume tset" > /app/file1
# cat /app/file1
volume test
# exit
# 返回到Docker主机Shell中，然后删除容器
$ sudo docker container stop voltest
$ sudo docker container rm voltest
# 即使容器被删除，卷依旧存在
$ sudo docker volume ls
# 进入到其在主机的挂载点并查看前面写入的数据是否还在
$ sudo cat /var/lib/docker/volumes/myvol2/_data/file1
volume test
$ sudo docker volume rm myvol2

9. container network
$ sudo docker network ls
# 默认的“bridge”网络被映射到内核中为“ docker0 ”的Linux网桥
$ sudo docker network inspect bridge
$ ifconfig or ip link show docker0

###桥接
# 创建新的桥接网络“brtest”
$ sudo docker network create -d bridge brtest
$ sudo docker network ls
# 新桥接网络brtest与新的网桥（br-xxx）对应
$ brctl show
# 创建一个新的容器，并接入到新建桥接网络 brtest 当中
$ sudo docker run -dit --name c1 --network brtest nginx:latest
# 容器加入到了新的brtest桥接网络中
$ sudo docker network inspect brtest
# 容器的网络接口连接到了新的网桥
$ brctl show
# 在相同网络中继续接入新的容器，两个容器可以互相ping通
$ sudo docker run -dit --name c2 --network brtest nginx:latest
$ sudo docker network inspect brtest
$ brctl show
$ sudo docker container exec -it c2 sh //默认的nginx容器rootfs没有ping命令

# 使用端口映射将容器端口映射到Docker主机端口上，之后任何发送到该host端口的流量，都会被转发到容器
$ sudo docker run -dit --name web --publish 5000:80 --network brtest nginx:latest
# 表示容器80端口已经映射到Docker主机所有接口上的5000端口
$ sudo docker port web
# 通过浏览器访问localhost:5000，可以看到nginx的welcome页面






