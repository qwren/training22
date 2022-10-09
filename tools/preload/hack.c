#include <stdio.h>
#include <stdlib.h>

extern int __open(char *,int,int);

//打开文件
int open(char * path,int flags,int mode)
{
	//输出打开的文件名
	printf("open :%s\n",path);
	return __open(path,flags,mode);
}
