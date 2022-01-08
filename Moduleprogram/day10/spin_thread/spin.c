// sequential lock demo progarm 1

#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/types.h>
#include<linux/uaccess.h>
#include<linux/kthread.h>
#include<linux/sched.h>
#include<linux/delay.h>
#include<linux/seqlock.h>
#include<linux/cdev.h>
#include<linux/slab.h>    //kmalloc()
#include<linux/device.h>

//seqlock declaration

spinlock_t lock;
unsigned long global_var=0;
dev_t dev;
static struct cdev my_cdev;
static struct task_struct *kthread_1;
static struct task_struct *kthread_2;

//Function declaration:
int CAN_open(struct inode *inode,struct file *filp);
ssize_t CAN_read(struct file *filp,char __user *ubuff,size_t count,loff_t *offp);
ssize_t CAN_write(struct file *filp, const char __user *ubuff,size_t count,loff_t *offp);
int CAN_release(struct inode *inode,struct file *filp);

//function of thread
int thread_function1(void *arg);
int thread_function2(void *arg);

int thread_function1(void *arg)
{
    while(!kthread_should_stop())
    {
        spin_lock(&lock);
        global_var++;
        printk("THREAD FUNCTION READ VALUE FUNCTION1 %lu\n",global_var);
        spin_unlock(&lock);
        msleep(2000);
    }
    return 0;
}
int thread_function2(void *arg)
{
    
    while(!kthread_should_stop())
    {
        spin_lock(&lock);
        global_var++;
        printk("THREAD FUNCTION READ VALUE FUNCTION2 %lu\n",global_var);
        spin_unlock(&lock);
        msleep(2000);
    }
    return 0;
}
struct file_operations fops=
{
    .owner  =   THIS_MODULE,
    .open   =   CAN_open,
    .read   =   CAN_read,
    .write  =   CAN_write,
    .release=   CAN_release,
};

int CAN_open(struct inode *inode,struct file *filp)
{
    printk("\n OPEN FUNCTION..\n");
    return 0;
}
int CAN_release(struct inode *inode,struct file *filp)
{
    printk("\n CLOSE FUNCTION..\n");
    return 0;
}
ssize_t CAN_read(struct file *filp,char __user *ubuff,size_t count,loff_t *offp)
{
    printk("\n READ FUNCTION..\n");
    return 0;
}    
ssize_t CAN_write(struct file *filp,const char __user *ubuff,size_t count,loff_t *offp)
{
    printk("\n WRITE FUNCTION..\n");
    return count;
}   

static int __init prog_init(void)
{
    if((alloc_chrdev_region(&dev,28,1,"CAN"))<0)
    {
        printk("\n cannot create major number..\n");
        return -1;
    }
    // cdev structure

    cdev_init(&my_cdev,&fops);

    // addind cdev and major

    if((cdev_add(&my_cdev,dev,1))<0)
    {
        printk("\n cannot add cdev and major..\n");
        unregister_chrdev_region(dev,1);
        return -1;
    }

    // creating threads
    printk("\n creating threads..\n");
    kthread_1=kthread_run(thread_function1,NULL,"CAN_THREAD_1");
    if(kthread_1 !=NULL)
    {
        printk("\n Thread 1 created and running..\n");
    }
    else
    {
        printk("\n Thread 1 not created..\n");
        kthread_stop(kthread_1);
        return -1;
    }
    kthread_2=kthread_run(thread_function2,NULL,"CAN_THREAD_2");
    if(kthread_2 !=NULL)
    {
        printk("\n Thread 2 created and running..\n");
    }
    else
    {
        printk("\n Thread 2 not created..\n");
        kthread_stop(kthread_2);
        return -1;
    }
    spin_lock_init(&lock);
    printk("\n DRIVER LOADED..\n");
    return 0;
}
static void __exit prog_exit(void)
{
    //stop the thread
    kthread_stop(kthread_1);
    kthread_stop(kthread_2);
    unregister_chrdev_region(dev,1);
    cdev_del(&my_cdev);
    printk("\n DRIVER UNLOADED..\n");
}
module_init(prog_init);
module_exit(prog_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("SRA");