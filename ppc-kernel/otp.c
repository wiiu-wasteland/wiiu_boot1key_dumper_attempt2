#include "otp.h"
#include "io.h"

u32 otp_read(u32 offset)
{
	u32 word = (offset / 4) & 0x1f;
	u32 bank = (offset / 4) >> 5;
	
	out_be32(LT_OTPCMD, 0x80000000 | bank << 8 | word);
	eieio();

	return in_be32(LT_OTPDATA);
} 
