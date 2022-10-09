/**
 * gcc -fprofile-arcs -ftest-coverage test.c
 */
#include <stdio.h>

int main(int argc, char **argv)
{
    int i, total;
    total = 0;

    for (i = 0; i < 10; i++)
        total += i;

    if (total != 45)
        printf ("Failure\n");
    else
        printf ("Success\n");

    return 0;
}
