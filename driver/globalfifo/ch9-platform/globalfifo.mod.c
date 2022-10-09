#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xe00b4984, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x3e83f3a, __VMLINUX_SYMBOL_STR(platform_driver_unregister) },
	{ 0x5f944725, __VMLINUX_SYMBOL_STR(__platform_driver_register) },
	{ 0x5eae5d6b, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0x68dfc59f, __VMLINUX_SYMBOL_STR(__init_waitqueue_head) },
	{ 0xa22311d0, __VMLINUX_SYMBOL_STR(__mutex_init) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
	{ 0xd8e484f0, __VMLINUX_SYMBOL_STR(register_chrdev_region) },
	{ 0xdd353f7f, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0x64e48f86, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0x20f4fdce, __VMLINUX_SYMBOL_STR(kill_fasync) },
	{ 0x3a013b7d, __VMLINUX_SYMBOL_STR(remove_wait_queue) },
	{ 0xe45f60d8, __VMLINUX_SYMBOL_STR(__wake_up) },
	{ 0x2e60bace, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0x4292364c, __VMLINUX_SYMBOL_STR(schedule) },
	{ 0xd7bd3af2, __VMLINUX_SYMBOL_STR(add_wait_queue) },
	{ 0xffd5a395, __VMLINUX_SYMBOL_STR(default_wake_function) },
	{ 0x14ee158f, __VMLINUX_SYMBOL_STR(current_task) },
	{ 0x50eedeb8, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x728d8849, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0xfe57f7e2, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0xe24abfc5, __VMLINUX_SYMBOL_STR(fasync_helper) },
	{ 0xb4390f9a, __VMLINUX_SYMBOL_STR(mcount) },
	{ 0xb6b46a7c, __VMLINUX_SYMBOL_STR(param_ops_int) },
	{ 0x68dfc1d4, __VMLINUX_SYMBOL_STR(devm_kmalloc) },
	{ 0x4f8b5ddb, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0x4f6b400b, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0x1e047854, __VMLINUX_SYMBOL_STR(warn_slowpath_fmt) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "E7D3AC29CA9610A94FA2DB6");
