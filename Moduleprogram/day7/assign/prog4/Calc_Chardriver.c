// calculator program with addition ,sun=btraction,multiplication and division

#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/cdev.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/types.h>
#include<linux/uaccess.h>
#include "calculator.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hanumanthu");

//static size_t arr_index=0;
int buffer[2];

static struct file_operations fops={
    .owner     =    THIS_MODULE,
    .open      =    cal_open,
    .release   =    cal_release,
    .read      =    add_read,
    .write     =    add_write,
};


// init function

static int __init prog_init(void)
{
    int ret;
    size_t i,j;
    drv_data.device_number=MKDEV(255,0);
    ret=register_chrdev_region(drv_data.device_number,4,"calculator");
    if(ret<0)
    {
        printk("\n cannot allocated device number..!\n");
        return -1;
    }
    i=0;
    while(i<4)
    {
        printk(KERN_INFO "\n device number <major> : <minor> %d:%d\n",MAJOR(drv_data.device_number+i),MINOR(drv_data.device_number+i));


        cdev_init(&drv_data.dev_data[i].cdev,&fops);

        if(cdev_add(&drv_data.dev_data[i].cdev,drv_data.device_number+i,1)<0)
        {
            j=0;
            while(j<i)
            {
                unregister_chrdev_region(drv_data.device_number+j,1);
                return -1;
            }
            
        }
        printk("\n DRIVER LOADED..\n");
        i++;
    }
    return 0;
}
static void __exit prog_exit(void)
{
    size_t i;
    unregister_chrdev_region(drv_data.device_number,4);
    i=0;
    while(i<4)
    {
        cdev_del(&drv_data.dev_data[i].cdev);
        i++;
    }
    printk("\n DRIVER UNLOADED..\n");
}

module_init(prog_init);
module_exit(prog_exit);


static int cal_open(struct inode *inode,struct file *filp)
{
    unsigned int minor_num;
    printk("\n OPEN CALL..\n");

    //find which device is opened with minor number
    minor_num=MINOR(inode -> i_rdev);
    printk("\n minor number for which driver invoked..%d\n",minor_num);

    struct file_operations *new_fops=filp->f_op;

//    printk("FUNCTION POINTERS %p and %p \n",new_fops->write,new_fops->read);
    switch(minor_num)
    {
        case 1:
            new_fops->write=sub_write;
            new_fops->read=sub_read;
            break;
        case 2:
            new_fops->write=mul_write;
            new_fops->read=mul_read;
            break;
        case 3:
            new_fops->write=div_write;
            new_fops->read=div_read;
            break;
        default:
            break;

    }
    return 0;
}
static ssize_t add_write(struct file *filp,const char *buf,size_t count,loff_t *off)
{
    ssize_t retval=-1;
    unsigned long result=0;

    result=copy_from_user(buffer,(int*)buf,count);
    if(result==0)
    {
        printk("\n wriiten successfully.\n");
        retval=count;
        return retval;
    }
    else if(result>0)
    {
        retval=count-result;
        return retval;
    }
    printk("\nerror writing data\n");
    return retval;
}
static ssize_t add_read(struct file *filp,char *buf,size_t count,loff_t *off)
{
    ssize_t retval=-1;
    int add=0;
    unsigned long result;
    
    printk("\n kernel add read call\n");
    printk("\n 1st operand & 2nd operand in read %d %d \n",buffer[0],buffer[1]);
    add=buffer[0]+buffer[1];

    result=copy_to_user(buf,&add,count);
    printk("copied buffer %s\n",buf);

    if(result==0)
    {
        retval=count;
    }
    else if(result>0)
    {
        retval=count-result;
    }
    return retval;
}

//subtraction
static ssize_t sub_write(struct file *filp,const char *buf,size_t count,loff_t *off)
{
    ssize_t retval=-1;
    unsigned long result=0;

    result=copy_from_user(buffer,(int*)buf,count);
    if(result==0)
    {
        printk("\n wriiten successfully.\n");
        retval=count;
        return retval;
    }
    else if(result>0)
    {
        retval=count-result;
        return retval;
    }
    printk("\nerror writing data\n");
    return retval;
}
static ssize_t sub_read(struct file *filp,char *buf,size_t count,loff_t *off)
{
    ssize_t retval=-1;
    int sub=0;
    unsigned long result;
    
    printk("\n kernel add read call\n");
    printk("\n 1st operand & 2nd operand in read %d %d \n",buffer[0],buffer[1]);
    sub=buffer[0]-buffer[1];

    result=copy_to_user(buf,&sub,count);
    printk("copied buffer %s\n",buf);

    if(result==0)
    {
        retval=count;
    }
    else if(result>0)
    {
        retval=count-result;
    }
    return retval;
}

//multiplication

static ssize_t mul_write(struct file *filp,const char *buf,size_t count,loff_t *off)
{
    ssize_t retval=-1;
    unsigned long result=0;

    result=copy_from_user(buffer,(int*)buf,count);
    if(result==0)
    {
        printk("\n wriiten successfully.\n");
        retval=count;
        return retval;
    }
    else if(result>0)
    {
        retval=count-result;
        return retval;
    }
    printk("\nerror writing data\n");
    return retval;
}
static ssize_t mul_read(struct file *filp,char *buf,size_t count,loff_t *off)
{
    ssize_t retval=-1;
    int mul=0;
    unsigned long result;
    
    printk("\n kernel add read call\n");
    printk("\n 1st operand & 2nd operand in read %d %d \n",buffer[0],buffer[1]);
    mul=(buffer[0])*(buffer[1]);

    result=copy_to_user(buf,&mul,count);
    printk("copied buffer %s\n",buf);

    if(result==0)
    {
        retval=count;
    }
    else if(result>0)
    {
        retval=count-result;
    }
    return retval;
}


//division

static ssize_t div_write(struct file *filp,const char *buf,size_t count,loff_t *off)
{
    ssize_t retval=-1;
    unsigned long result=0;

    result=copy_from_user(buffer,(int*)buf,count);
    if(result==0)
    {
        printk("\n wriiten successfully.\n");
        retval=count;
        return retval;
    }
    else if(result>0)
    {
        retval=count-result;
        return retval;
    }
    printk("\nerror writing data\n");
    return retval;
}
static ssize_t div_read(struct file *filp,char *buf,size_t count,loff_t *off)
{
    ssize_t retval=-1;
    int div=0;
    unsigned long result;
    
    printk("\n kernel add read call\n");
    printk("\n 1st operand & 2nd operand in read %d %d \n",buffer[0],buffer[1]);
    div=(buffer[0])/(buffer[1]);

    result=copy_to_user(buf,&div,count);
    printk("copied buffer %s\n",buf);

    if(result==0)
    {
        retval=count;
    }
    else if(result>0)
    {
        retval=count-result;
    }
    return retval;
}

static int cal_release(struct inode *inode,struct file *filp)
{
    printk("\n CLOSE CALL.\n");
    return 0;
}