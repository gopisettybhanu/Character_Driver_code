#include<linux/init.h>
#include<linux/module.h>
#include<linux/string.h>
#include<linux/uaccess.h>
#include<linux/fs.h>
#include<linux/slab.h>
#define msg 1024
MODULE_LICENSE("GPL");
MODULE_AUTHOR("BHANU");

uint8_t *kernel_buffer;

static ssize_t fops_read (struct file *pfile, char __user *buf, size_t len, loff_t *off);
ssize_t fops_write (struct file *pfile, const char __user *buf, size_t len, loff_t *off);
int fops_open (struct inode *pinode, struct file *pfile);
int fops_close (struct inode *pinode, struct file *pfile);

static ssize_t fops_read (struct file *pfile, char __user *buf, size_t length, loff_t *off)
{
        copy_to_user(buf,kernel_buffer,msg);
	printk(KERN_INFO"DATA READ IS DONE...\n");
	return msg;
}
ssize_t fops_write (struct file *pfile, const char __user *buf, size_t len, loff_t *off)
{
        copy_from_user(kernel_buffer,buf,len);
	printk(KERN_INFO"DATA OS WRITTEN SUCCESSFULLY..\n");
	return len;
}
int fops_open (struct inode *pinode, struct file *pfile)
{
        kernel_buffer = kmalloc(msg,GFP_KERNEL);
	printk(KERN_INFO"file is opened...\n");
	return msg;
        
}
int fops_close (struct inode *pinode, struct file *pfile)
{
        kfree(kernel_buffer);
        printk(KERN_ALERT " device file is close");
        return 0;
}

struct file_operations fops={
        .owner   = THIS_MODULE,
        .open    = fops_open,
        .write   = fops_write,
        .read    = fops_read,
        .release = fops_close,
};
static int my_init(void)
{
        int result = 0;
        result=register_chrdev(200/*major number*/,"my"/*name of the driver*/,&fops/*file operations*/);
        if(result < 0)
                printk(KERN_WARNING " SIMPLE DEVICE DRIVER: can't register chr device with error code =%i\n",result);

        return result;
}
static void my_exit(void)
{
        unregister_chrdev(200,"my");
}
module_init(my_init);
module_exit(my_exit);



