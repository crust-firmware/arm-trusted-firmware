/*
 * Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <platform.h>
#include <platform_def.h>
#include <sun50i_def.h>
#include <xlat_tables_v2.h>

static mmap_region_t sun50i_mmap[PLATFORM_MMAP_REGIONS + 1] = {
	MAP_REGION_FLAT(SUN50I_ROM_BASE, SUN50I_ROM_SIZE,
			MT_MEMORY | MT_RO | MT_SECURE),
	MAP_REGION_FLAT(SUN50I_SRAM_BASE, SUN50I_SRAM_SIZE,
			MT_MEMORY | MT_RW | MT_SECURE),
	MAP_REGION_FLAT(SUN50I_DEV_BASE, SUN50I_DEV_SIZE + SUN50I_R_DEV_SIZE,
			MT_DEVICE | MT_RW | MT_SECURE),
	MAP_REGION_FLAT(SUN50I_DRAM_BASE, SUN50I_DRAM_SIZE,
			MT_MEMORY | MT_RW | MT_NS),
	{},
};

unsigned int plat_get_syscnt_freq2(void)
{
	return SUN50I_OSC24M_CLK_IN_HZ;
}

#define DEFINE_CONFIGURE_MMU_EL(_el)					\
void sun50i_configure_mmu_el##_el(int flags)				\
{									\
	mmap_add_region(BL31_BASE, BL31_BASE,				\
			BL31_LIMIT - BL31_BASE,				\
			MT_MEMORY | MT_RW | MT_SECURE);			\
	mmap_add_region(BL_CODE_BASE, BL_CODE_BASE,			\
			BL_CODE_END - BL_CODE_BASE,			\
			MT_CODE | MT_SECURE);				\
	mmap_add_region(BL_RO_DATA_BASE, BL_RO_DATA_BASE,		\
			BL_RO_DATA_END - BL_RO_DATA_BASE,		\
			MT_RO_DATA | MT_SECURE);			\
	mmap_add_region(BL_COHERENT_RAM_BASE, BL_COHERENT_RAM_BASE,	\
			BL_COHERENT_RAM_END - BL_COHERENT_RAM_BASE,	\
			MT_DEVICE | MT_RW | MT_SECURE);			\
	mmap_add(sun50i_mmap);						\
	init_xlat_tables();						\
									\
	enable_mmu_el##_el(0);						\
}

/* Define EL1 and EL3 variants of the function initialising the MMU */
DEFINE_CONFIGURE_MMU_EL(1)
DEFINE_CONFIGURE_MMU_EL(3)
