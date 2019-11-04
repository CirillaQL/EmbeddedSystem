
/*
;************************************************************************************************************
;*				          鍖椾含绮句华杈剧洓绉戞妧鏈夐檺鍏徃
;*                                     	     鐮�   鍙�   閮�
;*
;*                                 	     http://www.techshine.com
;*
;*--------------------------------------------- 鏂囦欢淇℃伅 ---------------------------------------------------    
;*
;* 鏂囦欢鍚嶇О : ARM7279.c	
;* 鏂囦欢鍔熻兘 : 璇ユ枃浠朵负S3C2410纭欢骞冲彴鐨勯敭鐩樼▼搴忋��
;* 琛ュ厖璇存槑 : 
;*-------------------------------------------- 鏈�鏂扮増鏈俊鎭�-------------------------------------------------
;* 淇敼浣滆��: ARM寮�鍙戝皬缁�
;* 淇敼鏃ユ湡 : 2005/09/09
;* 鐗堟湰澹版槑 : V1.0.0
;*-------------------------------------------- 鍘嗗彶鐗堟湰淇℃伅 -------------------------------------------------
;* 鏂囦欢浣滆��: ARM寮�鍙戝皬缁�
;* 鍒涘缓鏃ユ湡 : 2005/09/09
;* 鐗堟湰澹版槑 : v1.0.0
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

/********************************** 閫佺粰HD7279鎸囦护 *************************************/

#define  cmd_reset 0xa4
#define  cmd_test  0xbf
#define  cmd_read  0x15
#define  decode1   0xc8



/********************************** 鍑芥暟瀹氫箟 ********************************************/

void          long_delay   (void);
void          short_delay  (void);
void          write7279    (unsigned char,unsigned char);
unsigned char read7279     (unsigned char);
void          send_byte    (unsigned char);
unsigned char receive_byte (void);

int kbd_isopen;
unsigned char kbd_buf = 0xFF;

/******************************** HD7279A鐨勪娇鐢ㄥ師鐞嗚鏄�***************************************/
/*
--HD7279A鐨勬暟鎹槸閫氳繃CPLD浣滀负涓嶴3C241O閫氳鐨勬帴鍙ｏ紝CPU閫氳繃鍦ㄧ浉搴斿湴鍧�涓婄殑璇诲啓CPLD锛屽嵆鍙笌HD7279
  杩涜鏈夋晥鐨勯�氳銆傜敱浜嶤PLD鏄�氳繃CPU鐨凬GCS4閫夋嫨鐨勶紝鎵�浠ュ悜CPLD璇诲啓鍧囬渶瑕佷娇鐢∟GCS4銆備笅闈负鍏蜂綋鐨�
  浣跨敤鏂规硶銆�
  
--鍚戝湴鍧� 0x20000004 閲屽啓0X01锛岃〃绀洪�変腑HD7279锛屽嵆CS7279='0
--浠庡湴鍧� 0x20000004 涓啓0x02锛岃〃绀烘湭閫変腑HD7279锛屽嵆CS7279='1

--鍚戝湴鍧� 0x20000004 閲屽啓0x05锛岃〃绀烘暟鎹祦浠嶤PLD鍒癏D7279
--浠庡湴鍧� 0x20000004 涓啓0x06锛岃〃绀烘暟鎹祦浠嶩D7279鍒癈PLD

*/


/******************************** 7279鏃堕挓妯℃嫙淇″彿 ***************************************/

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


//CPLD_BASE鐨勫畾涔夊湪/includ/asm-arm/arch-s3c2410/smdk.h鍐�
#define KEYBOARD_MAJOR 50
#define Kbd7279_GETKEY 0
int KeyValue;
/*
*************************************************************************************************************
- 鍑芥暟鍚嶇О : Kbd7279_Close
- 鍑芥暟璇存槑 : 鍏抽棴閿洏璁惧
- 杈撳叆鍙傛暟 : 鏃�
- 杈撳嚭鍙傛暟 : 0
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
- 鍑芥暟鍚嶇О : Kbd7279_Open
- 鍑芥暟璇存槑 : 鎵撳紑閿洏璁惧
- 杈撳叆鍙傛暟 : 鏃�
- 杈撳嚭鍙傛暟 : 0
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
- 鍑芥暟鍚嶇О : Kbd7279_Read
- 鍑芥暟璇存槑 : 鎵撳紑閿洏璁惧
- 杈撳叆鍙傛暟 : 鏃�- 杈撳嚭鍙傛暟 : 0
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
- 鍑芥暟鍚嶇О : kbd7279_getkey
- 鍑芥暟璇存槑 : 鑾峰彇涓�涓敭鍊�
- 杈撳叆鍙傛暟 : 鏃�
- 杈撳嚭鍙傛暟 : -1
*************************************************************************************************************
*/

static int kbd7279_getkey(void)

{

	int  i,j;


	enable_irq(33);

         
	KeyValue = 0xff;

	for (i=0;i<3000;i++)

	     for (j=0;j<900;j++);

        return KeyValue; //濡傛灉鏈夋寜閿寜涓嬶紝杩斿洖閿��
}

