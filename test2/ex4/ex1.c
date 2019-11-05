#include <linux/config.h>
#include <linux/utsname.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/string.h>
#include <linux/fcntl.h>
#include <linux/slab.h>
#include <linux/timer.h>
#include <linux/fs.h>

#include <linux/sched.h>
#include <linux/tty.h>
#include <linux/module.h>
#include <linux/init.h>


#include <asm-arm/arch-s3c2410/irqs.h>
#include <asm-arm/uaccess.h>
#include <asm-arm/mach/irq.h>
#include <asm/hardware.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/system.h>

#define DEV_MAJOR 70
#define rLed  *(unsigned char*)(0xd3000000)
static int delayLoopCount=600;

void mdelay(int time)
{
	int i;
	for(;time>0;time--)
		for(i=0;i<delayLoopCount;i++);
}

void Led_Display(int LedStatus)
{
	switch (LedStatus)
	{
		case 1:
			rLed = 0xfe;
			break;
		case 2:
			rLed = 0xfd;
			break;
		case 3:
			rLed = 0xfb;
			break;
		case 4:
			rLed = 0xf7;
			break;
		case 5:
			rLed = 0xef;
			break;
		case 6:
			rLed = 0xdf;
			break;
		case 7:
			rLed = 0xbf;
			break;
		case 8:
			rLed = 0x7f;
			break;
		case 9:
		    rLed = 0xff;
			break;
		case 10:
		    rLed = 0;
			break;
		default:
		    break;
	}
}
int led01_read(void){
	int i;
        i=rLed & (0x01);
        return i;
}
int led02_read(void){
	    int i;
        i=rLed & (0x02);
        return i;
}
int led03_read(void){
	    int i;
        i=rLed & (0x04);
        return i;
}

int ex1_open(struct inode * inode, struct file *file)
{
	printk("ex1 open! \n");
	return 0;
}
int ex1_read(struct file *file, char * buf,int count,int * ppos)
{
	unsigned char temp='0';
        printk("ex1 READ! \n");
        if(led01_read()==0) temp='1';
        if(led02_read()==0) temp='2';
        if(led03_read()==0) temp='3';
        put_user(temp,buf);
        printk("led %c is on \n",temp);
	return 0;
}
int ex1_write(struct file *file, char * buf,int count,int * ppos)
{
	char c;
        printk("ex1 write! \n");
	get_user(c,buf);
        Led_Display(9);
         mdelay(4000);  
        if(c=='a') {
		int k = 1;
		for(k; k < 9; k = k+1){
			Led_Display(k);
			mdelay(20000);
		}
	}
	if(c=='b'){
		int m = 8;
		for(m;m > 0; m = m -1){
			Led_Display(m);
			mdelay(20000);
		}
	}
         mdelay(4000); 
	return count;
}
static struct file_operations ex1_fops={
    .owner=THIS_MODULE,
    .open=ex1_open,
    .read=ex1_read,
    .write=ex1_write,
};
static int __init ex1_init(void){
	int     result;
	printk("\n Registering  Device\t--- >\t");
	result = register_chrdev(DEV_MAJOR, "ex1", &ex1_fops);//×¢²áÉè±¸

	
	if (result<0)

	{

		printk(KERN_INFO"[FALLED: Cannot register ex1]\n");

		return result;

	}

	else

	printk("[OK]\n");
	printk("QCA:ex1 driver init \n");
     Led_Display(10);
     mdelay(4000); 
    Led_Display(9);
     mdelay(4000);
    Led_Display(2);


	return 0;
}
static void __exit ex1_exit(void)
{
	unregister_chrdev(DEV_MAJOR, "ex1");
    printk("You have uninstall The ex1 Driver succesfully,\n if you want to install again,please use the insmod command \n");

}
module_init(ex1_init);
module_exit(ex1_exit);
MODULE_AUTHOR("qcd");
MODULE_LICENSE("GPL");

