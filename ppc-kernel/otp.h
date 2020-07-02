#ifndef _OTP_H
#define _OTP_H
#include "types.h"

#define LT_OTPCMD                     (u32*)0xD8001EC
#define LT_OTPDATA                    (u32*)0xD8001F0

u32 otp_read(u32 offset);

#endif
