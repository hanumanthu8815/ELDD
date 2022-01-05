#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/wait.h>

#define NAME que_device

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hanumanthu");

wait_queue_head_t wait_queue_etx;
dev_t Mydev; //global declaration
int wait_queue_flag = 0;

//fun prototypes
int NAME_open(struct inode *inode, struct file *filp);
ssize_t NAME_read(struct file *filp, char __user *Ubuff, size_t count, loff_t *offp);
ssize_t NAME_write(struct file *filp, const char __user *Ubuff, size_t count, loff_t *offp);
int NAME_release(struct inode *inode, struct file *filp);

//struct file operatons
struct file_operations fops=
{
    .owner  = THIS_MODULE,
    .open   = NAME_open,
    .read   = NAME_read,
    .write  = NAME_write,
    .release= NAME_release,
};

struct cdev *my_cdev; //structure for char driver

//initialisation
static int __init charDev_init(void)
{
    int res;
    int MAJOR, MINOR;
    Mydev = MKDEV(15,0); //will create Dev num
    MAJOR=MAJOR(Mydev);
    MINOR=MINOR(Mydev);
    printk(KERN_ALERT "initialization of mychardev\n");
    printk("the major num is %d   the minor num is %d\n",MAJOR,MINOR);
    res=register_chrdev_region(Mydev,1,"que_device"); //for registring the device
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
     //Initialize wait queue
    init_waitqueue_head(&wait_queue_etx);
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

ssize_t NAME_read(struct file *filp, char __user *Ubuff, size_t count, loff_t *offp)
{
    while(1)
    {
        printk(KERN_ALERT "Waiting For Event...\n");
        wait_event_interruptible(wait_queue_etx, wait_queue_flag != 0 );
        if(wait_queue_flag == 2) 
        {
            printk(KERN_ALERT "Event Came From Exit Function\n");
            return 0;
        }
    
    unsigned long int res;
    ssize_t retval;
    char Kbuff[100]="Hey, I am Kernel";

    printk(KERN_ALERT "In Read sys call\n");
    //wait_event_interruptible(wait_queue_etx,wait_queue_flag!=0);
    res=copy_to_user((char*)Ubuff,(char*)Kbuff,count);
    if(res==0)
    {
        printk(KERN_ALERT "\n--message to user--\n--%s--\n",Kbuff);
        printk(KERN_INFO "\n--data read successfully--\n");
        retval=count;
        return retval;
    }
    else if(res>0)
    {
        printk(KERN_ALERT "\n--msg to user--\n--%s--\n",Kbuff);
        printk(KERN_ALERT "\n the data is partially send\n");
        retval=(count-res);
        return retval;
    }
    else
    {
        printk(KERN_ALERT "\nError in reading\n");
        retval=-EFAULT;
        return retval;
    }
    }
    //printk(KERN_ALERT "Event Came From write Function - %d\n", ++read_count);
    wait_queue_flag = 0;
}

ssize_t NAME_write(struct file *filp,const char __user *Ubuff, size_t count, loff_t *offp)
{
    unsigned long int res;
    ssize_t retval;
    char Kbuff[100];

    printk(KERN_ALERT "In Write sys call\n");
    res=copy_from_user((char*)Kbuff,(char*)Ubuff,count);
    if(res==0)
    {
        printk(KERN_ALERT "\n--message from user--\n--%s--\n",Kbuff);
        printk(KERN_INFO "\n--data received successfully--\n");
        retval=count;
        return retval;
    }
    else if(res>0)
    {
        printk(KERN_ALERT "\n--msg from user--\n--%s--\n",Kbuff);
        printk(KERN_ALERT "\n the data is partially received\n");
        retval=(count-res);
        return retval;
    }
    else
    {
        printk(KERN_ALERT "\nError in writing\n");
        retval=-EFAULT;
        return retval;
    }
    wake_up_interruptible(&wait_queue_etx);


}

int NAME_release(struct inode *inode, struct file *filp)
{
    printk(KERN_ALERT "\nkernel close call....\n");
    return 0;
}

module_init(charDev_init);
module_exit(charDev_exit);