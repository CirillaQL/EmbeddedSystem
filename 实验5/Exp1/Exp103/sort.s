   EXPORT   sort       ;��������sort
   AREA  Exp103,CODE,READONLY
   
   CODE32


sort     ;������   
         ;��ڲ��� 
         ;   R0<==buffer���ݻ������׵�ַ    
         ;   R1<==bufferlen ���ݻ��������ݸ���

        SUBS R1,R1,#1      ;��һ��ѭ��    ����R1
L0      MOV R2,#0
        MOV R3,R1          ;�ڶ���ѭ��    ����R3
        
L1      LDR R6,[R0,R2]     ;R6<=[R0+R2]
        ADD R4,R2,#4
        LDR R7,[R0,R4]     ;R7<=[R0+R2+1]
        CMP R6,R7
        BGE L2

        STR R7,[R0,R2]     ;R6<R7����
        STR R6,[R0,R4]

L2      ADD R2,R2,#4               
        SUBS  R3,R3,#1     ;����ѭ������
        BNE   L1    
        
        SUBS R1,R1,#1      ;һ��ѭ������
        BNE  L0     
EXIT    MOV PC,LR          ;��������    

   END
