#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
 
int main()
{
        int i=0;
	printf("%s %d\n", __func__, __LINE__);
	while(1) {
		volatile int j,k;
		int q;
		for (i=0;i<1000000;i++);
		printf("hello %d\n", q++);
		for (j=0;j<1000000;j++);
		printf("hello %d\n", q++);
		for (k=0;k<1000000;k++);
		printf("hello %d\n", q++);
	}
        return 0;
}
