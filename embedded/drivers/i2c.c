#include <stdio.h>
#include "i2c.h"

I2C_HandleTypeDef i2c_handle;
uint8_t i2c_rx[I2C_BUF_SIZE];
uint8_t i2c_tx[I2C_BUF_SIZE];
uint8_t i2c_transfer[I2C_BUF_SIZE];
volatile uint32_t i2c_flags;

void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c) {
	UNUSED(hi2c);
	i2c_flags &= ~(I2C_WAITING_MEM_TX);
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c) {
	UNUSED(hi2c);
	i2c_flags &= ~(I2C_WAITING_MEM_RX);
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c) {
	UNUSED(hi2c);
	i2c_flags &= ~(I2C_WAITING_TX);
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c) {
	UNUSED(hi2c);
	i2c_flags &= ~(I2C_WAITING_RX);
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
