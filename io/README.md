### filesystem

1. 符号链接硬链接
$ touch a.txt
$ ln a.txt b.txt
$ ln -li
$ ln -s a.txt c.txt
$ ln -li

2. 模拟文件一致性问题
* create image and format
$ dd if=/dev/zero of=image bs=4096 count=1024
$ mkfs.ext4 -b 4096 image 

* chech block bitmap and inode bitmap
$ dumpe2fs image

* read inode bitmap
$ dd if=image bs=4096 skip=18 | hexdump -C -n 32

* mount image, create a file, and check its inode
$ mkdir aaa
$ mount -o loop image aaa
$ cd aaa
$ touch hello.txt
$ ls -li hello.txt

* umount image, and check inode bitmap
$ umount aaa
$ dd if=image bs=4096 skip=18 | hexdump -C -n 32

* modify inode bitmap (destroy)
$ vim -b image
%!xxd -g 1
%!xxd -r
$ dd if=image bs=4096 skip=18 | hexdump -C -n 32

fsck can not idenify this error.
$ fsck.ext4 image

but file "hello.txt" still exists.
$ mount -o loop image aaa
$ cd aaa
$ ls -li

!!!!!! but this FS still work normally

* create new file
$ touch bye.txt
IO error will happen!!

3. debugfs
$ sudo debugfs -R 'stat /home/qiaoweir/fork1.c' /dev/sda1
$ sudo dd if=/dev/sda1 of=1 skip=$((563005*8)) bs=512 count=1
$ sudo debugfs -R 'icheck block_num' /dev/sda1 //通过block号查看inode号
$ sudo debugfs -R 'ncheck inode_num' /dev/sda1 //通过inode号查看文件路径


### IO

1. dm-verity
* create a stand-in data partition and fill it with a few files:

root# truncate -s 10G data_partition.img
root# mkfs -t ext4 data_partition.img
root# mkdir mnt
root# mount -o loop data_partition.img mnt/
root# echo "hello" > mnt/one.txt
root# echo "integrity" > mnt/two.txt
root# umount mnt/

* create a stand-in partition to hold the hash data:

root# truncate -s 100M hash_partition.img

* hash the data and store it to our hash partition:

root# veritysetup -v --debug format data_partition.img hash_partition.img
# cryptsetup 2.2.2 processing "veritysetup -v --debug format data_partition.img hash_partition.img"
# Running command format.
# Allocating context for crypt device hash_partition.img.
# Trying to open and read device hash_partition.img with direct-io.
# Initialising device-mapper backend library.
# Formatting device hash_partition.img as type VERITY.
# Crypto backend (OpenSSL 1.1.1f  31 Mar 2020) initialized in cryptsetup library version 2.2.2.
# Detected kernel Linux 5.10.0 x86_64.
# Setting ciphertext data device to data_partition.img.
# Trying to open and read device data_partition.img with direct-io.
# Hash creation sha256, data device data_partition.img, data blocks 2621440, hash_device hash_partition.img, offset 1.
# Using 4 hash levels.
# Data device size required: 10737418240 bytes.
# Hash device size required: 84557824 bytes.
# Updating VERITY header of size 512 on device hash_partition.img, offset 0.
VERITY header information for hash_partition.img
UUID:                   caa7d6ed-5698-4593-975b-4d1c8230114f
Hash type:              1
Data blocks:            2621440
Data block size:        4096
Hash block size:        4096
Hash algorithm:         sha256
Salt:                   32c6755d934b56ca6f8e4b8c87328b6647f25e48b262849d012fd08bc559ff69
Root hash:              ce4677d088d05ad7660267f1d52bce4834b3def3dfbf72c0dc7ae0eb684a7826
# Releasing crypt device hash_partition.img context.
# Releasing device-mapper backend.
# Closing read write fd for hash_partition.img.
Command successful.

* mount the data partition, need to choose a device mapper name (verity-test)

root# veritysetup open \
> data_partition.img \
> verity-test \
> hash_partition.img \
> ce4677d088d05ad7660267f1d52bce4834b3def3dfbf72c0dc7ae0eb684a7826

At this point you should see /dev/mapper/verity-test appear in the file system. All we need to do is mount it like a regular disk, and we have a protected file system!

root# mkdir mnt
root# mount /dev/mapper/verity-test mnt/
root# cat mnt/one.txt mnt/two.txt
hello
integrity

2. IO cgroup

