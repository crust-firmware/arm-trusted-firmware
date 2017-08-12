/*
 * Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __SUN50I_PRIVATE_H__
#define __SUN50I_PRIVATE_H__

void sun50i_configure_mmu_el1(int flags);
void sun50i_configure_mmu_el3(int flags);
void sun50i_cpu_off(unsigned int cluster, unsigned int core);
void sun50i_cpu_on(unsigned int cluster, unsigned int core);

#endif /* __SUN50I_PRIVATE_H__ */
