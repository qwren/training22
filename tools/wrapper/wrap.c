#include <stdio.h>
#include <stdlib.h>
void* __real_malloc(size_t size); // 只声明不定义__real_malloc
void* __wrap_malloc(size_t size) // 定义__wrap_malloc
{
    printf("__wrap_malloc called, size:%zd\n", size); // log输出
    return __real_malloc(size); // 通过__real_malloc调用真正的malloc
}
