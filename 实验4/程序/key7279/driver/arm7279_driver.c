
/*
;************************************************************************************************************
;*				          北京精仪达盛科技有限公司
;*                                     	     砄1�7   叄1�7   郄1�7
;*
;*                                 	     http://www.techshine.com
;*
;*--------------------------------------------- 文件信息 ---------------------------------------------------    
;*
;* 文件名称 : ARM7279.c	
;* 文件功能 : 该文件为S3C2410硬件平台的键盘程序�1�7�1�7
;* 补充说明 : 
;*-------------------------------------------- 朄1�7新版本信恄1�7-------------------------------------------------
;* 修改作�1�7�1�7: ARM弄1�7发小组1�7
;* 修改日期 : 2005/09/09
;* 版本声明 : V1.0.0
;*-------------------------------------------- 历史版本信息 -------------------------------------------------
;* 文件作�1�7�1�7: ARM弄1�7发小组1�7
;* 创建日期 : 2005/09/09
;* 版本声明 : v1.0.0
;*-----------------------------------------------------------------------------------------------------------
;*-----------------------------------------------------------------------------------------------------------
;************************************************************************************************************
;*/
#include <linux/config.h>
#include <linux/utsname.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/string.h>
#include <linux/fcntl.h>
#include <linux/slab.h>
#include <linux/timer.h>

#include <linux/sched.h>
#include <linux/tty.h>
#include <linux/module.h>
#include <linux/init.h>


#include <asm-arm/arch-s3c2410/irqs.h>
#include <asm-arm/mach/irq.h>
#include <asm/hardware.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/system.h>

#include <linux/poll.h>
#include <asm/arch/io.h>

/********************************** 送给HD7279指令 *************************************/

#define  cmd_reset 0xa4
#define  cmd_test  0xbf
#define  cmd_read  0x15
#define  decode1   0xc8



/********************************** 函数定义 ********************************************/

void          long_delay   (void);
void          short_delay  (void);
void          write7279    (unsigned char,unsigned char);
unsigned char read7279     (unsigned char);
void          send_byte    (unsigned char);
unsigned char receive_byte (void);

int kbd_isopen;
unsigned char kbd_buf = 0xFF;

/******************************** HD7279A的使用原理说昄1�7***************************************/
/*
--HD7279A的数据是通过CPLD作为与S3C241O通讯的接口，CPU通过在相应地坄1�7上的读写CPLD，即可与HD7279
  进行有效的�1�7�讯。由于CPLD是�1�7�过CPU的NGCS4选择的，扄1�7以向CPLD读写均需要使用NGCS4。下面为具体的1�7
  使用方法〄1�7
  
--向地坄1�7 0x20000004 里写0X01，表示�1�7�中HD7279，即CS7279='0
--从地坄1�7 0x20000004 中写0x02，表示未选中HD7279，即CS7279='1

--向地坄1�7 0x20000004 里写0x05，表示数据流从CPLD到HD7279
--从地坄1�7 0x20000004 中写0x06，表示数据流从HD7279到CPLD

*/


/******************************** 7279时钟模拟信号 ***************************************/

        #define clrcs1  	        *((volatile unsigned char *)(CPLD_BASE + 0x4)) = 0x01
	#define setcs1      		*((volatile unsigned char *)(CPLD_BASE + 0x4)) = 0x02

     
        #define s_clr  	                *((volatile unsigned char *)(CPLD_BASE + 0x4)) = 0x05
	#define s_set      		*((volatile unsigned char *)(CPLD_BASE + 0x4)) = 0x06

	#define clk     		(GPIO_MODE_OUT | GPIO_PULLUP_EN | GPIO_E13)
	#define dat             	(GPIO_MODE_OUT | GPIO_PULLUP_EN | GPIO_E12)
        
        #define setclk                  write_gpio_bit(clk, 1)
        #define clrclk                  write_gpio_bit(clk, 0)
 
        #define setdat                  write_gpio_bit(dat, 1)
        #define clrdat                  write_gpio_bit(dat, 0)


//CPLD_BASE的定义在/includ/asm-arm/arch-s3c2410/smdk.h冄1�7
#define KEYBOARD_MAJOR 50
#define Kbd7279_GETKEY 0
int KeyValue;
/*
*************************************************************************************************************
- 函数名称 : Kbd7279_Close
- 函数说明 : 关闭键盘设备
- 输入参数 : 旄1�7
- 输出参数 : 0
*************************************************************************************************************
*/
static int Kbd7279_Close(struct inode * inode, struct file * file)

{
         printk("Close successful\n");
	kbd_isopen = 0;
	return 0;

}
/*
*************************************************************************************************************
- 函数名称 : Kbd7279_Open
- 函数说明 : 打开键盘设备
- 输入参数 : 旄1�7
- 输出参数 : 0
*************************************************************************************************************
*/

static int Kbd7279_Open(struct inode * inode, struct file * file)

{

 printk("Open successful\n");	
      kbd_isopen++;
      return 0;

}

