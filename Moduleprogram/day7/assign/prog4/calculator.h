#define NO_OF_DEVICE 4
#define data_size 4 //sizeof(int)

int device1_operands[2]={0};
int device2_operands[2]={0};
int device3_operands[2]={0};
int device4_operands[2]={0};

struct dev_private_data
{
    const char* name;
    int* buffer;
    unsigned size;
    struct cdev cdev;
};

struct driver_private_data
{
    int total_devices;
    dev_t device_number;
    struct dev_private_data dev_data[NO_OF_DEVICE];
    
};

struct driver_private_data drv_data={
    .total_devices=NO_OF_DEVICE,
    .dev_data={
        [0]={
            .buffer=device1_operands,
            .size=2,
            .name="ADD"
        },
        [1]={
            .buffer=device2_operands,
            .size=2,
            .name="SUB"
        },
        [2]={
            .buffer=device3_operands,
            .size=2,
            .name="MUL"
        },
        [3]={
            .buffer=device4_operands,
            .size=2,
            .name="DIV"
        }
    }
};

static int cal_open(struct inode *inode,struct file *filp);
static int cal_release(struct inode *inode,struct file *filp);

static ssize_t add_write(struct file *filp,const char *buff,size_t count,loff_t *off);
static ssize_t add_read(struct file *filp,char *buff,size_t count,loff_t *off);

static ssize_t sub_write(struct file *filp,const char *buff,size_t count,loff_t *off);
static ssize_t sub_read(struct file *filp,char *buff,size_t count,loff_t *off);

static ssize_t mul_write(struct file *filp,const char *buff,size_t count,loff_t *off);
static ssize_t mul_read(struct file *filp,char *buff,size_t count,loff_t *off);

static ssize_t div_write(struct file *filp,const char *buff,size_t count,loff_t *off);
static ssize_t div_read(struct file *filp,char *buff,size_t count,loff_t *off);


