#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/kdev_t.h>

#define NO_OF_DEVICES 4

/* Device private data structure */
struct char_dev_private_data
{   
    const char *name;
    struct cdev cdev;
};
/* Drivers private data structure */
struct char_drv_private_data
{
    int total_devices;
    dev_t device_number;
    struct class *class_char_dev;
    struct device *device_char_dev;
    struct char_dev_private_data
    char_dev_data[NO_OF_DEVICES];
};

struct char_drv_private_data char_drv_data = 
{
    .total_devices = NO_OF_DEVICES,
    .char_dev_data = {
    [0] = {.name = "ADD"},
    [1] = {.name = "SUB"},
    [2] = {.name = "MUL"},
    [3] = {.name = "DIV"}
    }
};
int char_dev_open(struct inode *inode, struct file *filp)
{
    int minor;
    struct char_dev_private_data *char_dev_data;
/*find out on which device file open was attempted by the
user space*/
    minor = MINOR(inode->i_rdev);
    pr_info("Minor access = %d\n",minor);
/*get device's private data structure*/
    char_dev_data = container_of(inode->i_cdev,struct
    char_dev_private_data,cdev);
/*to supply device private data to other methods of the
driver*/
    filp->private_data = char_dev_data;
    pr_info("open was successful\n");
    return 0;
}
int char_dev_release(struct inode *inode, struct file *filp)
{
    pr_info("release was successful\n");
    return 0;
}
ssize_t char_dev_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos)
{
    return 0;
}

ssize_t char_dev_write(struct file *filp, const char __user *buff,size_t count, loff_t *f_pos)
{
    unsigned long int res;
    ssize_t retval;
    int Ubuff[0];
    int Ubuff[1];
    char Kbuff[100];
    int add;

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
    add=Ubuff[0]+Ubuff[1];
    printk(KERN_ALERT "add=%d",add);
    return 0;
}
/* file operations of the driver */
struct file_operations char_dev_fops=
{
    .open = char_dev_open,
    .release = char_dev_release,
    .read = char_dev_read,
    .write = char_dev_write,
    .owner = THIS_MODULE
};
static int __init char_drv_init(void)
{
    int ret;
    int i;
/*Dynamically allocate  device numbers */
    ret = alloc_chrdev_region(&char_drv_data.device_number,0,NO_OF_DEVICES,"calculator");
    if(ret < 0)
    {
        pr_err("Alloc chrdev failed\n");
        goto out;
    }

/*create device class under /sys/class/ */
    char_drv_data.class_char_dev = class_create(THIS_MODULE,"calculator_class");
    if(IS_ERR(char_drv_data.class_char_dev))
    {
        pr_err("Class creation failed\n");
        ret = PTR_ERR(char_drv_data.class_char_dev);
        goto unreg_chrdev;
    }
    for(i = 0 ; i < NO_OF_DEVICES ; i++)
    {
        pr_info("Device number <major>:<minor> =%d:%d\n",MAJOR(char_drv_data.device_number+i),MINOR(char_drv_data.device_number+i));
/*Initialize the cdev structure with fops*/
        cdev_init(&char_drv_data.char_dev_data[i].cdev,&char_dev_fops);
/*  Register a device (cdev structure) with VFS */
        char_drv_data.char_dev_data[i].cdev.owner =THIS_MODULE;
        ret =cdev_add(&char_drv_data.char_dev_data[i].cdev,char_drv_data.device_number+i,1);
        if(ret < 0)
        {
            pr_err("Cdev add failed\n");
            goto cdev_del;
        }
/*populate the sysfs with device information */
        char_drv_data.device_char_dev = device_create(char_drv_data.class_char_dev,NULL,char_drv_data.device_number+i,NULL,"dev-%d",i+1);
        if(IS_ERR(char_drv_data.device_char_dev))
        {
            pr_err("Device create failed\n");
            ret = PTR_ERR(char_drv_data.device_char_dev);
            goto class_del;
        }
    }
    pr_info("Module init was successful\n");
    return 0;
cdev_del:
class_del:
    for(;i>=0;i--)
    {
        device_destroy(char_drv_data.class_char_dev,char_drv_data.device_number+i);
        cdev_del(&char_drv_data.char_dev_data[i].cdev);
    }
    class_destroy(char_drv_data.class_char_dev);
    unreg_chrdev:

    unregister_chrdev_region(char_drv_data.device_number,NO_OF_DEVICES);
out:
    return ret;
}
static void __exit char_drv_cleanup(void)
{
    int i;
    for(i=0;i<NO_OF_DEVICES;i++)
    {
        device_destroy(char_drv_data.class_char_dev,char_drv_data.device_number+i);
        cdev_del(&char_drv_data.char_dev_data[i].cdev);
    }
    class_destroy(char_drv_data.class_char_dev);
    unregister_chrdev_region(char_drv_data.device_number,NO_OF_DEVICES);
    pr_info("module unloaded\n");
}
module_init(char_drv_init);
module_exit(char_drv_cleanup);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("PRASAD - NISSI");
MODULE_DESCRIPTION("A character driver which handles n devices");

