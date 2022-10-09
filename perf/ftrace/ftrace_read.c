#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

void ftrace_read()
{
    int fd;
    char buf[4096];

    fd = open("file", O_RDONLY);
    read(fd, buf, 4096);
    read(fd, buf, 4096);
}

int main()
{
    sleep(30);
    ftrace_read();
    return 0;
}

