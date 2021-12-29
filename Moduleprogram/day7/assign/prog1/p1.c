#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>

#define NAME mydevice1

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hanumanthu");

dev_t Mydev; //global declaration

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
    int MAJOR, MINOR;
    Mydev = MKDEV(2,0); //will create Dev num
    MAJOR=MAJOR(Mydev);
    MINOR=MINOR(Mydev);
    printk(KERN_ALERT "initialization of mychardev\n");
    printk("the major num is %d   the minor num is %d\n",MAJOR,MINOR);
    res=register_chrdev_region(Mydev,1,"mydevice1"); //for registring the device
    if(res<0)
    {
        printk(KERN_ALERT "\nregion is not there|n");
        return (-1);
    }

    my_cdev = cdev_alloc(); //allocate mem for chardevice struct
    my_cdev->ops=&fops; //to link file operations

    res=cdev_add(my_cdev,Mydev,1); //kernel will know about the new device
    if(res<0)
    {
        printk(KERN_ALERT "\n device not created\n");
        unregister_chrdev_region(Mydev,1);
        return(-1);
    }
    return 0;
}

//exiting the module
void __exit charDev_exit(void)
{
    printk(KERN_ALERT "exit the module\n");
    unregister_chrdev_region(Mydev,1); //to unregister the device num and device
    cdev_del(my_cdev);
    printk("\ncleaning the resources\n");
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