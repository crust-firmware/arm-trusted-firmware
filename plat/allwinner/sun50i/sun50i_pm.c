/*
 * Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch_helpers.h>
#include <assert.h>
#include <debug.h>
#include <delay_timer.h>
#include <gicv2.h>
#include <mmio.h>
#include <platform.h>
#include <platform_def.h>
#include <psci.h>
#include <sun50i_cpucfg.h>
#include <sun50i_timer.h>

#include "sun50i_private.h"

#define mpidr_is_valid(mpidr) (plat_core_pos_by_mpidr(mpidr) >= 0)

uintptr_t warm_entrypoint;

static int sun50i_pwr_domain_on(u_register_t mpidr)
{
	if (!mpidr_is_valid(mpidr))
		return PSCI_E_INTERN_FAIL;

	mmio_write_32(SUN50I_CPUCFG_RVBAR_LO_REG(MPIDR_AFFLVL0_VAL(mpidr)),
		      warm_entrypoint & 0xffffffff);
	mmio_write_32(SUN50I_CPUCFG_RVBAR_HI_REG(MPIDR_AFFLVL0_VAL(mpidr)),
		      warm_entrypoint >> 32);
	sun50i_cpu_on(MPIDR_AFFLVL1_VAL(mpidr), MPIDR_AFFLVL0_VAL(mpidr));

	return PSCI_E_SUCCESS;
}

static void sun50i_pwr_domain_on_finish(const psci_power_state_t *target_state)
{
	gicv2_pcpu_distif_init();
	gicv2_cpuif_enable();
}

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

static int sun50i_validate_ns_entrypoint(uintptr_t ns_entrypoint)
{
	/* The non-secure entry point must be in DRAM */
	if (ns_entrypoint >= SUN50I_DRAM_BASE &&
	    ns_entrypoint < SUN50I_DRAM_BASE + SUN50I_DRAM_SIZE)
		return PSCI_E_SUCCESS;

	return PSCI_E_INVALID_ADDRESS;
}

static plat_psci_ops_t sun50i_psci_ops = {
	.pwr_domain_on			= sun50i_pwr_domain_on,
	.pwr_domain_on_finish		= sun50i_pwr_domain_on_finish,
	.system_off			= sun50i_system_off,
	.system_reset			= sun50i_system_reset,
	.validate_ns_entrypoint		= sun50i_validate_ns_entrypoint,
};

int plat_setup_psci_ops(uintptr_t sec_entrypoint,
			const plat_psci_ops_t **psci_ops)
{
	assert(psci_ops);

	warm_entrypoint = sec_entrypoint;
	*psci_ops = &sun50i_psci_ops;

	return 0;
}
