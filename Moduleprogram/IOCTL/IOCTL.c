#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/ioctl.h>
#include "ioctl.h"


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hanumanthu");

dev_t Mydev; //global declaration

//fun prototypes
int NAME_open(struct inode *inode, struct file *filp);
ssize_t NAME_read(struct file *filp, char __user *Ubuff, size_t count, loff_t *offp);
ssize_t NAME_write(struct file *filp, const char __user *Ubuff, size_t count, loff_t *offp);
int NAME_release(struct inode *inode, struct file *filp);
long NAME_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);

//struct file operatons
struct file_operations fops=
{
    .owner  = THIS_MODULE,
    .open   = NAME_open,
    .read   = NAME_read,
    .write  = NAME_write,
    .release= NAME_release,
    .unlocked_ioctl=NAME_ioctl,
};


struct DefaultData {
    int Baudrate;
    int Parity;
    int StopBits;
};

//struct for a char driver
struct cdev *my_cdev;

//initialisation module
static int __init CharDevice_init(void)
{
    int res;
    int MAJOR,MINOR;
    res=alloc_chrdev_region(&Mydev,0,1,"IOCTL_Device");//register device region
    if(res<0)
    {
        printk(KERN_ALERT "\nthe region requested is not available\n");
        return (-1);
    }

    MAJOR=MAJOR(Mydev);
    MINOR=MINOR(Mydev);
    printk("\n Major num is %d---Minor num is %d\n",MAJOR,MINOR);
    my_cdev=cdev_alloc();//allocating memory for char device structure
    my_cdev->ops=&fops;//link file ops to char device

    res=cdev_add(my_cdev,Mydev,1);//notify the kernel about new device
    if(res<0)
    {
        printk(KERN_ALERT "\nchar device is not created\n");
        return (-1);
    }
    return 0;
}
//cleanup module
void __exit CharDevice_exit(void)
{
    int MAJOR,MINOR;
    MAJOR=MAJOR(Mydev);
    MINOR=MINOR(Mydev);
    printk("\nMajor num is %d---Minor num is %d\n",MAJOR,MINOR);
    unregister_chrdev_region(Mydev,1);
    cdev_del(my_cdev);
    printk(KERN_ALERT "\nunregister the stuff that was allocated\n");
    return;
}

//system call
int NAME_open(struct inode *inode, struct file *filp)
{
    printk(KERN_ALERT "\nKernel opennnnnnn callllllll\n");
    return 0;
}

int NAME_release(struct inode *inode, struct file *filp)
{
    printk(KERN_ALERT "\nKernel closeeeeeeee callllllll\n");
    return 0;
}

ssize_t NAME_write(struct file *filp, const char __user *Ubuff, size_t count, loff_t *offp)
{
    char Kbuff[100];
    unsigned long res;
    ssize_t retval;
    res=copy_from_user((char *)Kbuff,(char *)Ubuff,count);//get user data
    if(res==0)
    {
        printk(KERN_ALERT "\nMessage from user----\n>>> %s <<<\n",Kbuff);
        printk(KERN_ALERT "\n data written successfully\n");
        retval=count;
        return retval;
    }
    else
    {
        printk(KERN_ALERT "\n Error writing data\n");
        retval=-EFAULT;
        return retval;
    }
}

ssize_t NAME_read(struct file *filp,char __user *Ubuff, size_t count, loff_t *offp)
{
    char Kbuff[]="This is some data from the kernel";
    unsigned long res;
    ssize_t retval;
    res=copy_to_user((char *)Ubuff,(char *)Kbuff,sizeof(Kbuff));//copy to user
    if(res==0)
    {
        printk(KERN_ALERT "\nData read successfully\n");
        retval=count;
        return retval;
    }
    else
    {
        printk(KERN_ALERT "\n error writing data to user\n");
        retval=-EFAULT;
        return retval;
    }
}

long NAME_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    unsigned int temp, STOP_BITS;
    //ssize_t retval=SUCCESS;
    char Ubuff[20];
    struct DefaultData *ToSend;
    printk("\n IOCTL function");
    switch (cmd) {
        case SET_BAUD_RATE:
        get_user(temp, (int __user *)arg);
        printk("\nsetting the baud rate to %ld", temp);
        temp=10;
        put_user(temp, (int __user *)arg);
        break;
        
        case SET_DIRECTION_WRITE:
        printk("\n the direction is set to write");
        break;

        case SET_NO_STOP_BITS:
        get_user(STOP_BITS,(int __user *)arg);
        printk("\n setting the num of two nits to %d", STOP_BITS);
        break;

        case DEFAULT_DATA:
        ToSend=(struct DefaultData *)Ubuff;
        copy_from_user(Ubuff,(struct DefaultData *)arg,sizeof(struct DefaultData));
        printk("Default data is %d, %d, %d",ToSend->Baudrate, ToSend->Parity, ToSend->StopBits);
        break;

        default:
        printk("\nCommand not found");
        return (-EINVAL);
    }
    return 0;
    
}

module_init(CharDevice_init);
module_exit(CharDevice_exit);