/*
*************************************************************************************************************
- 函数名称 : Kbd7279_Read
- 函数说明 : 打开键盘设备
- 输入参数 : 旄1�7- 输出参数 : 0
*************************************************************************************************************
*/

static int Kbd7279_Read(struct file *fp, char *buf, size_t count)
{
    put_user(kbd_buf, buf);
    //write7279(decode1+5,KeyValue/16*8);
    //write7279(decode1+4,KeyValue & 0x0f);
   // write7279(decode1+5,0x09);
    //write7279(decode1+4,0x07);
    kbd_buf = 0xFF;
    return 1;
}
/*
*************************************************************************************************************
- 函数名称 : kbd7279_getkey
- 函数说明 : 获取丄1�7个键倄1�7
- 输入参数 : 旄1�7
- 输出参数 : -1
*************************************************************************************************************
*/

static int kbd7279_getkey(void)

{

	int  i,j;


	enable_irq(33);

         
	KeyValue = 0xff;

	for (i=0;i<3000;i++)

	     for (j=0;j<900;j++);

        return KeyValue; //如果有按键按下，返回键�1�7�1�7
}

/*
*************************************************************************************************************
- 函数名称 : Kbd7279_ISR
- 函数说明 : 键盘服务子程庄1�7
- 输入参数 : irq,dev_id,regs
- 输出参数 : -1
*************************************************************************************************************
*/

static void Kbd7279_ISR(int irq,void* dev_id,struct pt_regs * regs){

	int i;


	disable_irq(33);

        for(i=0;i<100;i++);
  	KeyValue = read7279(cmd_read);
	
	  switch (KeyValue)
	    {
	    case 8:
	      KeyValue = 0x4;
	      break;
            case 9:
              KeyValue = 0x5;
	      break;
            case 10:
              KeyValue = 0x6;
              break;
            case 11:
              KeyValue = 0x7;
              break;
            case 4:
	      KeyValue = 0x8;
              break;
            case 5:
              KeyValue = 0x9;
              break;
            case 6:
              KeyValue = 0xa;
              break;
            case 7:
              KeyValue = 0xb;
              break;
            default:
              break;
	    }
        //write7279(decode1+5,KeyValue/16*8);
        //write7279(decode1+4,KeyValue & 0x0f);
	kbd_buf = (unsigned char)KeyValue;
        //printk("KeyValue = %d\n",KeyValue);

 }
/*
*************************************************************************************************************
- 函数名称 : Setup_kbd7279
- 函数说明 : 键盘设备的硬件初始化函数
- 输入参数 : 旄1�7
- 输出参数 : 旄1�7
*************************************************************************************************************
*/

void Setup_Kbd7279(void)

{

	int i;

   
        BWSCON &=(~(3<<16));  // set the bank4 databus is 8 bitwidth

	/* GPE13: CLK, OUTPUT */
	set_gpio_ctrl(clk);

	/* GPE12: DATA, OUTPUT */
	set_gpio_ctrl(dat);
                             
	set_gpio_ctrl(GPIO_F5|GPIO_MODE_EINT);
           //set EINT MODE
        set_external_irq(33,2,0);                           //set falling edge triger

        for(i=0;i<100;i++);

}
							//其他选项省略


/*
*************************************************************************************************************
- 函数名称 : Kbd7279_Ioctl
- 函数说明 : 键盘控制
- 输入参数 : 旄1�7
- 输出参数 : 0
*************************************************************************************************************
*/

static int Kbd7279_Ioctl(struct inode *inode,struct file *file,

                         unsigned int cmd, unsigned long arg)

{

	int i;

	switch(cmd) 

	{

		case Kbd7279_GETKEY:


			 return kbd7279_getkey();
	        case 1:
		         
			 
			 get_user(i, (int *) arg);
			// printk("the value is %d.\n", i);
			 send_byte(cmd_reset);
			 long_delay();
			 long_delay();
			 write7279(decode1+5,i/16*8);
                         write7279(decode1+4,i&0x0f);
			 send_byte(cmd_reset);
			 long_delay();
			 long_delay();
			 write7279(decode1+5,i/16*8);
                         write7279(decode1+4,i&0x0f);
			 break;
		 
 	
        	default:

                         printk("Unkown Keyboard Command ID.\n");

    }

    return 0;

}

/*
*************************************************************************************************************
- 函数名称 : struct file_operations Uart2_fops
- 函数说明 : 文件结构
- 输入参数 : 旄1�7
- 输出参数 : 旄1�7
*************************************************************************************************************
*/
struct file_operations Kbd7279_fops = 
{

	open:	  Kbd7279_Open,  //打开设备文件

	ioctl:	  Kbd7279_Ioctl, //设备文件其他操作

	release:    Kbd7279_Close, //关闭设备文件
	read:	  Kbd7279_Read,  //读取设备文件

};

/*
*************************************************************************************************************
- 函数名称 : int Kbd7279Init(void)
- 函数说明 : 注册键盘设备，调用初始化函数
- 输入参数 : 旄1�7
- 输出参数 : -1
*************************************************************************************************************
*/

