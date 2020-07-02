#include <stdarg.h>
#include <stddef.h>
#include "string.h"
#include "stdio.h"
#include "ops.h"
#include "reg.h"
#include "io.h"
#include "rtc.h"
#include "otp.h"

BSS_STACK(8192);

#define LT_GPIOE_OUT   (volatile u32 volatile*)0x0d8000c0
#define LT_GPIOE_DIR   (volatile u32 volatile*)0x0d8000c4

#define LT_IPC_PPCMSG  (volatile u32 volatile*)0x0d800000
#define LT_IPC_PPCCTRL (volatile u32 volatile*)0x0d800004
#define LT_RESETS      (volatile u32 volatile*)0x0d8005e0
#define HW_SRNPROT     (volatile u32 volatile*)0x0d800060

#define RSTB_AHB        (1 << 24) // ARM AHB reset
#define RSTB_IOP        (1 << 23) // IOP/Starbuck reset
#define RSTB_DSP        (1 << 22) // DSP reset
#define RSTB_IOPI       (1 << 19) // Processor Interface IO reset

#define RSTB_IOSI       (1 << 15) // SI reset
#define RSTB_AI_I2S3    (1 << 14) // Audio interface I2S3 reset

#define SRSTB_CPU       (1 << 5) // PowerPC SRESET
#define RSTB_CPU        (1 << 4) // PowerPC HRESET
#define RSTBINB         (1 << 0) // System reset


// sram:0: init
// sram:1: bit
// sram:2: survived
// sram:3: key

void start(void)
{
	u32 prev_bit, current_bit;
	
	u32 heapsize = 32*1024*1024 - (u32)_end;
	alloc_init(_end, heapsize, 32, 64);
	exi_init();
	
	printf("Hello from ppc-hax-kernel!\n");
	printf("[loaded at 0x%p (sp: 0x%p)]\n", _start, get_sp());
	
	// check if we survived previous attempt
	if (sram_read(2) == 1) {
		printf("Survived LT_RESET\n");
		
		if (sram_read(3) != 0) {
			// got the key!
			printf("Got the key! %08X\n", sram_read(3));
			while(1);
		}
	} else {
		printf("Did not survive LT_RESET\n");
	}
	
	u32 srnprot = in_be32(HW_SRNPROT);
	printf("HW_SRNPROT: %08X\n", srnprot);
	out_be32(HW_SRNPROT, srnprot & ~(1 << 6));
	printf("HW_SRNPROT: %08X\n", in_be32(HW_SRNPROT));

	udelay(1000000);
	
	// clear key and survived
	sram_write(2, 0);
	sram_write(3, 0);

	// reset current bit
	out_be32(LT_RESETS, ~(1 << current_bit));
	eieio();
	out_be32(LT_RESETS, ~0);
	eieio();

	exi_init();

	// attempt to save the key
	//sram_write(3, otp_read(0x3A0));

	// notify our survival
	sram_write(2, 1);
	
	// do the actual reset
	out_be32(LT_RESETS, ~RSTBINB);
	eieio();
	while(1);
}
