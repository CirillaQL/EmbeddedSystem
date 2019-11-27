   EXPORT   sort       ;导出函数sort
   AREA  Exp103,CODE,READONLY
   
   CODE32


sort     ;函数名   
         ;入口参数 
         ;   R0<==buffer数据缓冲区首地址    
         ;   R1<==bufferlen 数据缓冲区数据个数

        SUBS R1,R1,#1      ;第一层循环    次数R1
L0      MOV R2,#0
        MOV R3,R1          ;第二层循环    次数R3
        
L1      LDR R6,[R0,R2]     ;R6<=[R0+R2]
        ADD R4,R2,#4
        LDR R7,[R0,R4]     ;R7<=[R0+R2+1]
        CMP R6,R7
        BGE L2

        STR R7,[R0,R2]     ;R6<R7交换
        STR R6,[R0,R4]

L2      ADD R2,R2,#4               
        SUBS  R3,R3,#1     ;二层循环控制
        BNE   L1    
        
        SUBS R1,R1,#1      ;一层循环控制
        BNE  L0     
EXIT    MOV PC,LR          ;函数返回    

   END
