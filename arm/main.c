/*
 *	It's a Project! linux-loader
 *
 *	Copyright (C) 2017          Ash Logan <quarktheawesome@gmail.com>
 *
 *	Based on code from the following contributors:
 *
 *	Copyright (C) 2016          SALT
 *	Copyright (C) 2016          Daz Jones <daz@dazzozo.com>
 *
 *	Copyright (C) 2008, 2009    Haxx Enterprises <bushing@gmail.com>
 *	Copyright (C) 2008, 2009    Sven Peter <svenpeter@gmail.com>
 *	Copyright (C) 2008, 2009    Hector Martin "marcan" <marcan@marcansoft.com>
 *	Copyright (C) 2009          Andre Heider "dhewg" <dhewg@wiibrew.org>
 *	Copyright (C) 2009          John Kelley <wiidev@kelley.ca>
 *
 *	(see https://github.com/Dazzozo/minute)
 *
 *	This code is licensed to you under the terms of the GNU GPL, version 2;
 *	see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 */

#include <stdlib.h>
#include "video/gfx.h"
#include "system/ppc.h"
#include "system/exception.h"
#include "system/memory.h"
#include "system/irq.h"
#include "storage/sd/sdcard.h"
#include "storage/sd/fatfs/elm.h"
#include "storage/isfs.h"
#include "storage/crypto.h"
#include "system/smc.h"
#include "system/latte.h"
#include "common/utils.h"


#define CMD_KILL 0xCAFE0001

#define CMD_MASK 0xFF000000
#define CMD_PRINT 0x01000000

#define LT_IPC_ARMCTRL_COMPAT_X1 0x4
#define LT_IPC_ARMCTRL_COMPAT_Y1 0x1

u32 *magic   = (u32*)0x1000A000;
u32 *ppcdata = (u32*)0x1000A004;

void NORETURN _main(void* base) {
	int res;
	u32 ppc_entry = 0;

	gfx_clear(GFX_ALL, BLACK);
	exception_initialize();
	mem_initialize();
	irq_initialize();
	srand(read32(LT_TIMER));
	crypto_initialize();
	sdcard_init();
	res = ELM_Mount();
	if (res) {
		printf("panic: SD Card mount error: %d\n", res);
		panic(0);
	}
	isfs_init();

	printf("LT_AHBPROT     = %08X\n", read32(LT_AHBPROT));
	printf("LT_GPIO_ENABLE = %08X\n", read32(LT_GPIO_ENABLE));
	printf("LT_GPIO_OWNER  = %08X\n", read32(LT_GPIO_OWNER));

	printf("--------------------------\n");
	printf(" Starting ppc-hax-kernel! \n");
	printf("--------------------------\n");

	// remove hardware access restrictions
	write32(LT_EXICTRL, 0x1F);
	write32(LT_AHBPROT, 0xFFFFFFFF);
	write32(LT_GPIO_ENABLE, 0xFFFFFFFF);
	write32(LT_GPIO_OWNER, 0xFFFFFFFF);

	// load ppc-hax-kernel
	res = ppc_load_file("sdmc:/ppc-hax-kernel.elf", &ppc_entry);
	if (res < 0) {
		printf("panic: Failed to load ppc-hax-kernel.elf: %d\n", res);
		panic(0);
	}
	printf("ppc_entry: %08X\n");

	// jump to the kernel
	ppc_jump(ppc_entry);

	gfx_clear(GFX_ALL, BLACK);

	ELM_Unmount();
	sdcard_exit();
	irq_disable(IRQ_SD0);
	isfs_fini();
	irq_shutdown();
	mem_shutdown();

	for (;;) {
		//check for message sent flag
		u32 ctrl = read32(LT_IPC_ARMCTRL_COMPAT);
		if (!(ctrl & LT_IPC_ARMCTRL_COMPAT_X1)) continue;
		
		//read PowerPC's message
		u32 msg = read32(LT_IPC_PPCMSG_COMPAT);
		
		//process commands
		if ((msg & CMD_MASK) == CMD_PRINT) {
			char buf[4];
			buf[0] = (msg & 0x00FF0000) >> 16;
			buf[1] = (msg & 0x0000FF00) >>  8;
			buf[2] = (msg & 0x000000FF) >>  0;
			buf[3] = '\0';
			ppc_print(buf);
		}
		
		//writeback ctrl value to reset IPC
		write32(LT_IPC_ARMCTRL_COMPAT, ctrl);
	}
}
