#
# Copyright (c) 2013-2017, ARM Limited and Contributors. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

include lib/xlat_tables_v2/xlat_tables.mk

PLAT_INCLUDES		:=	-Iinclude/drivers/ti/uart			\
				-Iplat/allwinner/${PLAT}/${ARCH}/include	\
				-Iplat/allwinner/${PLAT}/include

PLAT_BL_COMMON_SOURCES	:=	drivers/console/${ARCH}/console.S		\
				drivers/ti/uart/${ARCH}/16550_console.S		\
				${XLAT_TABLES_LIB_SRCS}				\
				plat/allwinner/${PLAT}/${ARCH}/plat_helpers.S	\
				plat/allwinner/${PLAT}/${PLAT}_common.c

BL1_SOURCES		+=	lib/cpus/${ARCH}/cortex_a53.S

BL2_SOURCES		+=

BL2U_SOURCES		+=

BL31_SOURCES		+=	drivers/arm/gic/common/gic_common.c		\
				drivers/arm/gic/v2/gicv2_helpers.c		\
				drivers/arm/gic/v2/gicv2_main.c			\
				drivers/delay_timer/delay_timer.c		\
				drivers/delay_timer/generic_delay_timer.c	\
				lib/cpus/${ARCH}/cortex_a53.S			\
				plat/common/plat_gicv2.c			\
				plat/common/plat_psci_common.c			\
				plat/allwinner/${PLAT}/${PLAT}_bl31_setup.c	\
				plat/allwinner/${PLAT}/${PLAT}_pm.c		\
				plat/allwinner/${PLAT}/${PLAT}_topology.c

# Enable memory map optimisation.
ARM_BOARD_OPTIMISE_MEM		:=	1

# The bootloader is guaranteed to only run on CPU 0 by the boot ROM.
COLD_BOOT_SINGLE_CPU		:=	1

# Enable workarounds for Cortex-A53 errata.
ERRATA_A53_826319               :=      1
ERRATA_A53_835769               :=      1
ERRATA_A53_836870               :=      1
ERRATA_A53_843419               :=      1
ERRATA_A53_855873               :=      1

# Disable the PSCI platform compatibility layer.
ENABLE_PLAT_COMPAT		:=	0

# Prohibit using deprecated interfaces.
ERROR_DEPRECATED		:=	1

# The reset vector can be changed for each CPU.
PROGRAMMABLE_RESET_ADDRESS	:=	1

# Allow mapping read-only data as execute-never.
SEPARATE_CODE_AND_RODATA	:=	1
