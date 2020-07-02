#ifndef __RTC_H
#define __RTC_H

#include "types.h"

// EXI registers
#define HW_AIP_PROT		(u32*)0x0d800070
#define EXI0_CSR		(u32*)0x0d806800
#define EXI0_CR			(u32*)0x0d80680c
#define EXI0_DATA		(u32*)0x0d806810

// EXI registers usage
#define EXICSR_DEV		0x380
	#define EXICSR_DEV1			0x100
#define EXICSR_CLK		0x070
	#define EXICSR_CLK_1MHZ		0x000
	#define EXICSR_CLK_8MHZ		0x030
#define EXICSR_INT		0x008
	#define EXICSR_INTSET		0x008
    
#define EXICR_TSTART	0x001
#define EXICR_TRSMODE	0x002
	#define EXICR_TRSMODE_IMM	0x000
#define EXICR_TRSTYPE	0x00C
	#define EXICR_TRSTYPE_R		0x000
	#define EXICR_TRSTYPE_W		0x004
#define EXICR_TLEN		0x030
	#define EXICR_TLEN32		0x030

// EXI registers values to access the RTC
#define RTC_EXICSR	(EXICSR_DEV1 | EXICSR_CLK_8MHZ | EXICSR_INTSET)
#define RTC_EXICR_W	(EXICR_TSTART | EXICR_TRSMODE_IMM | EXICR_TRSTYPE_W | EXICR_TLEN32)
#define RTC_EXICR_R	(EXICR_TSTART | EXICR_TRSMODE_IMM | EXICR_TRSTYPE_R | EXICR_TLEN32)

// RTC registers
#define WIIU_RTC_COUNTER	0x20000000
#define WIIU_RTC_SRAM		0x20000100
	#define WIIU_RTC_SRAM_BIAS	0x2000010C
#define WIIU_RTC_SNAPSHOT	0x20400000
#define WIIU_RTC_ONTMR		0x21000000
#define WIIU_RTC_OFFTMR		0x21000100
#define WIIU_RTC_CONTROL0	0x21000C00
#define WIIU_RTC_CONTROL1	0x21000D00

// Write RTC register
void rtc_write(u32 reg, u32 data);

// Read RTC register
u32 rtc_read(u32 reg);

// Initialize EXI
void exi_init(void);


static inline void sram_write(u32 i, u32 v) {
	rtc_write(WIIU_RTC_SRAM + 0x100 * i, v);
}

static inline u32 sram_read(u32 i) {
	return rtc_read(WIIU_RTC_SRAM + 0x100 * i);
}

#endif
