#include <stdio.h>
#include <string.h>
#include "i2c.h"

I2C_HandleTypeDef i2c_handle;
uint8_t i2c_rx[I2C_BUF_SIZE];
uint8_t i2c_tx[I2C_BUF_SIZE];
uint8_t i2c_transfer[I2C_BUF_SIZE];
volatile uint32_t i2c_flags;

/*****************************************************************************/

int i2c_block(uint32_t flag, uint32_t ticks_start) {
	while ((i2c_flags & flag) && I2C_TIMEOUT_CHK) {;}
	return I2C_TIMEOUT_CHK;
}

bool i2c_errors_present(void) {
	return (i2c_flags & I2C_ERROR) ? true : false;
}

HAL_StatusTypeDef i2c_init(void) {
	HAL_StatusTypeDef retval;

	HAL_I2C_DeInit(&i2c_handle);

	/* clock source: PCLK1 (ABP1, 24 MHz) */
	__HAL_RCC_I2C1_CLK_ENABLE();

	/* enable interrupts */
	NVIC_SetPriority(I2C1_EV_IRQn, 3);
	NVIC_EnableIRQ(I2C1_EV_IRQn);
	NVIC_SetPriority(I2C1_ER_IRQn, 3);
	NVIC_EnableIRQ(I2C1_ER_IRQn);

	i2c_handle.Instance					= I2C1;

	i2c_handle.Init.Timing				= 0x0;

	/* 8 bits, tLOW constraints: >= ~1.3 us */
	i2c_handle.Init.Timing				|= 0x13 << I2C_TIMINGR_SCLL_Pos;

	/* 8 bits, tHIGH constraints: >= ~0.6 us */
	i2c_handle.Init.Timing				|= 0x0F << I2C_TIMINGR_SCLH_Pos;

	/* 4 bits, tSDADEL constraints: UNKNOWN */
	i2c_handle.Init.Timing				|= 0x02 << I2C_TIMINGR_SDADEL_Pos;

	/* 4 bits, tSCLDEL constraints: UNKNOWN */
	i2c_handle.Init.Timing				|= 0x04 << I2C_TIMINGR_SCLDEL_Pos;

	/* 4 bits, constraints:
	 *
	 *     tI2CCLK < (tLOW - tfilters) / 4 and tI2CCLK < tHIGH
	 *
	 * tLOW:   SCL low time  (>= 1.3 us)
	 * tHIGH : SCL high time (>= 0.6 us)
	 *
	 * tfilters: when enabled, sum of the delays brought by the analog
	 *           filter and by the digital filter
	 *
	 * 24 MHz / 3 = 8 MHz, tI2CCLK = 0.125 us
	 *
	 */
	i2c_handle.Init.Timing				|= 0x02 << I2C_TIMINGR_PRESC_Pos;

	i2c_handle.Init.OwnAddress1			= 0x01;
	i2c_handle.Init.AddressingMode		= I2C_ADDRESSINGMODE_7BIT;
	i2c_handle.Init.DualAddressMode		= I2C_DUALADDRESS_DISABLE;
	i2c_handle.Init.GeneralCallMode		= I2C_GENERALCALL_DISABLE;
	i2c_handle.Init.NoStretchMode		= I2C_NOSTRETCH_DISABLE;

	retval = HAL_I2C_Init(&i2c_handle);
	//HAL_I2CEx_ConfigAnalogFilter(&i2c_handle, I2C_ANALOGFILTER_DISABLE);
	
	memset(i2c_rx, 0x0, I2C_BUF_SIZE);
	memset(i2c_tx, 0x0, I2C_BUF_SIZE);
	i2c_flags = 0x0;

	return retval;
}

HAL_StatusTypeDef i2c_query(uint8_t addr) {
	return HAL_I2C_IsDeviceReady(&i2c_handle, addr << 1, 2, BLOOP_I2C_TIMEOUT);
}

HAL_StatusTypeDef i2c_start_write(
	uint8_t addr, uint16_t nbytes, const uint8_t *data
) {
	memcpy(i2c_tx, data, nbytes);
	i2c_flags |= I2C_WAITING_TX;
	i2c_flags &= I2C_TX_READY;
	return HAL_I2C_Master_Transmit_IT(&i2c_handle, addr << 1, i2c_tx, nbytes);
}

bool i2c_write_ready(void) {
	return (i2c_flags & I2C_TX_READY) ? true : false;
}

HAL_StatusTypeDef i2c_start_read(uint8_t addr, uint16_t nbytes) {
	i2c_flags |= I2C_WAITING_RX;
	i2c_flags &= I2C_RX_READY;
	return HAL_I2C_Master_Receive_IT(
		&i2c_handle, (addr << 1) | 0x1, i2c_rx, nbytes
	);
}

bool i2c_read_ready(void) {
	return (i2c_flags & I2C_RX_READY) ? true : false;
}

