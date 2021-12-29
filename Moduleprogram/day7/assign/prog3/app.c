#include<stdio.h>
#include<sys/types.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
    int fd;
    fd=open("/dev/mydevice3", O_RDWR, 0777);
    if(fd<0)
    {
        printf("\nerror opening device\n");
        exit(1);

    }
    close(fd);
}