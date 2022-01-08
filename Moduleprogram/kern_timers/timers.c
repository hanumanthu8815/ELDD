/*................Timers...............*/

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/timer.h>
#include <linux/jiffies.h>

#define NAME mydevice2
dev_t Mydev; //global declaration

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


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hanumanthu");

unsigned int i=0;
struct timer_list ktimer; //declare timer variable

//function executed once the timer is fired
void sample_timer(struct timer_list *timer)
{
    printk(KERN_ALERT "Inside the function with data=%d\n",i++);
    mod_timer(&ktimer, jiffies+msecs_to_jiffies(5000)); //reset the timer
    //add_timer(&ktimer); //to make the timer periodic
}


//system calls
int NAME_open(struct inode *inode, struct file *filp)
{
    printk(KERN_ALERT "kernel open call......\n");
    return 0;
}

ssize_t NAME_read(struct file *filp, char __user *Ubuff, size_t count, loff_t *offp)
{
    printk(KERN_ALERT "in read\n");
    return 0;
}

ssize_t NAME_write(struct file *filp,const char __user *Ubuff, size_t count, loff_t *offp)
{
    printk(KERN_ALERT "in write\n");
    return 0;
}

int NAME_release(struct inode *inode, struct file *filp)
{
    printk(KERN_ALERT "\nkernel close call....\n");
    return 0;
}

//init module
static int __init timers_init(void)
{
   // printk(KERN_ALERT "In init function\n");
   int res;
    int MAJOR, MINOR;
    Mydev = MKDEV(15,0); //will create Dev num
    MAJOR=MAJOR(Mydev);
    MINOR=MINOR(Mydev);
    printk(KERN_ALERT "initialization of mychardev\n");
    printk("the major num is %d   the minor num is %d\n",MAJOR,MINOR);
    res=register_chrdev_region(Mydev,1,"mydevice2"); //for registring the device
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

    //init_timer(&ktimer); //initialise timer in old method
    timer_setup(&ktimer,&sample_timer,0);//initialise of timer
    /* setup the timer parameters*/
    //ktimer.data=100; //set a data to be accessed in the timer fun
    //ktimer.function=sample_timer; //set the fun to be called once the timer expires
    //ktimer.expires=jiffies+100; //set the delay val relative to jiffies

    mod_timer(&ktimer, jiffies+msecs_to_jiffies(100));

    //register the timer with the kernel
    //add_timer(&Ktimer);

    printk(KERN_ALERT "Timer added with an delay of 100ms\n");

    return 0;
}


//exit module
static void __exit timers_exit(void)
{
    printk(KERN_ALERT "In exit function\n");
    unregister_chrdev_region(Mydev,1); //to unregister the device num and device
    cdev_del(my_cdev);
    
    
    del_timer(&ktimer);
    printk("\ncleaning the resources\n");
    printk(KERN_ALERT "Removing done\n");
}

module_init(timers_init);
module_exit(timers_exit);