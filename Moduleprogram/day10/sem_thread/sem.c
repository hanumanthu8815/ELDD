// demo program semaphore used between threads

#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/types.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/semaphore.h>
#include<linux/kthread.h>
#include<linux/sched.h>
#include<linux/slab.h>
#include<linux/device.h>


dev_t dev;
struct cdev my_cdev;
static int global_var=0;
static struct task_thread *kthread_5;
static struct task_thread *kthread_6;
struct semaphore sem_1;
struct semaphore sem_2;

int thread_function1(void * arg);
int thread_function2(void * arg);


int thread_function1(void * arg)  //
{
    down(&sem_1);
    global_var++;
    printk("THREAD 1 : %d\n",global_var);
    up(&sem_1);
    return 0;
}

int thread_function2(void * arg)  //
{
    down(&sem_2);
    global_var++;
    printk("THREAD 2 : %d\n",global_var);
    up(&sem_2);
    return 0;
}
//protocol
int GOOD_open(struct inode *inode,struct file *filp);
ssize_t GOOD_read(struct file *filp,char __user *ubuff,size_t count,loff_t *offp);
ssize_t GOOD_write(struct file *filp, const char __user *ubuff,size_t count,loff_t *offp);
int GOOD_release(struct inode *inode,struct file *filp);

struct file_operations fops=
{
    .owner =THIS_MODULE,
    .open  =GOOD_open,
    .read  =GOOD_read,
    .write =GOOD_write,
    .release=GOOD_release,
};

static int __init prog_init(void)
{
    if((alloc_chrdev_region(&dev,30,1,"GOOD"))<0)
    {
        printk("\n cannot create major number..\n");
        return -1;
    }
    // cdev structure

    cdev_init(&my_cdev,&fops);

    // adding major and cdev

    if((cdev_add(&my_cdev,dev,1))<0)
    {
        printk("\n cannot add major number cdev..\n");
        unregister_chrdev_region(dev,1);
        return -1;
    }
    // create thread
    printk("\n threads are created..!\n");
    kthread_5=kthread_run(thread_function1,NULL,"NAME_kthread_5");
    if(kthread_5 !=NULL)
    {
        printk("\n THREAD 1  CREATED..\n");
        
    }
    else
    {
        printk("\n THREAD NOT CREATED..\n");
        kthread_stop(kthread_5);
        return -1;
    }
    kthread_6=kthread_run(thread_function2,NULL,"NAME_kthread_6");
    if(kthread_6 !=NULL)
    {
        printk("\n THREAD 2 CREATED..\n");
        
    }
    else
    {
        printk("\n THREAD 2 NOT CREATED..\n");
        kthread_stop(kthread_6);
        return -1;
    }
    sema_init(&sem_1,1);
    sema_init(&sem_2,1);
    printk("\n SEMAPHORE INITAILIZED AND DRIVER LOADED..\n");
    return 0;
}
static void __exit prog_exit(void)
{
    unregister_chrdev_region(dev,1);
    cdev_del(&my_cdev);
    printk("\n DRIVER UNLOADED..\n");
}
module_init(prog_init);
module_exit(prog_exit);

