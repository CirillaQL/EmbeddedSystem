#include <stdio.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
  
int main()  
{  
    int fd = 0;  
    char dst = 0;  
      
    /*打开设备文件*/  
    fd = open("/dev/ex1",O_RDWR);  
      
    /*写入数据*/  
    read(fd, &dst, sizeof(char));  
      
    printf("dst is %c\n",dst);  
    
    dst='3';
    write(fd, &dst, sizeof(char));
     close(fd);  
      
    return 0;     
  
}  
