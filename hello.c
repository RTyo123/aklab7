#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/list.h>

struct head_list {
	struct list_head next;
	ktime_t time;
};

MODULE_AUTHOR("Sirokomskyi M <warface123roadster@gmail.com>");
MODULE_DESCRIPTION("Module aklab6");
MODULE_LICENSE("Dual BSD/GPL");

static LIST_HEAD(head);

static uint counter = 1;
module_param(counter, uint, 0444);
MODULE_PARM_DESC(counter, "This parameter is for number of hello world");

static int __init hello_init(void)
{
	uint i = 0;

	struct head_list *var;

	pr_info("Input %d\n", counter);

	if (!counter || (counter >= 5 && counter <= 10))
		pr_warn("WARNING!\tYou should enter number less than 5 and more than 0");

	BUG_ON(counter > 10);

	for (i = 0; i < counter; i++) {
		var = kmalloc(sizeof(struct head_list), GFP_KERNEL);

		if (i == 4)
			var = 0;

		if (ZERO_OR_NULL_PTR(var))
			goto err;

		var->time = ktime_get();
		pr_info("Hello, world!\n");

		list_add_tail(&(var->next), &head);
	}

	return 0;
err:
	{
	struct head_list *md, *tmp;

	pr_err("kmalloc is running out of memory");
	list_for_each_entry_safe(md, tmp, &head, next) {
		list_del(&md->next);
		kfree(md);
	}
	BUG();

	return -ENOMEM;
	}
}

static void __exit hello_exit(void)
{
	struct head_list *md, *tmp;

	list_for_each_entry_safe(md, tmp, &head, next) {
		pr_info("Time: %lld", md->time);
		list_del(&md->next);
		kfree(md);
	}
	BUG_ON(!list_empty(&head));
	pr_info("");
}

module_init(hello_init);
module_exit(hello_exit);


