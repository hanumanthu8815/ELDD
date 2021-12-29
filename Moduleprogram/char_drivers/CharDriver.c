/* program for basic functionality of char driver*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#define NAME MyCharDevice

//prototypes
int NAME_open(struct inode *inode, struct file *filp);
int NAME_release(struct inode *inode, struct file *filp);
ssize_t NAME_write(struct file *filp, const char __user *Ubuff, size_t count, loff_t *offp);
ssize_t NAME_read(struct file *filp, char __user *Ubuff, size_t count, loff_t *offp);

//driver provides the operations within the structure
struct file_operations fops =
{
	.owner  = THIS_MODULE,
	.open   = NAME_open,
	.read   = NAME_read,
	.write  = NAME_write,
	.release= NAME_release,
};

//structure of char driver
struct cdev *my_cdev;

//chardriver structure
static int __init CharDevice_init(void)
{
	int result;
	int MAJOR,MINOR;
	dev_t Mydev;
	Mydev = MKDEV(255,0);//device create
	MAJOR=MAJOR(Mydev);
	MINOR=MINOR(Mydev);
	printk("\nthe major number is %d the minor number us %d\n",MAJOR,MINOR);
	result=register_chrdev_region(Mydev,1,"MyCharDevice");//reg device region
	if(result<0)
	{
		printk(KERN_ALERT "\n region is not obtained\n");
		return(-1);
	}

	my_cdev = cdev_alloc();//allocate mem for char device struct
	my_cdev->ops = &fops;//linking fops to char device

	result=cdev_add(my_cdev,Mydev,1);
	if(result<0)
	{
		printk(KERN_ALERT "\nchar device not created\n");
		unregister_chrdev_region(Mydev,1);
		return(-1);
	}
	return 0;
}


//cleanup module
void __exit CharDevice_exit(void)
{
	dev_t Mydev;
	int MAJOR,MINOR;
	Mydev=MKDEV(255,0);
	MAJOR=MAJOR(Mydev);
	MINOR=MINOR(Mydev);
	printk("\nthe major num is %d  the minor num is %d\n",MAJOR,MINOR);
	unregister_chrdev_region(Mydev,1);//unreg the device numbers and device ctreated
	cdev_del(my_cdev);
	printk(KERN_ALERT "\ngoodbye\n");
	return;
}


//open sys call
int NAME_open(struct inode *inode, struct file *filp)
{
	printk(KERN_ALERT "\nthis is kernel----open call\n");
	return 0;
}
//close sys call
int NAME_release(struct inode *inode, struct file *filp)
{
	printk(KERN_ALERT "\nrelease method\n");
	return 0;
}

//write functionality
ssize_t NAME_write(struct file *filp,const char __user *Ubuff, size_t count, loff_t *offp)
{
	char Kbuff[80];
	unsigned long result;
	ssize_t retval;
	result=copy_from_user((char *)Kbuff,(char *)Ubuff,count);//getting usr data
	if(result==0)
	{
		printk(KERN_ALERT "\nmsg from usr--\n>>> %s <<<\n",Kbuff);
		printk(KERN_ALERT "\ndata written\n");
		retval=count;
		return retval;
	}
	else if(result>0)
	{
		printk(KERN_ALERT "\nmsg from usr---\n>>> %s <<<\n",Kbuff);
		printk(KERN_ALERT "\n data copied\n");
		retval=(count-result);
		return retval;
	}
	else
	{
		printk(KERN_ALERT "\n error writing data\n");
	        retval=-EFAULT;
	        return retval;
	}
}

//read functionality
ssize_t NAME_read(struct file *filp, char __user *Ubuff, size_t count, loff_t *offp)
{
	char Kbuff[]="this is some data from the kernel to the user---user";
	unsigned long result;
	ssize_t retval;
	result=copy_to_user((char *)Ubuff,(char *)Kbuff,count);//copy to user
	if(result==0)
	{
		printk(KERN_ALERT "\ndata read\n");
		retval=count;
		return retval;
	}
	else if(result>0)
	{
		printk(KERN_ALERT "\ndata available\n");
		retval=(count-result);
		return retval;
	}
	else
	{
		printk(KERN_ALERT "\nerror writing data to user\n");
		retval=-EFAULT;
		return retval;
	}
}

int NAME_flush(struct file *filp)
{
	printk("\nthis is close fun of the file");
	return 0;
}

//module over ride funs
module_init(CharDevice_init);
module_exit(CharDevice_exit);

MODULE_LICENSE("GPL");
