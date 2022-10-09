// test.c
#include <stdio.h>
#include <stdlib.h>
int main()
{
    char* c = (char*)malloc(sizeof(char)); // 调用malloc
    printf("c = %p\n", c);
    free(c); // 调用free，防止内存泄漏
    return 0;
}
