/* SEMAPHORE EXAMPLE PROGRAM.................................. */

#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/kdev_t.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/semaphore.h>


#define NAME mydevice2000
struct cdev *my_cdev; 
struct semaphore sem;
struct completion data_read_done;
struct completion data_write_done;


//int completion_flag=0;

//protocol
int NAME_open(struct inode *inode,struct file *filp);
ssize_t NAME_read(struct file *filp,char __user *ubuff,size_t count,loff_t *offp);
ssize_t NAME_write(struct file *filp, const char __user *ubuff,size_t count,loff_t *offp);
int NAME_release(struct inode *inode,struct file *filp);

struct file_operations fops=
{
    .owner =THIS_MODULE,
    .open  =NAME_open,
    .read  =NAME_read,
    .write =NAME_write,
    .release=NAME_release,
};

dev_t MYDEV;

static int __init prog_init(void)
{
    int result;
    int MAJOR,MINOR;
    MYDEV=MKDEV(17,0);
    MAJOR=MAJOR(MYDEV);
    MINOR=MINOR(MYDEV);
    printk(KERN_INFO "\n THE MAJOR NUMBER %d.. THE MINOR NUMBER %d..\n",MAJOR,MINOR);
    result=register_chrdev_region(MYDEV,1,"mydevice2000");
    if(result<0)
    {
        printk(KERN_INFO "\n THE DEVICE NUMBER IS NOT REGISTERED..\n");
        return(-1);
    }
    my_cdev=cdev_alloc();
    my_cdev->ops=&fops;

    result=cdev_add(my_cdev,MYDEV,1);
    if(result<0)
    {
        printk(KERN_INFO "\n THE DEVICE NUMBER AND CDEV NOT CONNECTED.\n");
        unregister_chrdev_region(MYDEV,1);
        return(-1);
    }

    // semaphore initialize
    sema_init(&sem,1);
    // completion initialize
    init_completion(&data_read_done);
    init_completion(&data_write_done);
    
    printk(KERN_ALERT "\n SEMAPHORE IS INITAILIZED..\n");
    return 0;
    
}
static void __exit prog_exit(void)
{
    int MAJOR,MINOR;
    MYDEV=MKDEV(17,0);
    MAJOR=MAJOR(MYDEV);
    MINOR=MINOR(MYDEV);
    printk(KERN_INFO "\n THE MAJOR NUMBER %d.. THE MINOR NUMBER %d..\n",MAJOR,MINOR);
    unregister_chrdev_region(MYDEV,1);
    cdev_del(my_cdev);
    printk(KERN_INFO "\n I HAVE REMOVED ALL THE INIT....\n");
}
module_init(prog_init);
module_exit(prog_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hanumanthu");
//function definition

// Globally declared

char kbuff[60];

int NAME_open(struct inode *inode,struct file *filp)
{
    
    printk(KERN_ALERT "\n THE OPEN SYSTEM CALL IS CALLED...\n");
    return 0;
}

ssize_t NAME_read(struct file *filp,char __user *ubuff,size_t count,loff_t *offp)
{
    unsigned long result;
    ssize_t retval;
    wait_for_completion_interruptible(&data_write_done);
    result=copy_to_user((char*)ubuff,(char*)kbuff,count);
    complete(&data_read_done);
    if(result==0)
    {
        printk(KERN_ALERT "\n MESSAGE TO USER..\n...%s....\n",kbuff);
        printk(KERN_INFO  "\n DATA SEND COMPLETED..\n");
        retval=count;
        return retval;
    }
    else if(result>0)
    {  
        printk(KERN_ALERT "\n MESSAGE TO USER..\n...%s....\n",kbuff);
        printk(KERN_ALERT "\n THE PART OF DATA IS SENDED..\n ");
        retval=(count-result);
        return retval;
    }
    else
    {
        printk(KERN_ALERT "\n ERROR IN READING");
        retval=-EFAULT;
        return retval;
    }
    
}



ssize_t NAME_write(struct file *filp,const char __user *ubuff,size_t count,loff_t *offp)
{
    
    unsigned long result;
    ssize_t retval;
    down(&sem);
    printk(KERN_ALERT "\n CRITICAL SECTION STARTED..\n");
    
    result=copy_from_user((char*)kbuff,(char*)ubuff,count);
    complete(&data_write_done);
    wait_for_completion_interruptible(&data_read_done);
    up(&sem);
 
    if(result==0)
    {
        printk(KERN_ALERT "\n MESSAGE FROM USER..\n...%s....\n",kbuff);
        printk(KERN_INFO  "\n DATA RECEIVED COMPLETED..\n");
        retval=count;
        return retval;
    }
    else if(result>0)
    {  
        printk(KERN_ALERT "\n MESSAGE FROM USER..\n...%s....\n",kbuff);
        printk(KERN_ALERT "\n THE PART OF DATA IS RECEIVED..\n ");
        retval=(count-result);
        return retval;
    }
    else
    {
        printk(KERN_ALERT "\n ERROR IN WRITING");
        retval=-EFAULT;
        return retval;
    }
    
}
int NAME_release(struct inode *inode,struct file *filp)
{
    
    printk(KERN_ALERT "\n THE CLOSE SYSTEM CALL IS CALLED...\n");
    return 0;
}



