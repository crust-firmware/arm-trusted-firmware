/*
 * Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch_helpers.h>
#include <assert.h>
#include <debug.h>
#include <delay_timer.h>
#include <mmio.h>
#include <platform.h>
#include <platform_def.h>
#include <psci.h>
#include <sun50i_timer.h>

static void __dead2 sun50i_system_off(void)
{
	ERROR("PSCI: Full shutdown not implemented, halting\n");
	wfi();
	panic();
}

static void __dead2 sun50i_system_reset(void)
{
	/* Reset the whole system when the watchdog times out */
	mmio_write_32(SUN50I_WDOG0_CFG_REG, 1);
	/* Enable the watchdog with the shortest timeout (0.5 seconds) */
	mmio_write_32(SUN50I_WDOG0_MODE_REG, (0 << 4) | 1);
	/* Wait for twice the watchdog timeout before panicking */
	mdelay(1000);

	ERROR("PSCI: System reset failed\n");
	wfi();
	panic();
}

static plat_psci_ops_t sun50i_psci_ops = {
	.system_off			= sun50i_system_off,
	.system_reset			= sun50i_system_reset,
};

int plat_setup_psci_ops(uintptr_t sec_entrypoint,
			const plat_psci_ops_t **psci_ops)
{
	assert(psci_ops);

	*psci_ops = &sun50i_psci_ops;

	return 0;
}
