#include <linux/module.h>
#include <linux/timer.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <linux/ioport.h>

struct timer_list mytimer;
int ticks;

#define KBD_CONTROL_REG        0x64

void mytimer_function(struct timer_list *ptimer)
{
	printk(KERN_INFO " %s : mytimer_function \n", THIS_MODULE->name);
	
	while (inb(KBD_CONTROL_REG) & 0x02)
		mdelay(1);
	outb(0xAE, KBD_CONTROL_REG);
	mod_timer(&mytimer, jiffies + ticks);

	printk(KERN_INFO " %s : keyboard enabled\n", THIS_MODULE->name);
}

static __init int desd_init(void)
{
	int sec = 10;
	ticks = sec * HZ;		//750

	printk(KERN_INFO " %s : Timer initialisation\n", THIS_MODULE->name);
	timer_setup(&mytimer, mytimer_function, 0);
	mytimer.expires = jiffies + ticks;
	add_timer(&mytimer);
	
	while (inb(KBD_CONTROL_REG) & 0x02)
		mdelay(1);
	outb(0xAD, KBD_CONTROL_REG);
	printk(KERN_INFO " %s : keyboard disabled\n", THIS_MODULE->name);

	printk(KERN_INFO " %s : Timer initialisation is done successfully\n", THIS_MODULE->name);
	return 0;
}

static __exit void desd_exit(void)
{
	del_timer_sync(&mytimer);
	printk(KERN_INFO " %s : Timer deinitialisation is done successfully\n", THIS_MODULE->name);
}

module_init(desd_init);
module_exit(desd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DESD @ Sunbeam");
MODULE_DESCRIPTION("This is demo of kernel timers");














