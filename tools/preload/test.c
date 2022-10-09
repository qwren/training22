#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
 
int main(int argc,char *argv[])
{
	int fd;
	if (fd=open("/home/qiaoweir/training/system/section1/README.txt",O_RDONLY)!=-1) {
		printf("openfile successed!\n");
	} else {
		printf("openfile error!\n");
	}

	close(fd);
	return 0;
}
