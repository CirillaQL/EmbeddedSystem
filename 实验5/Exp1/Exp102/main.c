
//清除存储器
void ClearMemory(unsigned char* pAddr,int nCnt)
{
  int i;
  if (nCnt<=0) return;                 
  for(i=0;i<nCnt;i++)
  {
     *pAddr++=(unsigned char)0x00;   //数据0x00写入存储器，并且地址指针加1
  }
}

//数据写入存储器
void WriteToMemory(unsigned char *pSnAddr,int nCnt,unsigned char *pDnAddr)
{
int i;
  if (nCnt<=0) return;                 
  for(i=0;i<nCnt;i++)
  {
     *pDnAddr++=*pSnAddr++;   //将pSnAddr指向的字节单元数据写入pDnAddr指向的字节单元
                              //pSnAddr加1，pDnAddr加1
  }
}


int main()
{
 int i;
 unsigned char buffer[256];
 
 for (i=1;i<=256;i++)  buffer[i-1]=(unsigned char)i;  //准备数据
 
 ClearMemory((unsigned char *)0x40003000,256);        
 
 WriteToMemory((unsigned char *)buffer,256,(unsigned char *)0x40003100); 

 return 0;
}