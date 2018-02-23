/*
 * Copyright © 2017-2018 The Crust Firmware Authors.
 * SPDX-License-Identifier: (BSD-3-Clause OR GPL-2.0)
 */

#include <assert.h>
#include <bakery_lock.h>
#include <css_mhu.h>
#include <debug.h>
#include <delay_timer.h>
#include <mmio.h>
#include <stdbool.h>
#include <sun50i_mmap.h>
#include <utils_def.h>

#define IRQ_EN_REG		0x0060
#define IRQ_STATUS_REG		0x0070
#define RX_IRQ(n)		BIT(2 + 4 * (n))

#define REMOTE_IRQ_EN_REG	0x0040
#define REMOTE_IRQ_STATUS_REG	0x0050
#define REMOTE_RX_IRQ(n)	BIT(0 + 4 * (n))

#define RX_MSG_DATA_REG(n)	(0x0184 + 0x8 * (n))
#define TX_MSG_DATA_REG(n)	(0x0180 + 0x8 * (n))

#define MBOX_CHANNEL		1
#define MBOX_TIMEOUT_ITER	5000 /* 500 ms total */
#define MBOX_TIMEOUT_UNIT	100  /* 100 μs each  */

#define MHU_MAX_SLOT_ID		31

DEFINE_BAKERY_LOCK(msgbox_transaction_lock);

static bool
sunxi_msgbox_rx_pending(void)
{
	return mmio_read_32(SUN50I_MSGBOX_BASE + IRQ_STATUS_REG)
		& RX_IRQ(MBOX_CHANNEL);
}

static bool
sunxi_msgbox_tx_pending(void)
{
	return mmio_read_32(SUN50I_MSGBOX_BASE + REMOTE_IRQ_STATUS_REG)
		& REMOTE_RX_IRQ(MBOX_CHANNEL);
}

void mhu_secure_message_start(unsigned int slot_id)
{
	uint32_t timeout = MBOX_TIMEOUT_ITER;

	assert(slot_id <= MHU_MAX_SLOT_ID);

	bakery_lock_get(&msgbox_transaction_lock);

	/* Make sure any previous command has finished */
	while (sunxi_msgbox_tx_pending() && --timeout)
		udelay(MBOX_TIMEOUT_UNIT);

	if (!timeout) WARN("%s timeout!\n", __func__);
}

void mhu_secure_message_send(unsigned int slot_id)
{
	assert(slot_id <= MHU_MAX_SLOT_ID);
	assert(!sunxi_msgbox_tx_pending());

	/* Send command to SCP */
	mmio_write_32(SUN50I_MSGBOX_BASE + TX_MSG_DATA_REG(MBOX_CHANNEL),
			BIT(slot_id));
}

uint32_t mhu_secure_message_wait(void)
{
	uint32_t timeout = MBOX_TIMEOUT_ITER;

	/* Wait for response from SCP */
	while (!sunxi_msgbox_rx_pending() && --timeout)
		udelay(MBOX_TIMEOUT_UNIT);

	if (!timeout) WARN("%s timeout!\n", __func__);

	return mmio_read_32(SUN50I_MSGBOX_BASE + RX_MSG_DATA_REG(MBOX_CHANNEL));
}

void mhu_secure_message_end(unsigned int slot_id)
{
	assert(slot_id <= MHU_MAX_SLOT_ID);

	/* Clear any response we got by clearing the interrupt status. */
	mmio_write_32(SUN50I_MSGBOX_BASE + IRQ_STATUS_REG,
			RX_IRQ(MBOX_CHANNEL));

	bakery_lock_release(&msgbox_transaction_lock);
}
