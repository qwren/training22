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
// switch to another teminal
$ ps aux
$ sudo kill -9 <pid>

3.2 mount namespace
$ sudo unshare --mount --fork /bin/bash
$ mkdir /tmp/tmpfs
$ df -h
$ mount -t tmpfs -o size=20m tmpfs /tmp/tmpfs
$ df -h
$ ls /proc/self/ns -l
// switch to another teminal
$ df -h
$ ls /proc/self/ns -l
$ kill -9 <pid>

3.3 uts namespace
$ sudo unshare --fork --uts /bin/bash
$ hostname -b test
$ hostname
// switch to another teminal
$ hostname
$ kill -9 <pid>

3.4 net namespace
$ sudo unshare --net --fork /bin/bash
$ ip a
// switch to another teminal
$ ip a
$ kill -9 <pid>

4. cgroup
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
$ sudo apt-get install docker.io
$ touch /etc/docker/daemon.json
{
    "registry-mirrors": ["https://alzgoonw.mirror.aliyuncs.com"]        
}
$ service docker restart
$ sudo docker run hello-world
