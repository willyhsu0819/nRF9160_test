/* auto-generated by gen_syscalls.py, don't edit */
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#pragma GCC diagnostic push
#endif
#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif
#include <syscalls/can.h>

extern void z_vrfy_can_detach(struct device * dev, int filter_id);
u32_t z_mrsh_can_detach(u32_t arg0, u32_t arg1, u32_t arg2,
		u32_t arg3, u32_t arg4, u32_t arg5, void *ssf)
{
	_current_cpu->syscall_frame = ssf;
	(void) arg2;	/* unused */
	(void) arg3;	/* unused */
	(void) arg4;	/* unused */
	(void) arg5;	/* unused */
	z_vrfy_can_detach(*(struct device **)&arg0, *(int*)&arg1)
;
	return 0;
}

#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#pragma GCC diagnostic pop
#endif
