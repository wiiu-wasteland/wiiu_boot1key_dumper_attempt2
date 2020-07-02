/*
 * console.c
 * Copyright (C) 2018 Ash Logan <quarktheawesome@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 */

#include <stddef.h>
#include "string.h"
#include "stdio.h"
#include "types.h"
#include "io.h"
#include "ops.h"

#define LT_IPC_PPCMSG  (volatile u32 volatile*)0x0d800000
#define LT_IPC_PPCCTRL (volatile u32 volatile*)0x0d800004

#define WIIU_LOADER_CMD_PRINT 0x01000000

void console_write(const char *buf, int len) {
    int i = 0;
    for (i = 0; i < len; i += 3) {
        *LT_IPC_PPCMSG = WIIU_LOADER_CMD_PRINT
            | (buf[i + 0] << 16) | (buf[i + 1] << 8) | buf[i + 2];

        *LT_IPC_PPCCTRL = 1;

        while (*LT_IPC_PPCCTRL & 1);
    }
    if (i < len) {
        for (; i < len; i++) {
            *LT_IPC_PPCMSG = WIIU_LOADER_CMD_PRINT
                | (buf[i] << 16);

            *LT_IPC_PPCCTRL = 1;

            while (*LT_IPC_PPCCTRL & 1);
        }
    }
}
