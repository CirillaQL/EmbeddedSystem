#include <stdio.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	int fd;
	char keyvalue;
	int keyvalue1;
	
	if ((fd = open("/dev/Kbd7279",0)) < 0)
	  {
		printf("cannot open /dev/Kbd7279\n");
		exit(0);
	  }
       for (;;){
		ioctl(fd, 0, 0);
		read(fd, &keyvalue, sizeof(char));
		if(keyvalue!=255){
			printf("you have pressed key %d \n", keyvalue);
			keyvalue1=(int)keyvalue;
			int fl = keyvalue1;
			char dst = 0;
			fl = open("/dev/ex1",O_RDWR);
			read(fl,&dst,sizeof(char));
			write(fl,&dst,sizeof(char))
			close(fl);
		}
	}
	close(fd);
	return 0;
}



