#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>
#include <linux/string.h>

#define STR_SIZE 30

#define CHRDEV_NAME "kmod"
#define PROC_NAME "test_proc"
#define SYS_NAME "test_kobject_sysfs"

static int major = 0;
static struct proc_dir_entry* test_proc = NULL;
static struct kobject* test_kobj_sysfs = NULL;
static char module_string[STR_SIZE] = "Hello!";

static ssize_t test_read(struct file* fd, char __user* buff, size_t size, loff_t* off)
{
    return simple_read_from_buffer(buff, size, off, module_string, STR_SIZE);
}

static ssize_t test_write(struct file* fd, const char __user* buff, size_t size, loff_t* off)
{
    if (size > STR_SIZE)
        return -EINVAL;

    return simple_write_to_buffer(module_string, STR_SIZE, off, buff, size);
}

static struct file_operations fops = {
        .owner = THIS_MODULE,
        .read = test_read,
        .write = test_write
};

static ssize_t proc_test_read(struct file* fd, char __user* buff, size_t size, loff_t* off)
{
    return simple_read_from_buffer(buff, size, off, module_string, STR_SIZE);
}

static ssize_t proc_test_write(struct file* fd, const char __user* buff, size_t size, loff_t* off)
{
    if (size > STR_SIZE)
        return -EINVAL;

    return simple_write_to_buffer(module_string, STR_SIZE, off, buff, size);
}

static const struct proc_ops pops = {
        .proc_read = proc_test_read,
        .proc_write = proc_test_write
};

static ssize_t string_show(struct kobject* kobj, struct kobj_attribute* attr, char* buff)
{
    memcpy(buff, module_string, STR_SIZE);
    return strlen(module_string);
}

static ssize_t string_store(struct kobject* kobj, struct kobj_attribute* attr, const char* buff, size_t count)
{
    if (count > STR_SIZE)
        return -EINVAL;

    memcpy(module_string, buff, count);
    return count;
}

static struct kobj_attribute string_attribute = __ATTR(module_string, 0664, string_show, string_store);

static struct attribute* attrs[] = { &string_attribute.attr, NULL };

static struct attribute_group attr_group = { .attrs = attrs };

static int __init kmod_init(void)
{
    int retval;

    pr_info("Test module is loaded!\n");

    major = register_chrdev(0, CHRDEV_NAME, &fops);
    if (major < 0) {
        pr_err("Failed to register %s\n", CHRDEV_NAME);
        return major;
    }
    pr_info("Major number for module is %d.\n", major);

    test_proc = proc_create(PROC_NAME, 0666, NULL, &pops);
    if (!test_proc) {
        unregister_chrdev(major, CHRDEV_NAME);
        pr_err("Failed to initialize /proc/%s\n", PROC_NAME);
        return -ENOMEM;
    }

    test_kobj_sysfs = kobject_create_and_add(SYS_NAME, kernel_kobj);
    if (!test_kobj_sysfs) {
        remove_proc_entry(PROC_NAME, NULL);
        unregister_chrdev(major, CHRDEV_NAME);
        return -ENOMEM;
    }

    retval = sysfs_create_group(test_kobj_sysfs, &attr_group);
    if (retval) {
        kobject_put(test_kobj_sysfs);
        remove_proc_entry(PROC_NAME, NULL);
        unregister_chrdev(major, CHRDEV_NAME);
        pr_err("Failed to create sysfs group /sys/kernel/%s\n", SYS_NAME);
        return retval;
    }

    return 0;
}

static void __exit kmod_exit(void)
{
    sysfs_remove_group(test_kobj_sysfs, &attr_group);
    kobject_put(test_kobj_sysfs);
    remove_proc_entry(PROC_NAME, NULL);
    unregister_chrdev(major, CHRDEV_NAME);
    pr_info("Test module is unloaded!\n");
}

module_init(kmod_init);
module_exit(kmod_exit);

MODULE_LICENSE("GPL");
