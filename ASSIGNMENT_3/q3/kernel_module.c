#include <linux/module.h>     // included for all kernel modules
#include <linux/kernel.h>     // included for KERN_INFO
#include <linux/init.h>       // included for __init and __exit macros
#include <asm/threadinfo.h>
#include <linux/sched.h>
#include <linux/cred.h>
#include <kernel/sys.c>
#include <unistd.h>
#include <kernel/pid.c>
#include <linux/rcupdate.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MANAS");
MODULE_DESCRIPTION("Task Struct Reading Module"); 

static int __init hello_init(pid_t PID){
	long pid,uid,pgid;
	pid=(long)PID;
	struct task_struct task;
	task=pid_task(find_vpid(PID),PIDTYPE_PID);
	uid=(long)task->cred->uid;
	pgid=(long)pid_vnr(task_pgrp(task));

	printk(KERN_INFO "pid=%l\n",pid);
	printk(KERN_INFO "user_id=%l\n",uid);
	printk(KERN_INFO "pgid=%l\n",pgid);
	return 0;
}



static void __exit hello_cleanup(void){
	printk(KERN_INFO "Cleaning up module.\n");
}
module_init(hello_init);      
module_exit(hello_cleanup);   
