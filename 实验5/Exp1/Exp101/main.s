   AREA  Exp101,CODE,READONLY
Address  EQU 0x40003000     ;��������
N        EQU 0x11223344

   ENTRY
   CODE32


start   LDR R0,=Address     ;R0<== 0x40003000
        LDR R1,=N           ;R1<== 0x11223344
        STR R1,[R0]
L       LDR R1,[R0]         ;R1<==[0x40003000]
        MOV R1,R1,ROR #8    ;R1ѭ������8λ==��R1
        STR R1,[R0]         ;R1==>[0x40003000]
        B L                 ;ת�Ƶ�L
   END