/*
 * Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __SUN50I_TIMER_H__
#define __SUN50I_TIMER_H__

#include <sun50i_mmap.h>

#define SUN50I_WDOG0_IRQ_EN_REG		(SUN50I_TIMER_BASE + 0x00a0)
#define SUN50I_WDOG0_IRQ_STA_REG	(SUN50I_TIMER_BASE + 0x00a4)
#define SUN50I_WDOG0_CTRL_REG		(SUN50I_TIMER_BASE + 0x00b0)
#define SUN50I_WDOG0_CFG_REG		(SUN50I_TIMER_BASE + 0x00b4)
#define SUN50I_WDOG0_MODE_REG		(SUN50I_TIMER_BASE + 0x00b8)

#endif /* __SUN50I_TIMER_H__ */
