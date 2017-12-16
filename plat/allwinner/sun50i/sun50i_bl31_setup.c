/*
 * Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <debug.h>
#include <generic_delay_timer.h>
#include <gicv2.h>
#include <mmio.h>
#include <platform.h>
#include <platform_def.h>
#include <sun50i_def.h>
#include <sun50i_mmap.h>
#include <uart_16550.h>

#include "sun50i_private.h"

static entry_point_info_t bl32_image_ep_info;
static entry_point_info_t bl33_image_ep_info;

static console_16550_t console;

static const gicv2_driver_data_t sun50i_gic_data = {
	.gicd_base = SUN50I_GICD_BASE,
	.gicc_base = SUN50I_GICC_BASE,
};

void bl31_early_platform_setup(bl31_params_t *from_bl2,
			       void *plat_params_from_bl2)
{
	/* Initialize the debug console as soon as possible */
	console_16550_register(SUN50I_UART0_BASE, SUN50I_UART0_CLK_IN_HZ,
			       SUN50I_UART0_BAUDRATE, &console);

	/* Validate parameters passed from BL2 */
	assert(from_bl2->h.type == PARAM_BL31);
	assert(from_bl2->h.version >= VERSION_1);

	/* Copy BL32 (if populated) and BL33 entry point information */
	if (from_bl2->bl32_ep_info)
		bl32_image_ep_info = *from_bl2->bl32_ep_info;
	bl33_image_ep_info = *from_bl2->bl33_ep_info;
}

void bl31_plat_arch_setup(void)
{
	sun50i_configure_mmu_el3(0);
}

void bl31_platform_setup(void)
{
	generic_delay_timer_init();

	/* Configure the interrupt controller */
	gicv2_driver_init(&sun50i_gic_data);
	gicv2_distif_init();
	gicv2_pcpu_distif_init();
	gicv2_cpuif_enable();

	if (mmio_read_32(0x40100)) {
		NOTICE("ARISC firmware found, deasserting reset\n");
		mmio_setbits_32(SUN50I_R_CPUCFG_BASE, BIT(0));
	} else {
		WARN("ARISC firmware not found\n");
	}

	INFO("BL31: Platform setup done\n");
}

entry_point_info_t *bl31_plat_get_next_image_ep_info(uint32_t type)
{
	entry_point_info_t *next_image_info;

	assert(sec_state_is_valid(type));
	next_image_info = (type == NON_SECURE)
			? &bl33_image_ep_info : &bl32_image_ep_info;

	/* Address 0 is not a valid entry point on this platform */
	if (next_image_info->pc)
		return next_image_info;

	return NULL;
}
