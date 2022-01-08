#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include<stdlib.h>
int main()
{
	int fd;
	char Ubuff[]="this is user buffer from wrietr2\n";
	char Kbuff[100];
	fd=open("/dev/mydevice2",O_RDWR,0777);
	if(fd<0)
	{
		printf("\nerror opening device\n");
		exit(1);
	}
	write(fd,Ubuff,sizeof(Ubuff));
	//read(fd,Kbuff,50);
	//printf("\nthe data read from kernel is\n>>> %s <<<\n",Kbuff);
	close(fd);
    return 0;
}

