;/****************************************Copyright (c)**************************************************
;**                               Guangzou ZLG-MCU Development Co.,LTD.
;**                                      graduate school
;**                                 http://www.zlgmcu.com
;**
;**--------------File Info-------------------------------------------------------------------------------
;** File name:			main.s
;** Last modified Date:  2004-09-16
;** Last Version:		1.0
;** Descriptions:		The main() function example template
;**
;**------------------------------------------------------------------------------------------------------
;** Created by:			Chenmingji
;** Created date:		2004-09-16
;** Version:				1.0
;** Descriptions:		The original version
;**
;**------------------------------------------------------------------------------------------------------
;** Modified by:
;** Modified date:
;** Version:
;** Descriptions:
;**
;********************************************************************************************************/
		    INCLUDE		LPC2294.INC			; Include the head file 引入头文件

;声明MAIN函数
;Export MAIN functon
    EXPORT  MAIN                            ;MAIN Function 主程序
    
    CODE32

    AREA    main,CODE,READONLY
        ENTRY
        
MAIN
	
	;add the user code here. 添加用户代码
	
	B	MAIN
	
	
	END