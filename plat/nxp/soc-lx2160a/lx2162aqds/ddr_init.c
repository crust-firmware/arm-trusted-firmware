/*
 * Copyright 2018-2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <common/debug.h>
#include <ddr.h>
#include <lib/utils.h>
#include <load_img.h>

#include "plat_common.h"
#include <platform_def.h>

#ifdef CONFIG_STATIC_DDR

const struct ddr_cfg_regs static_3200 = {
	.cs[0].bnds = U(0x03FFU),
	.cs[1].bnds = U(0x03FF),
	.cs[0].config = U(0x80050422),
	.cs[1].config = U(0x80000422),
	.cs[2].bnds = U(0x00),
	.cs[3].bnds = U(0x00),
	.cs[2].config = U(0x00),
	.cs[3].config = U(0x00),
	.timing_cfg[0] = U(0xFFAA0018),
	.timing_cfg[1] = U(0x646A8844),
	.timing_cfg[2] = U(0x00058022),
	.timing_cfg[3] = U(0x13622100),
	.timing_cfg[4] = U(0x02),
	.timing_cfg[5] = U(0x07401400),
	.timing_cfg[7] = U(0x3BB00000),
	.timing_cfg[8] = U(0x0944AC00),
	.sdram_cfg[0] = U(0x65044008),
	.sdram_cfg[1] = U(0x00401011),
	.sdram_cfg[2] = U(0x00),
	.sdram_mode[0] = U(0x06010C50),
	.sdram_mode[1] = U(0x00280400),
	.sdram_mode[2] = U(0x00),
	.sdram_mode[3] = U(0x00),
	.sdram_mode[4] = U(0x00),
	.sdram_mode[5] = U(0x00),
	.sdram_mode[6] = U(0x00),
	.sdram_mode[7] = U(0x00),
	.sdram_mode[8] = U(0x0500),
	.sdram_mode[9] = U(0x10240000),
	.sdram_mode[10] = U(0x00),
	.sdram_mode[11] = U(0x00),
	.sdram_mode[12] = U(0x00),
	.sdram_mode[13] = U(0x00),
	.sdram_mode[14] = U(0x00),
	.sdram_mode[15] = U(0x00),
	.md_cntl = U(0x00),
	.interval = U(0x30C00000),
	.data_init = U(0xDEADBEEF),
	.init_addr = U(0x00),
	.zq_cntl = U(0x8A090705),
	.sdram_rcw[0] = U(0x00),
	.sdram_rcw[1] = U(0x00),
	.sdram_rcw[2] = U(0x00),
	.sdram_rcw[3] = U(0x00),
	.sdram_rcw[4] = U(0x00),
	.sdram_rcw[5] = U(0x00),
	.err_disable = U(0x00),
	.err_int_en = U(0x00),
};

const struct ddr_cfg_regs static_2900 = {
	.cs[0].bnds = U(0x03FF),
	.cs[1].bnds = U(0x03FF),
	.cs[0].config = U(0x80050422),
	.cs[1].config = U(0x80000422),
	.cs[2].bnds = U(0x00),
	.cs[3].bnds = U(0x00),
	.cs[2].config = U(0x00),
	.cs[3].config = U(0x00),
	.timing_cfg[0] = U(0xFF990018),
	.timing_cfg[1] = U(0x4F4A4844),
	.timing_cfg[2] = U(0x0005601F),
	.timing_cfg[3] = U(0x125F2100),
	.timing_cfg[4] = U(0x02),
	.timing_cfg[5] = U(0x07401400),
	.timing_cfg[7] = U(0x3AA00000),
	.timing_cfg[8] = U(0x09449B00),
	.sdram_cfg[0] = U(0x65044008),
	.sdram_cfg[1] = U(0x00401011),
	.sdram_cfg[2] = U(0x00),
	.sdram_mode[0] = U(0x06010C50),
	.sdram_mode[1] = U(0x00280400),
	.sdram_mode[2] = U(0x00),
	.sdram_mode[3] = U(0x00),
	.sdram_mode[4] = U(0x00),
	.sdram_mode[5] = U(0x00),
	.sdram_mode[6] = U(0x00),
	.sdram_mode[7] = U(0x00),
	.sdram_mode[8] = U(0x0500),
	.sdram_mode[9] = U(0x10240000),
	.sdram_mode[10] = U(0x00),
	.sdram_mode[11] = U(0x00),
	.sdram_mode[12] = U(0x00),
	.sdram_mode[13] = U(0x00),
	.sdram_mode[14] = U(0x00),
	.sdram_mode[15] = U(0x00),
	.md_cntl = U(0x00),
	.interval = U(0x2C2E0000),
	.data_init = U(0xDEADBEEF),
	.init_addr = U(0x00),
	.zq_cntl = U(0x8A090705),
	.sdram_rcw[0] = U(0x00),
	.sdram_rcw[1] = U(0x00),
	.sdram_rcw[2] = U(0x00),
	.sdram_rcw[3] = U(0x00),
	.sdram_rcw[4] = U(0x00),
	.sdram_rcw[5] = U(0x00),
	.err_disable = U(0x00),
	.err_int_en = U(0x00),
};

const struct ddr_cfg_regs static_2600 = {
	.cs[0].bnds = U(0x03FF),
	.cs[1].bnds = U(0x03FF),
	.cs[0].config = U(0x80050422),
	.cs[1].config = U(0x80000422),
	.cs[2].bnds = U(0x00),
	.cs[3].bnds = U(0x00),
	.cs[2].config = U(0x00),
	.cs[3].config = U(0x00),
	.timing_cfg[0] = U(0xFF880018),
	.timing_cfg[1] = U(0x2A24F444),
	.timing_cfg[2] = U(0x007141DC),
	.timing_cfg[3] = U(0x125B2100),
	.timing_cfg[4] = U(0x02),
	.timing_cfg[5] = U(0x06401400),
	.timing_cfg[7] = U(0x28800000),
	.timing_cfg[8] = U(0x07338A00),
	.sdram_cfg[0] = U(0x65044008),
	.sdram_cfg[1] = U(0x00401011),
	.sdram_cfg[2] = U(0x00),
	.sdram_mode[0] = U(0x06010A70),
	.sdram_mode[1] = U(0x00200400),
	.sdram_mode[2] = U(0x00),
	.sdram_mode[3] = U(0x00),
	.sdram_mode[4] = U(0x00),
	.sdram_mode[5] = U(0x00),
	.sdram_mode[6] = U(0x00),
	.sdram_mode[7] = U(0x00),
	.sdram_mode[8] = U(0x0500),
	.sdram_mode[9] = U(0x0C240000),
	.sdram_mode[10] = U(0x00),
	.sdram_mode[11] = U(0x00),
	.sdram_mode[12] = U(0x00),
	.sdram_mode[13] = U(0x00),
	.sdram_mode[14] = U(0x00),
	.sdram_mode[15] = U(0x00),
	.md_cntl = U(0x00),
	.interval = U(0x279C0000),
	.data_init = U(0xDEADBEEF),
	.init_addr = U(0x00),
	.zq_cntl = U(0x8A090705),
	.sdram_rcw[0] = U(0x00),
	.sdram_rcw[1] = U(0x00),
	.sdram_rcw[2] = U(0x00),
	.sdram_rcw[3] = U(0x00),
	.sdram_rcw[4] = U(0x00),
	.sdram_rcw[5] = U(0x00),
	.err_disable = U(0x00),
	.err_int_en = U(0x00),
};

const struct dimm_params static_dimm = {
	.rdimm = 0U,
	.primary_sdram_width = 64U,
	.ec_sdram_width = 8U,
	.n_ranks = 2U,
	.device_width = 8U,
	.mirrored_dimm = 1U,
};

/* Sample code using two UDIMM MT18ASF1G72AZ-2G6B1, on each DDR controller */
unsigned long long board_static_ddr(struct ddr_info *priv)
{
	memcpy(&priv->ddr_reg, &static_2900, sizeof(static_2900));
	memcpy(&priv->dimm, &static_dimm, sizeof(static_dimm));
	priv->conf.cs_on_dimm[0] = 0x3;
	ddr_board_options(priv);
	compute_ddr_phy(priv);

	return ULL(0x400000000);
}

