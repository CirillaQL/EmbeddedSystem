#include <stdio.h>

 int buffer[256];      //���ݻ�����
 int  bufferlen=0;     //���ݻ��������ݸ���

extern sort(int *buffer,int bufferlen);    //��ຯ��sort�ӿ�

//********�Ӽ���¼������********
int InputDataFromKeyBoard(int *pBuffer)
{
  int nCnt=0;
  int x;
  
 printf("�������뿪ʼ:\n");
 while(1)
  {
   scanf("%d",&x);                //��������������==��x
   pBuffer[nCnt++]=x;             //x���뻺����
   if((x==(int)-1)||(nCnt>=256))  //��������Ϊ-1 ��������
    {
    nCnt--;
    break;
    }
  }  
  printf("�����������!\n");
  return nCnt;
}

//********�����̨���buffer����********
void OutputConsole()
{
  int i;
  printf("���Buffer����:\n"); // �����̨�������
  for(i=0;i<bufferlen;i++)
  {  printf("%4d",buffer[i]); }
  printf("\n");

}



//********������********
int main()
{

  bufferlen=InputDataFromKeyBoard((int *)buffer);    //�Ӽ���¼������
  OutputConsole();                                   //�������������������̨
  sort((int *)buffer,bufferlen);                     //��������������
  OutputConsole();                                   //����������� 
 return 0;
}