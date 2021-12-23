#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HANUMANTHU");
MODULE_DESCRIPTION("Exporting add function");

//function that is going to export as sumbol
int my_sub(int a, int b)
{
        return (a-b);
}

//command to export symbol into kernel symbol table
EXPORT_SYMBOL(my_sub);

//function initialization
static int __init subfun_init(void)
{
        printk(KERN_ALERT "\naddfun initialisation\n");
        return 0;
}
//remove the module
static void __exit subfun_exit(void)
{
        printk(KERN_ALERT "\naddfun clean\n");
}

module_init(subfun_init);
module_exit(subfun_exit);

