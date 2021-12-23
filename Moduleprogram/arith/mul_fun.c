#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HANUMANTHU");
MODULE_DESCRIPTION("Exporting add function");

//function that is going to export as sumbol
int my_mul(int a, int b)
{
        return (a*b);
}

//command to export symbol into kernel symbol table
EXPORT_SYMBOL(my_mul);

//function initialization
static int __init mulfun_init(void)
{
        printk(KERN_ALERT "\naddfun initialisation\n");
        return 0;
}
//remove the module
static void __exit mulfun_exit(void)
{
        printk(KERN_ALERT "\naddfun clean\n");
}

module_init(mulfun_init);
module_exit(mulfun_exit);

