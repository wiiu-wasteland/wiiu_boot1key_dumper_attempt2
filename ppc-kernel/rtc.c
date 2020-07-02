#include "rtc.h"
#include "io.h"

// Write RTC register
void rtc_write(u32 reg, u32 data) {
	// Write register offset
	out_be32(EXI0_CSR, RTC_EXICSR);
	out_be32(EXI0_DATA, reg | 0x80000000);
	out_be32(EXI0_CR, RTC_EXICR_W);
	while(!(in_be32(EXI0_CSR) & EXICSR_INTSET));
	
	// Write data
	out_be32(EXI0_CSR, RTC_EXICSR);
	out_be32(EXI0_DATA, data);
	out_be32(EXI0_CR, RTC_EXICR_W);
	while(!(in_be32(EXI0_CSR) & EXICSR_INTSET));
	
	// Clear channel parameters
	out_be32(EXI0_CSR, 0);
}

// Read RTC register
u32 rtc_read(u32 reg) {
	u32 data;
	
	// Write register offset
	out_be32(EXI0_CSR, RTC_EXICSR);
	out_be32(EXI0_DATA, reg);
	out_be32(EXI0_CR, RTC_EXICR_W);
	while(!(in_be32(EXI0_CSR) & EXICSR_INTSET));
	
	// Read data
	out_be32(EXI0_CSR, RTC_EXICSR);
	out_be32(EXI0_CR, RTC_EXICR_R);
	while(!(in_be32(EXI0_CSR) & EXICSR_INTSET));
	data = in_be32(EXI0_DATA);
	
	// Clear channel parameters
	out_be32(EXI0_CSR, 0);
	
	return data;
}

// Initialize EXI
void exi_init(void) {
	u32 aip_prot_val = in_be32(HW_AIP_PROT) | 1;
	out_be32(HW_AIP_PROT, aip_prot_val);
}