#elif defined(CONFIG_DDR_NODIMM)
/*
 * Sample code to bypass reading SPD. This is a sample, not recommended
 * for boards with slots. DDR model number: UDIMM MT18ASF1G72AZ-2G6B1.
 */
struct dimm_params ddr_raw_timing = {
	.n_ranks = 2U,
	.rank_density = U(0x200000000),
	.capacity = U(0x400000000),
	.primary_sdram_width = 64U,
	.ec_sdram_width = 8U,
	.device_width = 8U,
	.die_density = U(0x5),
	.rdimm = 0U,
	.mirrored_dimm = 1U,
	.n_row_addr = 16U,
	.n_col_addr = 10U,
	.bank_addr_bits = 0U,
	.bank_group_bits = 2U,
	.edc_config = 2U,
	.burst_lengths_bitmask = U(0x0c),
	.tckmin_x_ps = 625,
	.tckmax_ps = 1600,
	.caslat_x = U(0x15FFFC00),
	.taa_ps = 13750,
	.trcd_ps = 13750,
	.trp_ps = 13750,
	.tras_ps = 32000,
	.trc_ps = 457500,
	.twr_ps = 15000,
	.trfc1_ps = 350000,
	.trfc2_ps = 260000,
	.trfc4_ps = 160000,
	.tfaw_ps = 21000,
	.trrds_ps = 2500,
	.trrdl_ps = 4900,
	.tccdl_ps = 5000,
	.refresh_rate_ps = 7800000U,
};

