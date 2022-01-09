#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


int main()
{
    int fd;
    int arr[2]={5,5};
    int res=0;
    fd=open("/dev/MUL",O_RDWR);
    if(fd<0)
    {
        printf("ERROR IN ADDING OPENING..\n");
        return -1;
    }
    write(fd,arr,sizeof(arr));
    read(fd,&res,4);
    printf("ADDITION :%d\n",res);
    close(fd);
}