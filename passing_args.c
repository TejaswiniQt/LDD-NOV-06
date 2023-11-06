include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/moduleparam.h>

int value;
int arr[5];
char *name;
int cb_value = 0;

module_param(value, int, S_IRUSR | S_IWUSR);
module_param(name, charp, S_IRUSR | S_IWUSR);
module_param_array(arr, int, NULL, S_IRUSR | S_IWUSR);


/* module_param_cb */
int notify_param(const char *val, const struct kernel_param *kp)
{
        int res = param_set_int(val, kp); // Use helper for write variable
        if(res==0) {
                printk(KERN_INFO "Call back function called...\n");
                printk(KERN_INFO "New value of cb_valueETX = %d\n", cb_value);
                return 0;
        }
        return -1;
}

const struct kernel_param_ops my_param_ops =
{
        .set = &notify_param, // Use our setter ...
        .get = &param_get_int, // .. and standard getter
};

module_param_cb(cb_value, &my_param_ops, &cb_value, S_IRUGO|S_IWUSR );

/* Module init function */
static int __init passing_args_init(void)
{
       int  i;
        printk(KERN_INFO "value = %d\n",value);
        printk(KERN_INFO "name = %s\n",name);
        printk(KERN_INFO "cb_value = %d\n",cb_value);
        printk(KERN_INFO "array values are :\n");
        for(i=0; i<(sizeof(arr)/sizeof(int)); i++)
                printk(KERN_INFO "arr[%d] = %d\n",i,arr[i]);
        printk(KERN_INFO "Kernel module inserted successfully\n");
        return 0;
}

/*  Module exit function */
void __exit passing_args_exit(void)
{
        printk(KERN_INFO "Module removed successfully\n");
}

module_init(passing_args_init);
module_exit(passing_args_exit);

MODULE_LICENSE("GPL");
