#include <stdio.h>

 int buffer[256];      //数据缓冲区
 int  bufferlen=0;     //数据缓冲区数据个数

extern sort(int *buffer,int bufferlen);    //汇编函数sort接口

//********从键盘录入数据********
int InputDataFromKeyBoard(int *pBuffer)
{
  int nCnt=0;
  int x;
  
 printf("数据输入开始:\n");
 while(1)
  {
   scanf("%d",&x);                //键盘输入整型数==〉x
   pBuffer[nCnt++]=x;             //x存入缓冲区
   if((x==(int)-1)||(nCnt>=256))  //输入数据为-1 结束输入
    {
    nCnt--;
    break;
    }
  }  
  printf("数据输入结束!\n");
  return nCnt;
}

//********向控制台输出buffer数据********
void OutputConsole()
{
  int i;
  printf("输出Buffer数据:\n"); // 向控制台输出数据
  for(i=0;i<bufferlen;i++)
  {  printf("%4d",buffer[i]); }
  printf("\n");

}



//********主函数********
int main()
{

  bufferlen=InputDataFromKeyBoard((int *)buffer);    //从键盘录入数据
  OutputConsole();                                   //将输入数据输出到控制台
  sort((int *)buffer,bufferlen);                     //对输入数据排序
  OutputConsole();                                   //输出排序数据 
 return 0;
}