int ddr_get_ddr_params(struct dimm_params *pdimm,
		       struct ddr_conf *conf)
{
	static const char dimm_model[] = "Fixed DDR on board";

	conf->dimm_in_use[0] = 1;	/* Modify accordingly */
	memcpy(pdimm, &ddr_raw_timing, sizeof(struct dimm_params));
	memcpy(pdimm->mpart, dimm_model, sizeof(dimm_model) - 1);

	/* valid DIMM mask, change accordingly, together with dimm_on_ctlr. */
	return 0x5;
}
#endif	/* CONFIG_DDR_NODIMM */

int ddr_board_options(struct ddr_info *priv)
{
	struct memctl_opt *popts = &priv->opt;
	const struct ddr_conf *conf = &priv->conf;

	popts->vref_dimm = U(0x19);		/* range 1, 83.4% */
	popts->rtt_override = 1U;
	popts->rtt_override_value = 0x5U;	/* RTT being used as 60 ohm */
	popts->rtt_park = 120U;
	popts->otf_burst_chop_en = 0;
	popts->burst_length = DDR_BL8;
	popts->trwt_override = 1U;
	popts->bstopre = 0U;			/* auto precharge */
	popts->addr_hash = 1;

	/* Set ODT impedance on PHY side */
	switch (conf->cs_on_dimm[1]) {
	case 0xc:	/* Two slots dual rank */
	case 0x4:	/* Two slots single rank, not valid for interleaving */
		popts->trwt = U(0xf);
		popts->twrt = U(0x7);
		popts->trrt = U(0x7);
		popts->twwt = U(0x7);
		popts->vref_phy = U(0x6B);	/* 83.6% */
		popts->odt = 60U;
		popts->phy_tx_impedance = 28U;
		break;
	case 0:		/* Ont slot used */
	default:
		popts->trwt = U(0x3);
		popts->twrt = U(0x3);
		popts->trrt = U(0x3);
		popts->twwt = U(0x3);
		popts->vref_phy = U(0x5D);		/* 72% */
		popts->odt = 60U;
		popts->phy_tx_impedance = 28U;
		break;
	}

	return 0;
}

#ifdef NXP_WARM_BOOT
long long init_ddr(uint32_t wrm_bt_flg)
#else
long long init_ddr(void)
#endif
{
	int spd_addr[] = { 0x51, 0x52, 0x53, 0x54 };
	struct ddr_info info;
	struct sysinfo sys;
	long long dram_size;

	zeromem(&sys, sizeof(sys));
	if (get_clocks(&sys) != 0) {
		ERROR("System clocks are not set\n");
		panic();
	}
	debug("platform clock %lu\n", sys.freq_platform);
	debug("DDR PLL1 %lu\n", sys.freq_ddr_pll0);
	debug("DDR PLL2 %lu\n", sys.freq_ddr_pll1);

	zeromem(&info, sizeof(info));

	/* Set two DDRC. Unused DDRC will be removed automatically. */
	info.num_ctlrs = NUM_OF_DDRC;
	info.spd_addr = spd_addr;
	info.ddr[0] = (void *)NXP_DDR_ADDR;
	info.ddr[1] = (void *)NXP_DDR2_ADDR;
	info.phy[0] = (void *)NXP_DDR_PHY1_ADDR;
	info.phy[1] = (void *)NXP_DDR_PHY2_ADDR;
	info.clk = get_ddr_freq(&sys, 0);
	info.img_loadr = load_img;
	info.phy_gen2_fw_img_buf = PHY_GEN2_FW_IMAGE_BUFFER;
	if (info.clk == 0) {
		info.clk = get_ddr_freq(&sys, 1);
	}
	info.dimm_on_ctlr = DDRC_NUM_DIMM;

	info.warm_boot_flag = DDR_WRM_BOOT_NT_SUPPORTED;
#ifdef NXP_WARM_BOOT
	if (wrm_bt_flg != 0) {
		info.warm_boot_flag = DDR_WARM_BOOT;
	} else {
		info.warm_boot_flag = DDR_COLD_BOOT;
	}
#endif

	dram_size = dram_init(&info
#if defined(NXP_HAS_CCN504) || defined(NXP_HAS_CCN508)
				    , NXP_CCN_HN_F_0_ADDR
#endif
			);


	if (dram_size < 0) {
		ERROR("DDR init failed.\n");
	}

	return dram_size;
}
