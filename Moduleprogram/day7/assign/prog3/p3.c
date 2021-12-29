/* 3. Write a charcter driver to dynamically allocate a maj,min number
pair from the kernel.
Test the same and conclude   */

#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>

#define NAME mydevice3

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hanumanthu");

dev_t Mydev=0; //global declaration

//fun prototypes
int NAME_open(struct inode *inode, struct file *filp);
int NAME_release(struct inode *inode, struct file *filp);

//struct file operatons
struct file_operations fops=
{
    .owner  = THIS_MODULE,
    .open   = NAME_open,
    .release= NAME_release,
};

struct cdev *my_cdev; //structure for char driver

//initialisation
static int __init charDev_init(void)
{
    int res;
    printk(KERN_ALERT "Initialization\n");

    if((alloc_chrdev_region(&Mydev,0,1,"mydevice3")<0)) //register device
    {
        printk(KERN_ALERT "\n Device is not allocated\n");
        return (-1);
    }
    printk(KERN_ALERT "--major num is %d and minor num is %d\n",MAJOR(Mydev),MINOR(Mydev));
    printk(KERN_ALERT "Device inserted sucessfully\n");

    my_cdev=cdev_alloc(); //allocate mem to char dev struct
    my_cdev->ops=&fops; //link to file ops

    res=cdev_add(my_cdev,Mydev,1);
    if(res<0)
    {
        printk(KERN_ALERT "\n Device is not created\n");
        unregister_chrdev_region(Mydev,1);
        return (-1);

    }
    return 0;
}

//exiting module

static void __exit charDev_exit(void)
{
    printk(KERN_ALERT "Exiting module\n");
    unregister_chrdev_region(Mydev,1); //unreg 
    cdev_del(my_cdev);
    printk(KERN_ALERT "cleanup the module\n ");
    return;
}

//system calls
int NAME_open(struct inode *inode, struct file *filp)
{
    printk(KERN_ALERT "kernel open call......\n");
    return 0;
}

int NAME_release(struct inode *inode, struct file *filp)
{
    printk(KERN_ALERT "\nkernel close call....\n");
    return 0;
}

module_init(charDev_init);
module_exit(charDev_exit);