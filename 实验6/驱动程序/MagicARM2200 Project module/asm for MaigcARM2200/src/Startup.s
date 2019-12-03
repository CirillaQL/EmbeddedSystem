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
		    INCLUDE		LPC2294.INC			; IMPORT The Head File 引入头文件

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
;引入的外部标号在这声明
    IMPORT  MAIN                            ;主程序

;The emported labels    
;给外部使用的标号在这声明
    EXPORT  Reset

    CODE32

    AREA    vectors,CODE,READONLY
        ENTRY
;interrupt vectors
;中断向量表
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

;未定义指令
Undefined
        B       Undefined

;软中断
SoftwareInterrupt
        B       SoftwareInterrupt

;取指令中止
PrefetchAbort
        B       PrefetchAbort

;取数据中止
DataAbort
        B       DataAbort

;快速中断
FIQ_Handler
        B       FIQ_Handler


;/*********************************************************************************************************
;** unction name 	函数名称: 	ResetInit
;** Descriptions 	功能描述: 	RESET  复位入口
;** input parameters 	输　入:   	None 无
;** Returned value   	输　出 :  	None 无
;** Used global variables 全局变量: 	None 无
;** Calling modules 	调用模块: 	None 无
;** 
;** Created by 		作　者: 	Chenmingji 陈明计
;** Created Date 	日　期: 	2004/02/02 2004年2月2日
;**-------------------------------------------------------------------------------------------------------
;** Modified by 	修　改: Chenmingji 陈明计
;** Modified date 	日　期: 2004/02/02 2004年3月3日
;**-------------------------------------------------------------------------------------------------------
;********************************************************************************************************/
ResetInit
;Initial the extenal controller
;初始化外部总线控制器，根据目标板决定配置

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
;** unction name 	函数名称: 	CrpData
;** Descriptions 	功能描述: 	encrypt the chip
;** input parameters 	输　入:   	None 无
;** Returned value   	输　出 :  	None 无
;** Used global variables 全局变量: 	None 无
;** Calling modules 	调用模块: 	None 无
;** 
;** Created by 		作　者: 	Chenmingji 陈明计
;** Created Date 	日　期: 	2004/03/27 2004年3月27日
;**-------------------------------------------------------------------------------------------------------
;** Modified by 	修　改: 
;** Modified date 	日　期: 
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
    DCD     0x87654321          ;/*When the Data is 为0x87654321,user code be protected. 当此数为0x87654321时，用户程序被保护 */
    ENDIF
    END
;/*********************************************************************************************************
;**                            End Of File
;********************************************************************************************************/
