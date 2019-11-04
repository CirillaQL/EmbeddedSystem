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

#define DEV_MAJOR 70

int ex1_open(struct inode * inode, struct file *file)
{
	printk("ex1 open! \n");
	return 0;
}
int ex1_read(struct file *file, char * buf,int count,int * ppos)
{
	printk("ex1 READ! \n");
	return 0;
}
int ex1_write(struct file *file, char * buf,int count,int * ppos)
{
	printk("ex1 write! \n");
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
	result = register_chrdev(DEV_MAJOR, "ex1", &ex1_fops);//注册设备

	
	if (result<0)

	{

		printk(KERN_INFO"[FALLED: Cannot register ex1]\n");

		return result;

	}

	else

		printk("[OK]\n");
	printk("ex1 driver init \n");
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



