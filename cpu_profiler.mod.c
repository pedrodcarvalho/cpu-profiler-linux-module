#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x30ff7695, "module_layout" },
	{ 0x8e3e5524, "single_release" },
	{ 0xa5457562, "seq_lseek" },
	{ 0x83599202, "seq_read" },
	{ 0xa81d3524, "remove_proc_entry" },
	{ 0x8c03d20c, "destroy_workqueue" },
	{ 0x42160169, "flush_workqueue" },
	{ 0xdc0e4855, "timer_delete" },
	{ 0x49cd25ed, "alloc_workqueue" },
	{ 0xc6f46339, "init_timer_key" },
	{ 0xb4e4d6a9, "proc_create" },
	{ 0xf6897c7, "proc_mkdir" },
	{ 0x37a0cba, "kfree" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x4f00afd3, "kmem_cache_alloc_trace" },
	{ 0xac1c4313, "kmalloc_caches" },
	{ 0x2d5f69b3, "rcu_read_unlock_strict" },
	{ 0x37befc70, "jiffies_to_msecs" },
	{ 0x6d294e43, "clock_t_to_jiffies" },
	{ 0xa42810ad, "pid_task" },
	{ 0xb2e53b3e, "find_vpid" },
	{ 0xa68165d, "seq_printf" },
	{ 0xba8fbd64, "_raw_spin_lock" },
	{ 0x33a21a09, "pv_ops" },
	{ 0xc38c83b8, "mod_timer" },
	{ 0x15ba50a6, "jiffies" },
	{ 0xc5b6f236, "queue_work_on" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x801e8735, "single_open" },
	{ 0x92997ed8, "_printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "2E77168DA3CB01A41AE98DC");
