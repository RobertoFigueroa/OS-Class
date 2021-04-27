#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>


/*This function is called when the module is loaded*/
int simple_init(void) {
    printk(KERN_INFO "Loading Module\nHola");
    return 0;
}

/*This function si called when the module is removed*/
void simple_exit(void) {
        printk(KERN_INFO "Removing Module\nAdios");
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("ESTE ES EL MOD. DESC.");
MODULE_AUTHOR("ROBERTO FIGUEROA");