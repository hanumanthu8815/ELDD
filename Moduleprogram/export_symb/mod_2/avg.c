
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include "arith.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hanumanthu");
MODULE_DESCRIPTION("Module Program to Pass Command Line Arguments");

static int num1 = 10;
static int num2 = 12;

//Init Function 
static int module_param_init(void)
{
    int Sum, Average;

    Sum = my_add(num1,num2);
    Average = Sum/2;

    printk(KERN_ALERT "We are inside init Section\n");

    printk("The Sum of %d and %d is = %d\n",num1,num2,Sum);
    printk("Average of %d and %d  = %d\n",num1,num2,Average);
    
    return 0;
}
//Cleanup Function
static void module_param_exit(void)
{
    printk(KERN_ALERT "Exiting from Kernel\n");
}

module_init(module_param_init);
module_exit(module_param_exit);

