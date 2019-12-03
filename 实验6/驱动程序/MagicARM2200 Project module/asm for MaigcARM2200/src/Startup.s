;/****************************************Copyright (c)**************************************************
;**                               Guangzou ZLG-MCU Development Co.,LTD.
;**                                      graduate school
;**                                 http://www.zlgmcu.com
;**
;**--------------File Info-------------------------------------------------------------------------------
;** File name: 			Startup.s
;** Last modified Date:  2004-09-17
;** Last Version: 		1.0
;** Descriptions: 		The start up codes for LPC2100, including the initializing codes for the entry point of exceptions and the stacks of user tasks.
;**				Every project should have a independent copy of this file for related modifications
;**------------------------------------------------------------------------------------------------------
;** Created by: 			Chenmingji
;** Created date:   		2004-02-02
;** Version:			1.0
;** Descriptions: 		The original version
;**
;**------------------------------------------------------------------------------------------------------
;** Modified by: 		Chenmingji
;** Modified date:		2004-09-17
;** Version:			1.01
;** Descriptions: 		Modified the bus setting to adapt for many common situations 
;**
;**------------------------------------------------------------------------------------------------------
;** Modified by: 		Chenmingji
;** Modified date:		2004-09-17
;** Version:			1.02
;** Descriptions: 		Added codes to support the enciphering of the chip
;**
;**------------------------------------------------------------------------------------------------------
;** Modified by: 		Chenmingji
;** Modified date:		2004-09-17
;** Version:			1.04
;** Descriptions: 		Renewed the template, added codes to support more compilers 
;**
;**------------------------------------------------------------------------------------------------------
;** Modified by: 		
;** Modified date:
;** Version:	
;** Descriptions: 
;**
;********************************************************************************************************/
		    INCLUDE		LPC2294.INC			; IMPORT The Head File ����ͷ�ļ�

;/* Define the Bus Speed */
BCFG_08DEF	EQU	    0x00000000      ;//  8Bit Bus
BCFG_16DEF	EQU	    0x10000400      ;// 16Bit Bus
BCFG_32DEF	EQU	    0x20000400      ;// 32Bit Bus

;//                             |    IDCY    |    WST1    |    WST2	
;//                             | Idle width | Read width | Write width
;//                             | 0x00 ~ 0x0f| 0x00 ~ 0x1f| 0x00~0x1f
BCFG_FLASH	EQU		(BCFG_16DEF | (0x00<<00) | (0x03<<05) | (0x02<<11))   ;// For 90ns Flash
BCFG_PSRAM  EQU		(BCFG_16DEF | (0x00<<00) | (0x03<<05) | (0x02<<11))   ;// For 70ns PSRAM
BCFG_CS2	EQU		(BCFG_16DEF | (0x0f<<00) | (0x1f<<05) | (0x1f<<11))   ;// Blank 
BCFG_CS3    EQU		(BCFG_16DEF | (0x01<<00) | (0x03<<05) | (0x03<<11))   ;// For Peripheral Equipment


;The imported labels    
;������ⲿ�����������
    IMPORT  MAIN                            ;������

;The emported labels    
;���ⲿʹ�õı����������
    EXPORT  Reset

    CODE32

    AREA    vectors,CODE,READONLY
        ENTRY
;interrupt vectors
;�ж�������
Reset
        LDR     PC, ResetAddr
        LDR     PC, UndefinedAddr
        LDR     PC, SWI_Addr
        LDR     PC, PrefetchAddr
        LDR     PC, DataAbortAddr
        DCD     0xb9205f80
        LDR     PC, [PC, #-0xff0]
        LDR     PC, FIQ_Addr

ResetAddr           DCD     ResetInit
UndefinedAddr       DCD     Undefined
SWI_Addr            DCD     SoftwareInterrupt
PrefetchAddr        DCD     PrefetchAbort
DataAbortAddr       DCD     DataAbort
Nouse               DCD     0
IRQ_Addr            DCD     0
FIQ_Addr            DCD     FIQ_Handler

;δ����ָ��
Undefined
        B       Undefined

;���ж�
SoftwareInterrupt
        B       SoftwareInterrupt

;ȡָ����ֹ
PrefetchAbort
        B       PrefetchAbort

;ȡ������ֹ
DataAbort
        B       DataAbort

;�����ж�
FIQ_Handler
        B       FIQ_Handler


;/*********************************************************************************************************
;** unction name 	��������: 	ResetInit
;** Descriptions 	��������: 	RESET  ��λ���
;** input parameters 	�䡡��:   	None ��
;** Returned value   	�䡡�� :  	None ��
;** Used global variables ȫ�ֱ���: 	None ��
;** Calling modules 	����ģ��: 	None ��
;** 
;** Created by 		������: 	Chenmingji ������
;** Created Date 	�ա���: 	2004/02/02 2004��2��2��
;**-------------------------------------------------------------------------------------------------------
;** Modified by 	�ޡ���: Chenmingji ������
;** Modified date 	�ա���: 2004/02/02 2004��3��3��
;**-------------------------------------------------------------------------------------------------------
;********************************************************************************************************/
ResetInit
;Initial the extenal controller
;��ʼ���ⲿ���߿�����������Ŀ����������

        LDR     R0, =PINSEL2
    IF :DEF: EN_CRP
        LDR     R1, =0x0f814910
    ELSE
        LDR     R1, =0x0f814914
    ENDIF
        STR     R1, [R0]

        LDR     R0, =BCFG0
        LDR     R1, =BCFG_FLASH
        STR     R1, [R0]

        LDR     R0, =BCFG1
        LDR     R1, =BCFG_PSRAM
        STR     R1, [R0]

        LDR     R0, =BCFG2
        LDR     R1, =BCFG_CS2
        STR     R1, [R0]

        LDR     R0, =BCFG3
        LDR     R1, =BCFG_CS3
        STR     R1, [R0]
        
        B       MAIN

;/*********************************************************************************************************
;** unction name 	��������: 	CrpData
;** Descriptions 	��������: 	encrypt the chip
;** input parameters 	�䡡��:   	None ��
;** Returned value   	�䡡�� :  	None ��
;** Used global variables ȫ�ֱ���: 	None ��
;** Calling modules 	����ģ��: 	None ��
;** 
;** Created by 		������: 	Chenmingji ������
;** Created Date 	�ա���: 	2004/03/27 2004��3��27��
;**-------------------------------------------------------------------------------------------------------
;** Modified by 	�ޡ���: 
;** Modified date 	�ա���: 
;**-------------------------------------------------------------------------------------------------------
;********************************************************************************************************/
    IF :DEF: EN_CRP
        IF  . >= 0x1fc
        INFO    1,"\nThe data at 0x000001fc must be 0x87654321.\nPlease delete some source before this line."
        ENDIF
CrpData
    WHILE . < 0x1fc
    NOP
    WEND
CrpData1
    DCD     0x87654321          ;/*When the Data is Ϊ0x87654321,user code be protected. ������Ϊ0x87654321ʱ���û����򱻱��� */
    ENDIF
    END
;/*********************************************************************************************************
;**                            End Of File
;********************************************************************************************************/
