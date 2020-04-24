
/* auto-generated by gen_syscalls.py, don't edit */
#ifndef Z_INCLUDE_SYSCALLS_ENTROPY_H
#define Z_INCLUDE_SYSCALLS_ENTROPY_H


#ifndef _ASMLANGUAGE

#include <syscall_list.h>
#include <syscall_macros.h>

#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#pragma GCC diagnostic push
#endif

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern int z_impl_entropy_get_entropy(struct device * dev, u8_t * buffer, u16_t length);
static inline int entropy_get_entropy(struct device * dev, u8_t * buffer, u16_t length)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (int) z_arch_syscall_invoke3(*(u32_t *)&dev, *(u32_t *)&buffer, *(u32_t *)&length, K_SYSCALL_ENTROPY_GET_ENTROPY);
	}
#endif
	compiler_barrier();
	return z_impl_entropy_get_entropy(dev, buffer, length);
}


#ifdef __cplusplus
}
#endif

#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#pragma GCC diagnostic pop
#endif

#endif
#endif /* include guard */
