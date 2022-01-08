#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hanumanthu");
MODULE_DESCRIPTION("Module Program to Pass Command Line Arguments");

//This is the Addition function that we are going to export into Kernel Symbol Table
static int my_add(int a, int b)
{
    return (a+b);
}

EXPORT_SYMBOL_GPL(my_add);  //Command to export function into Kernel Symbol Table

static int module_param_init(void)
{
    printk(KERN_ALERT "We are inside init Section\n");

    return 0;
}
static void module_param_exit(void)
{
    printk(KERN_ALERT "Exiting from Kernel\n");
}

module_init(module_param_init);
module_exit(module_param_exit);

