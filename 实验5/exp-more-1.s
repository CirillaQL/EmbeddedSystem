   AREA  Exp101,CODE,READONLY
Address  EQU 0x40003000     ;常量定义

   ENTRY
   CODE32


start   LDR R0,=Address     ;R0<== 0x40003000
        MOV R1,#1           ;R1<== 1
        MOV R2,#2
	MOV R3,#98
LOOP 
	ADD R1,R1,R2
	ADDS R2,R2,#1
	SUBS R3,R3,#1
	BNE LOOP
	STR R1,[R0]	
   END
