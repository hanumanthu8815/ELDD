#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include<stdlib.h>
int main()
{
	int fd;
	//char Ubuff[]="this is user buffer---sending data to kernel";
	char Kbuff[100];
	fd=open("/dev/sema_device",O_RDWR,0777);
	if(fd<0)
	{
		printf("\nerror opening device\n");
		exit(1);
	}
	//write(fd,Ubuff,sizeof(Ubuff));
	read(fd,Kbuff,100);
	printf("\nthe data read from kernel in read2 is\n>>> %s <<<\n",Kbuff);
	close(fd);
    return 0;
}