HAL_StatusTypeDef i2c_start_mem_read(
	uint8_t addr, uint16_t memAddr, uint16_t memAddrSize, uint16_t nbytes
) {
	i2c_flags |= I2C_WAITING_MEM_RX;
	i2c_flags &= I2C_MEM_RX_READY;
	return HAL_I2C_Mem_Read_IT(
		&i2c_handle, (addr << 1) | 0x1, memAddr, memAddrSize, i2c_rx, nbytes
	);
}

bool i2c_mem_read_ready(void) {
	return (i2c_flags & I2C_MEM_RX_READY) ? true : false;
}

HAL_StatusTypeDef i2c_start_mem_write(
	uint8_t addr, uint16_t memAddr, uint16_t memAddrSize,
	uint16_t nbytes, const uint8_t *data
) {
	memcpy(i2c_tx, data, nbytes);
	i2c_flags |= I2C_WAITING_MEM_TX;
	i2c_flags &= I2C_MEM_TX_READY;
	return HAL_I2C_Mem_Write_IT(
		&i2c_handle, addr << 1, memAddr, memAddrSize, i2c_tx, nbytes
	);
}

bool i2c_mem_write_ready(void) {
	return (i2c_flags & I2C_MEM_TX_READY) ? true : false;
}

/*****************************************************************************/

void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c) {
	UNUSED(hi2c);
	i2c_flags &= ~(I2C_WAITING_MEM_TX);
	i2c_flags |= I2C_MEM_TX_READY;
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c) {
	UNUSED(hi2c);
	i2c_flags &= ~(I2C_WAITING_MEM_RX);
	i2c_flags |= I2C_MEM_RX_READY;
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c) {
	UNUSED(hi2c);
	i2c_flags &= ~(I2C_WAITING_TX);
	i2c_flags |= I2C_TX_READY;
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c) {
	UNUSED(hi2c);
	i2c_flags &= ~(I2C_WAITING_RX);
	i2c_flags |= I2C_RX_READY;
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c) {
	printf("%s: why are we here?\r\n", __func__);
	print_i2c_state(hi2c);
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c) {
	printf("%s: why are we here?\r\n", __func__);
	print_i2c_state(hi2c);
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c) {
	printf("%s\r\n", __func__);
	print_i2c_error(hi2c);
	i2c_flags |= I2C_ERROR;
}

void HAL_I2C_AbortCpltCallback(I2C_HandleTypeDef *hi2c) {
	printf("%s\r\n", __func__);
	print_i2c_error(hi2c);
	i2c_flags |= I2C_ERROR;
}

void I2C1_EV_IRQHandler(void) {
	HAL_I2C_EV_IRQHandler(&i2c_handle);
}

void I2C1_ER_IRQHandler(void) {
	HAL_I2C_ER_IRQHandler(&i2c_handle);
}

void print_i2c_error(I2C_HandleTypeDef *handle) {
	const char *err_msg;
	switch (handle->ErrorCode) {
		case HAL_I2C_ERROR_NONE: err_msg = "none"; break;
		case HAL_I2C_ERROR_BERR: err_msg = "bus"; break;
		case HAL_I2C_ERROR_ARLO: err_msg = "arbitration lost"; break;
		case HAL_I2C_ERROR_AF: err_msg = " no ack"; break;
		case HAL_I2C_ERROR_OVR: err_msg = "over/underrun"; break;
		case HAL_I2C_ERROR_DMA: err_msg = "dma transfer"; break;
		case HAL_I2C_ERROR_TIMEOUT: err_msg = "timeout"; break;
		case HAL_I2C_ERROR_SIZE: err_msg = "size management"; break;
		default: err_msg = "uknown";
	}
	printf("i2c error: %s\r\n", err_msg);
}

const char *i2c_state_str(HAL_I2C_StateTypeDef value) {
	switch (value) {
		case HAL_I2C_STATE_RESET: return "reset";
		case HAL_I2C_STATE_READY: return "ready";
		case HAL_I2C_STATE_BUSY: return "busy";
		case HAL_I2C_STATE_BUSY_TX: return "busy_tx";
		case HAL_I2C_STATE_BUSY_RX: return "busy_rx";
		case HAL_I2C_STATE_LISTEN: return "listen";
		case HAL_I2C_STATE_BUSY_TX_LISTEN: return "tx_listen";
		case HAL_I2C_STATE_BUSY_RX_LISTEN: return "rx_listen";
		case HAL_I2C_STATE_ABORT: return "abort";
		case HAL_I2C_STATE_TIMEOUT: return "timeout";
		case HAL_I2C_STATE_ERROR: return "error";
	}
	return "unknown";
}

void print_i2c_state(I2C_HandleTypeDef *handle) {
	printf("i2c curr state: %s | prev state: %s\r\n",
			i2c_state_str(handle->State),
			i2c_state_str(handle->PreviousState)
	);
}

void i2c_dump(void) {
	print_i2c_error(&i2c_handle);
	print_i2c_state(&i2c_handle);
	printf("internal flags: 0x%lx\r\n", i2c_flags);
}