int __init Kbd7279_Init(void)

{

   	int     result;

	printk("\n Registering Kbdboard Device\t--- >\t");

	result = register_chrdev(KEYBOARD_MAJOR, "Kbd7279", &Kbd7279_fops);//注册设备

	
	if (result<0)

	{

		printk(KERN_INFO"[FALLED: Cannot register Kbd7279_driver!]\n");

		return result;

	}

	else

		printk("[OK]\n");

	
	printk("Initializing HD7279 Device\t--- >\t");

	Setup_Kbd7279();

	
	if (request_irq(33,Kbd7279_ISR,0,"Kbd7279","88"))

	{

   		printk(KERN_INFO"[FALLED: Cannot register Kbd7279_Interrupt!]\n");

		return -EBUSY;

	}

	else

		printk("[OK]\n");

	printk("Kbd7279 Driver Installed.\n");

	return 0;

}

								//其他选项省略


/*
*************************************************************************************************************
- 函数名称 : void long_delay(void)
- 函数说明 : 长延时程庄1�7
- 输入参数 : 旄1�7
- 输出参数 : 旄1�7
*************************************************************************************************************
*/
void long_delay(void)
{ 
	unsigned char i;
   	for (i=0;i<250;i++)
   	{
	}
	
}

/*
*************************************************************************************************************
- 函数名称 : void short_delay(void)
- 函数说明 : 短延时程庄1�7
- 输入参数 : 旄1�7
- 输出参数 : 旄1�7
*************************************************************************************************************
*/
void short_delay(void)
{ 
	int i;
  	for(i=0;i<150;i++)
  	{
  	}

}

/*
*************************************************************************************************************
- 函数名称 : void send_byte(unsigned char out_byte )
- 函数说明 : 各1�779发�1�7�一个字节的程序
- 输入参数 : out_byte
- 输出参数 : 旄1�7
*************************************************************************************************************
*/
void send_byte(unsigned char out_byte )
{
	unsigned short  i;
	
	clrcs1;
	  s_clr;
  	long_delay();
  	
  	for (i=0;i<8;i++)
  	{   
  		if (0x80 == (out_byte & 0x80))  
      	{ 
      		setdat;
      	}
      	else
      	{
      		clrdat;
      	}
      	
      	setclk;
      	short_delay();
      	clrclk;
      	short_delay();
      	
      	out_byte <<= 1;
  	}
  	short_delay();
  	clrdat;
  
 }
 
/*
*************************************************************************************************************
- 函数名称 : unsigned char receive_byte (void)
- 函数说明 : 各1�779接收丄1�7个字节的程序
- 输入参数 : 旄1�7
- 输出参数 : in_byte
*************************************************************************************************************
*/
unsigned char receive_byte (void)    
{
   	unsigned char i,in_byte=0;
   	
   	s_set;
	set_gpio_ctrl(GPIO_MODE_IN | GPIO_PULLUP_EN | GPIO_E12);
   	long_delay();

    for(i=0;i<8;i++)
    {
    	setclk;
     	short_delay();
    	in_byte <<= 1;

    	if (read_gpio_bit(GPIO_E12)&0x01)
    	{ 
    		in_byte |= 1;
    	}
 	clrclk;
    	short_delay();
	
    }

    s_clr;
    set_gpio_ctrl(GPIO_MODE_OUT | GPIO_PULLUP_EN | GPIO_E12);
    clrdat;
    short_delay();
   
    return(in_byte);
}
 
/*
*************************************************************************************************************
- 函数名称 : unsigned char read7279(unsigned char comand)
- 函数说明 : 读键盘指令程庄1�7
- 输入参数 : 旄1�7
- 输出参数 : in_byte
*************************************************************************************************************
*/    
unsigned char read7279(unsigned char comand)
{
  	send_byte(comand);
   	return (receive_byte());
}

/*
*************************************************************************************************************
- 函数名称 : void write7279 (unsigned char cmd, unsigned char dat)
- 函数说明 : 写键盘指令程庄1�7
- 输入参数 : 旄1�7
- 输出参数 : in_byte
*************************************************************************************************************
*/   
void write7279 (unsigned char cmd, unsigned char date)
{
	send_byte(cmd);
 	send_byte(date);
}



/*
*************************************************************************************************************
- 函数名称 : Kbd7279_Close
- 函数说明 : 关闭键盘设备
- 输入参数 : 旄1�7
- 输出参数 : 0
*************************************************************************************************************
*/
void __exit Kbd7279_Exit(void)

{

        unregister_chrdev(KEYBOARD_MAJOR, "Kbd7279");
        free_irq(33,"88");
        send_byte(cmd_reset);
        printk("You have uninstall The Kbd7279 Driver succesfully,\n if you want to install again,please use the insmod command \n");
}

module_init(Kbd7279_Init);
module_exit(Kbd7279_Exit);
/*
*************************************************************************************************************
*-						结束文件                                                   -*
*************************************************************************************************************
*/
    



