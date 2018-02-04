/*
 * Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch.h>
#include <asm_macros.S>

	.globl	plat_my_core_pos
	.globl	plat_report_exception

func plat_my_core_pos
	mrs	x0, mpidr_el1
	and	x1, x0, #MPIDR_CLUSTER_MASK
	and	x0, x0, #MPIDR_CPU_MASK
	add	x0, x0, x1, LSR #6
	ret
endfunc plat_my_core_pos

func plat_report_exception
	ret
endfunc plat_report_exception
