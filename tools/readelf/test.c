#include <stdio.h>
#include <stdlib.h>

int global1 = 1;
int global2;

void print(void)
{
	printf("this is a test.\n");
}
 
int main()
{
        int local_var;
        print();
        return 0;
}
