#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "commands.h"
#include "stm32l4xx_hal_conf.h"

I2C_HandleTypeDef i2c_handle;

void print_hal(HAL_StatusTypeDef value) {
	const char *err_msg;
	switch (value) {
		case HAL_OK: err_msg = "ok"; break;
		case HAL_ERROR: err_msg = "error"; break;
		case HAL_BUSY: err_msg = "busy"; break;
		case HAL_TIMEOUT: err_msg = "timeout"; break;
		default: err_msg = "uknown";
	}
	printf("hal returned '%s'\r\n", err_msg);
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

command_status do_init(void) {
	HAL_StatusTypeDef retval;

	HAL_I2C_DeInit(&i2c_handle);

	/* clock source: PCLK1 (ABP1, 24 MHz) */
	__HAL_RCC_I2C1_CLK_ENABLE();

	i2c_handle.Instance					= I2C1;

	i2c_handle.Init.Timing				= 0x0;

	/* 8 bits, tLOW constraints: >= ~1.3 us */
	i2c_handle.Init.Timing				|= 0x09 << I2C_TIMINGR_SCLL_Pos;

	/* 8 bits, tHIGH constraints: >= ~0.6 us */
	i2c_handle.Init.Timing				|= 0x03 << I2C_TIMINGR_SCLH_Pos;

	/* 4 bits, tSDADEL constraints: UNKNOWN */
	i2c_handle.Init.Timing				|= 0x01 << I2C_TIMINGR_SDADEL_Pos;

	/* 4 bits, tSCLDEL constraints: UNKNOWN */
	i2c_handle.Init.Timing				|= 0x03 << I2C_TIMINGR_SCLDEL_Pos;

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
	HAL_I2CEx_ConfigAnalogFilter(&i2c_handle, I2C_ANALOGFILTER_DISABLE);

	return (retval == HAL_OK) ? CMD_SUCCESS : FAIL;
}

command_status try_query(uint8_t addr) {
	HAL_StatusTypeDef retval;
	retval = HAL_I2C_IsDeviceReady(&i2c_handle, addr << 1, 2, 50);
	if (retval == HAL_OK)
		printf("found 0x%x\r\n", addr);
	else if (retval != HAL_TIMEOUT) {
		print_hal(retval);
		print_i2c_error(&i2c_handle);
		print_i2c_state(&i2c_handle);
	}
	return (retval == HAL_OK) ? CMD_SUCCESS : FAIL;
}

command_status try_scan(void) {
	uint8_t i;
	command_status retval = FAIL;
	for (i = 0; i < 128; i++)
		if (try_query(i) == CMD_SUCCESS)
			retval = CMD_SUCCESS;
	return retval;
}

command_status do_i2c(int argc, char *argv[]) {

	if (argc == 1) return USAGE;

	if (!strcmp("init", argv[1]))
		return do_init();
	else if (!strcmp("query", argv[1]) && argc >= 3)
		return try_query((uint8_t) strtol(argv[2], NULL, 16));
	else if (!strcmp("scan", argv[1]))
		return try_scan();

	return USAGE;
}

COMMAND_ENTRY(
	"i2c",
	"i2c {init|query <addr>|scan}",
	"Interact with the I2C subsystem.",
	do_i2c
)
