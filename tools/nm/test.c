#include<time.h>

int global1;
int global2=3;

static int static_global1;
static int static_global2=3;

void foo()
{
    static int internal1;
    static int internal2=3;
    time(0);
}

static void bar()
{

}

int main(void)
{
    int local1;
    int local2=3;
    foo();
    return 0;
}
