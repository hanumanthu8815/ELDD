#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include "my_add.h"     //Header containing the declaration of Symbol (my_add)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hanumanthu");
MODULE_DESCRIPTION("Calling the AddFunction module");

/*Variables are declared as static to keep their scope local to this module*/
static int num1 = 10;
static int num2 = 20;

static int __init data_init(void)
{
	
	printk(KERN_ALERT "Add of %d and %d =  %d\n", num1, num2, my_add(num1,num2));
	return 0;
}

static void __exit data_exit(void)
{
	printk(KERN_ALERT "clean\n");
}

module_init(data_init);
module_exit(data_exit);
