/*
 * Copyright (c) 2017-2019, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <stdbool.h>

#include <drivers/delay_timer.h>
#include <lib/bakery_lock.h>
#include <lib/mmio.h>
#include <lib/utils_def.h>

#include <sunxi_mmap.h>

#define SCP_IRQ_EN_REG		0x0040
#define SCP_IRQ_STAT_REG	0x0050
#define AP_IRQ_EN_REG		0x0060
#define AP_IRQ_STAT_REG		0x0070

#define RX_IRQ(n)		BIT(0 + 2 * (n))

#define MSG_STAT_REG(n)		(0x0140 + 0x4 * (n))
#define MSG_DATA_REG(n)		(0x0180 + 0x4 * (n))

#define SCPI_CHANNEL_AP_TO_SCP	2
#define SCPI_CHANNEL_SCP_TO_AP	3
#define SCPI_TIMEOUT_ITERATIONS	5000 /* 500 ms total */
#define SCPI_TIMEOUT_DELAY	100  /* 100 μs each  */

#define MHU_MAX_SLOT_ID		31

static DEFINE_BAKERY_LOCK(mhu_secure_message_lock);

static bool sunxi_msgbox_rx_pending(void)
{
	return mmio_read_32(SUNXI_MSGBOX_BASE +
			    MSG_STAT_REG(SCPI_CHANNEL_SCP_TO_AP));
}

static bool sunxi_msgbox_tx_done(void)
{
	uint32_t status = mmio_read_32(SUNXI_MSGBOX_BASE + SCP_IRQ_STAT_REG);

	return !(status & RX_IRQ(SCPI_CHANNEL_AP_TO_SCP));
}

void mhu_secure_message_start(unsigned int slot_id)
{
	uint32_t timeout = SCPI_TIMEOUT_ITERATIONS;

	assert(slot_id <= MHU_MAX_SLOT_ID);

	bakery_lock_get(&mhu_secure_message_lock);

	/* Make sure any previous command has finished. */
	while (!sunxi_msgbox_tx_done() && timeout--)
		udelay(SCPI_TIMEOUT_DELAY);
}

void mhu_secure_message_send(unsigned int slot_id)
{
	assert(slot_id <= MHU_MAX_SLOT_ID);

	/* Send command to the SCP. */
	mmio_write_32(SUNXI_MSGBOX_BASE + MSG_DATA_REG(SCPI_CHANNEL_AP_TO_SCP),
		      BIT(slot_id));
}

uint32_t mhu_secure_message_wait(void)
{
	uint32_t response;
	uint32_t timeout = SCPI_TIMEOUT_ITERATIONS;

	/* Wait for a response from the SCP. */
	while (!sunxi_msgbox_rx_pending() && timeout--)
		udelay(SCPI_TIMEOUT_DELAY);

	/* Return the last message in the FIFO. */
	do {
		response = mmio_read_32(SUNXI_MSGBOX_BASE +
					MSG_DATA_REG(SCPI_CHANNEL_SCP_TO_AP));
	} while (sunxi_msgbox_rx_pending());

	return response;
}

void mhu_secure_message_end(unsigned int slot_id)
{
	assert(slot_id <= MHU_MAX_SLOT_ID);

	/* Clear any response we got by clearing the interrupt status. */
	mmio_write_32(SUNXI_MSGBOX_BASE + AP_IRQ_STAT_REG,
		      RX_IRQ(SCPI_CHANNEL_SCP_TO_AP));

	bakery_lock_release(&mhu_secure_message_lock);
}