/*
*************************************************************************************************************
- 鍑芥暟鍚嶇О : Kbd7279_ISR
- 鍑芥暟璇存槑 : 閿洏鏈嶅姟瀛愮▼搴�
- 杈撳叆鍙傛暟 : irq,dev_id,regs
- 杈撳嚭鍙傛暟 : -1
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
- 鍑芥暟鍚嶇О : Setup_kbd7279
- 鍑芥暟璇存槑 : 閿洏璁惧鐨勭‖浠跺垵濮嬪寲鍑芥暟
- 杈撳叆鍙傛暟 : 鏃�
- 杈撳嚭鍙傛暟 : 鏃�
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
							//鍏朵粬閫夐」鐪佺暐


/*
*************************************************************************************************************
- 鍑芥暟鍚嶇О : Kbd7279_Ioctl
- 鍑芥暟璇存槑 : 閿洏鎺у埗
- 杈撳叆鍙傛暟 : 鏃�
- 杈撳嚭鍙傛暟 : 0
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
- 鍑芥暟鍚嶇О : struct file_operations Uart2_fops
- 鍑芥暟璇存槑 : 鏂囦欢缁撴瀯
- 杈撳叆鍙傛暟 : 鏃�
- 杈撳嚭鍙傛暟 : 鏃�
*************************************************************************************************************
*/
struct file_operations Kbd7279_fops = 
{

	open:	  Kbd7279_Open,  //鎵撳紑璁惧鏂囦欢

	ioctl:	  Kbd7279_Ioctl, //璁惧鏂囦欢鍏朵粬鎿嶄綔

	release:    Kbd7279_Close, //鍏抽棴璁惧鏂囦欢
	read:	  Kbd7279_Read,  //璇诲彇璁惧鏂囦欢

};

/*
*************************************************************************************************************
- 鍑芥暟鍚嶇О : int Kbd7279Init(void)
- 鍑芥暟璇存槑 : 娉ㄥ唽閿洏璁惧锛岃皟鐢ㄥ垵濮嬪寲鍑芥暟
- 杈撳叆鍙傛暟 : 鏃�
- 杈撳嚭鍙傛暟 : -1
*************************************************************************************************************
*/

int __init Kbd7279_Init(void)

{

   	int     result;

	printk("\n Registering Kbdboard Device\t--- >\t");

	result = register_chrdev(KEYBOARD_MAJOR, "Kbd7279", &Kbd7279_fops);//娉ㄥ唽璁惧

	
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

								//鍏朵粬閫夐」鐪佺暐


/*
*************************************************************************************************************
- 鍑芥暟鍚嶇О : void long_delay(void)
- 鍑芥暟璇存槑 : 闀垮欢鏃剁▼搴�
- 杈撳叆鍙傛暟 : 鏃�
- 杈撳嚭鍙傛暟 : 鏃�
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
- 鍑芥暟鍚嶇О : void short_delay(void)
- 鍑芥暟璇存槑 : 鐭欢鏃剁▼搴�
- 杈撳叆鍙傛暟 : 鏃�
- 杈撳嚭鍙傛暟 : 鏃�
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
- 鍑芥暟鍚嶇О : void send_byte(unsigned char out_byte )
- 鍑芥暟璇存槑 : 鍚�79鍙戦�佷竴涓瓧鑺傜殑绋嬪簭
- 杈撳叆鍙傛暟 : out_byte
- 杈撳嚭鍙傛暟 : 鏃�
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
- 鍑芥暟鍚嶇О : unsigned char receive_byte (void)
- 鍑芥暟璇存槑 : 鍚�79鎺ユ敹涓�涓瓧鑺傜殑绋嬪簭
- 杈撳叆鍙傛暟 : 鏃�
- 杈撳嚭鍙傛暟 : in_byte
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
- 鍑芥暟鍚嶇О : unsigned char read7279(unsigned char comand)
- 鍑芥暟璇存槑 : 璇婚敭鐩樻寚浠ょ▼搴�
- 杈撳叆鍙傛暟 : 鏃�
- 杈撳嚭鍙傛暟 : in_byte
*************************************************************************************************************
*/    
unsigned char read7279(unsigned char comand)
{
  	send_byte(comand);
   	return (receive_byte());
}

/*
*************************************************************************************************************
- 鍑芥暟鍚嶇О : void write7279 (unsigned char cmd, unsigned char dat)
- 鍑芥暟璇存槑 : 鍐欓敭鐩樻寚浠ょ▼搴�
- 杈撳叆鍙傛暟 : 鏃�
- 杈撳嚭鍙傛暟 : in_byte
*************************************************************************************************************
*/   
void write7279 (unsigned char cmd, unsigned char date)
{
	send_byte(cmd);
 	send_byte(date);
}



/*
*************************************************************************************************************
- 鍑芥暟鍚嶇О : Kbd7279_Close
- 鍑芥暟璇存槑 : 鍏抽棴閿洏璁惧
- 杈撳叆鍙傛暟 : 鏃�
- 杈撳嚭鍙傛暟 : 0
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
*-						缁撴潫鏂囦欢                                                   -*
*************************************************************************************************************
*/
    



