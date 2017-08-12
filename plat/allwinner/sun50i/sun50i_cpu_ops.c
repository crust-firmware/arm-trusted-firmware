/*
 * Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <debug.h>
#include <mmio.h>
#include <platform_def.h>
#include <sun50i_cpucfg.h>
#include <sun50i_prcm.h>
#include <utils_def.h>

#include "sun50i_private.h"

static void sun50i_cpu_disable_power(unsigned int cluster, unsigned int core)
{
	if (mmio_read_32(SUN50I_R_PRCM_CPU_PWR_CLAMP_REG(cluster, core)) == 0xff)
		return;

	mmio_write_32(SUN50I_R_PRCM_CPU_PWR_CLAMP_REG(cluster, core), 0xff);
}

static void sun50i_cpu_enable_power(unsigned int cluster, unsigned int core)
{
	if (mmio_read_32(SUN50I_R_PRCM_CPU_PWR_CLAMP_REG(cluster, core)) == 0)
		return;

	/* Power enable sequence from original Allwinner sources */
	mmio_write_32(SUN50I_R_PRCM_CPU_PWR_CLAMP_REG(cluster, core), 0xfe);
	mmio_write_32(SUN50I_R_PRCM_CPU_PWR_CLAMP_REG(cluster, core), 0xf8);
	mmio_write_32(SUN50I_R_PRCM_CPU_PWR_CLAMP_REG(cluster, core), 0xe0);
	mmio_write_32(SUN50I_R_PRCM_CPU_PWR_CLAMP_REG(cluster, core), 0x80);
	mmio_write_32(SUN50I_R_PRCM_CPU_PWR_CLAMP_REG(cluster, core), 0x00);
}

void sun50i_cpu_off(unsigned int cluster, unsigned int core)
{
	/* Deassert DBGPWRDUP */
	mmio_clrbits_32(SUN50I_CPUCFG_DBG_REG0, BIT(core));
	/* Activate the core output clamps */
	mmio_setbits_32(SUN50I_R_PRCM_PWROFF_GATING_REG(cluster), BIT(core));
	/* Assert CPU power-on reset */
	mmio_clrbits_32(SUN50I_R_CPUCFG_PWRONRST_REG(cluster), BIT(core));
	/* Remove power from the CPU */
	sun50i_cpu_disable_power(cluster, core);
}

void sun50i_cpu_on(unsigned int cluster, unsigned int core)
{
	/* Assert CPU core reset */
	mmio_clrbits_32(SUN50I_CPUCFG_RST_CTRL_REG(cluster), BIT(core));
	/* Assert CPU power-on reset */
	mmio_clrbits_32(SUN50I_R_CPUCFG_PWRONRST_REG(cluster), BIT(core));
	/* Set CPU to start in AArch64 mode */
	mmio_setbits_32(SUN50I_CPUCFG_CLS_CTRL_REG0(cluster), BIT(24 + core));
	/* Apply power to the CPU */
	sun50i_cpu_enable_power(cluster, core);
	/* Release the core output clamps */
	mmio_clrbits_32(SUN50I_R_PRCM_PWROFF_GATING_REG(cluster), BIT(core));
	/* Deassert CPU power-on reset */
	mmio_setbits_32(SUN50I_R_CPUCFG_PWRONRST_REG(cluster), BIT(core));
	/* Deassert CPU core reset */
	mmio_setbits_32(SUN50I_CPUCFG_RST_CTRL_REG(cluster), BIT(core));
	/* Assert DBGPWRDUP */
	mmio_setbits_32(SUN50I_CPUCFG_DBG_REG0, BIT(core));
}
