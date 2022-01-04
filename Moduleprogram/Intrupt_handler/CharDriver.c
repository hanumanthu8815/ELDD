#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/slab.h>
#include<linux/uaccess.h>
#include<linux/sysfs.h>
#include<linux/kobject.h>
#include<linux/interrupt.h>
#include<asm/io.h>

#define IRQ_NO 1
unsigned int i=0;
//interrupt handler for IRQ 1.

static irqreturn_t irq_handler(int irq, void *dev_id)
{
    printk(KERN_INFO "keyboard: Interrupt occured %d\n",i);
    i++;
    return IRQ_HANDLED;
}
volatile int chr_value=0;

dev_t dev=0;
static struct class *dev_class;
static struct cdev chr_cdev;
struct kobject *kobj_ref;

static int __init chr_driver_init(void);
static void __exit chr_driver_exit(void);

//driver functions
static int chr_open(struct inode *inode, struct file *file);
static int chr_release(struct inode *inode, struct file *file);
static ssize_t chr_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t chr_write(struct file *filp,const char *buf, size_t len, loff_t * off);

static struct file_operations fops=
{
    .owner  = THIS_MODULE,
    .read   = chr_read,
    .write  = chr_write,
    .open   = chr_open,
    .release= chr_release,
};

static int chr_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Device file opened.....\n");
    return 0;
}

static int chr_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Device file closed.....\n");
    return 0;
}

static ssize_t chr_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "Read function.....\n");
    return 0;
}

static ssize_t chr_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "Erite function.....\n");
    return 0;
}

//Initialisation
static int __init chr_driver_init(void)
{
    //allocating major num
    if((alloc_chrdev_region(&dev, 0, 1, "chr_dev"))<0)
    {
        printk(KERN_INFO "cannot allocate major num\n");
        return -1;
    }
    printk(KERN_INFO "Major=%d Minor=%d \n", MAJOR(dev), MINOR(dev));

    //create cdev struct
    cdev_init(&chr_cdev,&fops);

    //add char device 
    if((cdev_add(&chr_cdev,dev,1))<0)
    {
        printk(KERN_INFO "cannot add device\n");
        goto r_class;
    }

    //create struct class
    if((dev_class=class_create(THIS_MODULE,"chr_class"))==NULL)
    {
        printk(KERN_INFO "cannot create struct class\n");
        goto r_class;
    }

    //create device
    if((device_create(dev_class,NULL,dev,NULL,"chr_device"))==NULL)
    {
        printk(KERN_INFO "cannot create device\n");
        return 0;
    }

    if(request_irq(IRQ_NO, irq_handler, IRQF_SHARED, "chr_device", (void *)(irq_handler)))
    {
        printk(KERN_INFO "chr_device: cannot regidter IRQ");
        goto irq;
    }
    printk(KERN_INFO "Device driver insert done\n");
    return 0;

irq: 
        free_irq(IRQ_NO,(void *)(irq_handler));

r_device:
        class_destroy(dev_class);

r_class:
        unregister_chrdev_region(dev,1);
        cdev_del(&chr_cdev);
        return -1;
}

//exiting
void __exit chr_driver_exit(void)
{
    free_irq(IRQ_NO,(void *)(irq_handler));
    device_destroy(dev_class,dev);
    class_destroy(dev_class);
    cdev_del(&chr_cdev);
    unregister_chrdev_region(dev,1);
    printk(KERN_INFO "removing driver done\n");
}

module_init(chr_driver_init);
module_exit(chr_driver_exit);
MODULE_LICENSE("GPL");