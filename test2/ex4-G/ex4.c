#include <stdio.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
  
int main()  
{  
    int fd = 0;  
    char dst = 0;  
      
    /*���豸�ļ�*/  
    fd = open("/dev/ex1",O_RDWR);  
      
    /*д������*/  
    read(fd, &dst, sizeof(char));  
      
    printf("dst is %c\n",dst);  
    
    dst='3';
    write(fd, &dst, sizeof(char));
     close(fd);  
      
    return 0;     
  
}  
