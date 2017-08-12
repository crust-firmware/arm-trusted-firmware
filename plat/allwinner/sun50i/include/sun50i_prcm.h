/*
 * Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __SUN50I_PRCM_H__
#define __SUN50I_PRCM_H__

#include <sun50i_mmap.h>

/* c = cluster, n = core */
#define SUN50I_R_PRCM_CPUS_CFG_REG		(SUN50I_R_PRCM_BASE + 0x0000)
#define SUN50I_R_PRCM_PWROFF_GATING_REG(c)	(SUN50I_R_PRCM_BASE + 0x0100 + (c) * 0x04)
#define SUN50I_R_PRCM_CPU_PWR_CLAMP_REG(c, n)	(SUN50I_R_PRCM_BASE + 0x0140 + (c) * 0x10 + (n) * 0x04)

#endif /* __SUN50I_PRCM_H__ */
