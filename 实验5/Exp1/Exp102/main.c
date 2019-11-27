
//����洢��
void ClearMemory(unsigned char* pAddr,int nCnt)
{
  int i;
  if (nCnt<=0) return;                 
  for(i=0;i<nCnt;i++)
  {
     *pAddr++=(unsigned char)0x00;   //����0x00д��洢�������ҵ�ַָ���1
  }
}

//����д��洢��
void WriteToMemory(unsigned char *pSnAddr,int nCnt,unsigned char *pDnAddr)
{
int i;
  if (nCnt<=0) return;                 
  for(i=0;i<nCnt;i++)
  {
     *pDnAddr++=*pSnAddr++;   //��pSnAddrָ����ֽڵ�Ԫ����д��pDnAddrָ����ֽڵ�Ԫ
                              //pSnAddr��1��pDnAddr��1
  }
}


int main()
{
 int i;
 unsigned char buffer[256];
 
 for (i=1;i<=256;i++)  buffer[i-1]=(unsigned char)i;  //׼������
 
 ClearMemory((unsigned char *)0x40003000,256);        
 
 WriteToMemory((unsigned char *)buffer,256,(unsigned char *)0x40003100); 

 return 0;
}