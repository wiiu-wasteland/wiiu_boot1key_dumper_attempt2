/*
 * Global definition of all the bootwrapper operations.
 *
 * Author: Mark A. Greer <mgreer@mvista.com>
 *
 * 2006 (c) MontaVista Software, Inc.  This file is licensed under
 * the terms of the GNU General Public License version 2.  This program
 * is licensed "as is" without any warranty of any kind, whether express
 * or implied.
 */
#ifndef _PPC_BOOT_OPS_H_
#define _PPC_BOOT_OPS_H_

#include <stddef.h>
#include "types.h"
#include "string.h"

void start(void);
void *alloc_init(char *base, unsigned long heap_size,
			unsigned long granularity, unsigned long max_allocs);
extern void flush_cache(void *, unsigned long);

void console_write(const char *buf, int len);

void *realloc(void *ptr, unsigned long size);
void *malloc(unsigned long size);
void free(void *ptr);

static inline void exit(void)
{
	for(;;);
}
#define fatal(args...) { printf(args); exit(); }


#define BSS_STACK(size) \
	static char _bss_stack[size]; \
	void *_platform_stack_top = _bss_stack + sizeof(_bss_stack);

extern unsigned long timebase_period_ns;
void udelay(long delay);

extern char _start[];
extern char __bss_start[];
extern char _end[];

#endif /* _PPC_BOOT_OPS_H_ */
