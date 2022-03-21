#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h> /* for our device file */
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h> /* for kmalloc */
#include <linux/uaccess.h> /* for communicationg between userspace and kernel space */

#define mem_size 1024

dev_t dev = 0;
static struct class *dev_class;
static struct cdev my_cdev; 
uint8_t *kernel_buffer;



/* prototypes */
static int __init chr_driver_init(void);
static void __exit chr_driver_exit(void);
static int my_open(struct inode *inode, struct file *file);
static int my_release(struct inode *inode, struct file *file);
static ssize_t my_read(struct file *filep, char __user *buf, size_t len, loff_t *off);
static ssize_t my_write(struct file *filep, const char *buf, size_t len, loff_t *off);


static int my_open(struct inode *inode, struct file *file){
    /* creating  physical memory */
    if((kernel_buffer = kmalloc(mem_size, GFP_KERNEL)) == 0){
        printk(KERN_INFO "cannot alllocate memory to the kernel....\n");
        return -1;
    }

    printk(KERN_INFO "device file opened....\n");
    return 0;
}


static int my_release(struct inode *inode, struct file *file){
    kfree(kernel_buffer);
    printk(KERN_INFO "device file closed....\n");
    return 0;
}


static ssize_t my_read(struct file *filep, char __user *buf, size_t len, loff_t *off){
    if((copy_to_user(buf, kernel_buffer, mem_size)) < 0){
        printk(KERN_INFO "error in copy to user....\n");
        return -1;
    }
    printk(KERN_INFO "data read done....\n");
    return mem_size;
}


static ssize_t my_write(struct file *filep, const char *buf, size_t len, loff_t *off){
    if((copy_from_user(kernel_buffer, buf, len)) < 0){
        printk(KERN_INFO "error in copy from user....\n");
        return -1;
    }
    printk(KERN_INFO "data is written....\n");
    return len;
}





/* file operation structer, to read, write, open, close */
static struct file_operations fops = {
    .owner             = THIS_MODULE,
    .read              = my_read,
    .write             = my_write,
    .open              = my_open,
    .release           = my_release,
};


/* body of init and exit */
static int __init chr_driver_init(void){

    /* allocating major number */
    if((alloc_chrdev_region(&dev, 0, 1, "my_chr_dev")) < 0){
        printk(KERN_INFO "cannot allocate the major number....\n");
        return -1;
    }

    printk(KERN_INFO "major num: %d   minor num: %d\n", MAJOR(dev), MINOR(dev));

    /* create a cdev structure */
    cdev_init(&my_cdev, &fops);

    /* add character device to the system */
   if((cdev_add(&my_cdev, dev, 1)) < 0){
       printk(KERN_INFO "cannot add the device to the system....\n");
       goto r_class;
   } 

   /* create struct class */
   if((dev_class = class_create(THIS_MODULE, "my_class")) == NULL){
       printk(KERN_INFO "cannot create the struct class....\n");
       goto r_class;
   }

   /* creating device */
   if((device_create(dev_class, NULL, dev, NULL, "my_device")) == NULL){
       printk(KERN_INFO "cannot create the device..\n");
       goto r_device;
   }

    printk(KERN_INFO "device driver is inserted properly....\n");

    r_device:
        class_destroy(dev_class);

    r_class:
        unregister_chrdev_region(dev, 1);
        return -1;

}



void __exit chr_driver_exit(void){
    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "character driver is removed successfully....\n");
}


module_init(chr_driver_init);
module_exit(chr_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mohit Sinha");
MODULE_DESCRIPTION("simple character driver");

