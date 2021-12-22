#include<linux/init.h>
#include<linux/module.h>
#include<linux/moduleparam.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HANUMANTHU");
MODULE_DESCRIPTION("Module prog to pass command line args");

static char *charvar="hello";
static int intvar=10;

module_param(charvar,charp,S_IRUGO);
module_param(intvar,int,S_IRUGO);

static int __init module_param_init(void)
{
	printk(KERN_ALERT "in the module\n");
	printk("charvar=%s\n",charvar);
	printk("intvar=%d\n",intvar);
	return 0;
}

static void __exit module_param_exit(void)
{
	printk(KERN_ALERT "exit module\n");
}

module_init(module_param_init);
module_exit(module_param_exit);
