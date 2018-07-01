#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "commands.h"
#include "stm32l4xx_hal_conf.h"

extern volatile uint32_t ticks;

#define BLOOP_I2C_TIMEOUT	50
#define I2C_BUF_SIZE		64

I2C_HandleTypeDef i2c_handle;
uint8_t i2c_rx[I2C_BUF_SIZE];
uint8_t i2c_tx[I2C_BUF_SIZE];
uint8_t i2c_transfer[I2C_BUF_SIZE];

volatile uint32_t i2c_flags = 0x0;
#define I2C_WAITING_TX		0x00000001
#define I2C_WAITING_RX		0x00000002
#define I2C_WAITING_MEM_TX	0x00000004
#define I2C_WAITING_MEM_RX	0x00000008
#define I2C_ERROR			0x00000010
#define I2C_CLEAR_ERROR		i2c_flags &= ~(I2C_ERROR)
#define I2C_TIMEOUT_CHK		(ticks - ticks_start < BLOOP_I2C_TIMEOUT)

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

	return (retval == HAL_OK) ? CMD_SUCCESS : FAIL;
}

command_status try_query(uint8_t addr) {
	HAL_StatusTypeDef retval;
	retval = HAL_I2C_IsDeviceReady(&i2c_handle, addr << 1, 2, BLOOP_I2C_TIMEOUT);
	if (retval == HAL_OK)
		printf("found 0x%x\r\n", addr);
	else if (retval != HAL_TIMEOUT) {
		print_hal(retval);
		print_i2c_error(&i2c_handle);
		print_i2c_state(&i2c_handle);
	}
	return (retval == HAL_OK) ? CMD_SUCCESS : FAIL;
}

command_status try_dump(void) {
	print_i2c_error(&i2c_handle);
	print_i2c_state(&i2c_handle);
	printf("internal flags: 0x%lx\r\n", i2c_flags);
	return CMD_SUCCESS;
}

command_status try_scan(void) {
	uint8_t i;
	command_status retval = FAIL;
	for (i = 0; i < 128; i++)
		if (try_query(i) == CMD_SUCCESS)
			retval = CMD_SUCCESS;
	return retval;
}

command_status try_write(uint8_t addr, uint16_t nbytes, const uint8_t *data) {
	uint32_t ticks_start = ticks;
	memcpy(i2c_tx, data, nbytes);
	if (HAL_I2C_Master_Transmit_IT(&i2c_handle, addr << 1, i2c_tx, nbytes) != HAL_OK)
		return FAIL;
	i2c_flags |= I2C_WAITING_TX;
	while ((i2c_flags & I2C_WAITING_TX) && I2C_TIMEOUT_CHK) {;}
	return (i2c_flags & I2C_ERROR) ? FAIL : CMD_SUCCESS;
}

command_status try_read(uint8_t addr, uint16_t nbytes) {
	uint16_t i;
	uint32_t ticks_start = ticks;
	if (HAL_I2C_Master_Receive_IT(&i2c_handle, addr << 1, i2c_rx, nbytes) != HAL_OK)
		return FAIL;
	i2c_flags |= I2C_WAITING_RX;
	while ((i2c_flags & I2C_WAITING_RX) && I2C_TIMEOUT_CHK) {;}
	if (i2c_flags & I2C_ERROR) return FAIL;
	for (i = 0; i < nbytes; i++) printf("%d: 0x%x\r\n", i, i2c_rx[i]);
	return CMD_SUCCESS;
}

command_status try_mem_read(uint8_t addr, uint16_t memAddr, uint16_t memAddrSize, uint16_t nbytes) {
	uint16_t i;
	uint32_t ticks_start = ticks;
	if (HAL_I2C_Mem_Read_IT(&i2c_handle, addr << 1, memAddr, memAddrSize, i2c_rx, nbytes) != HAL_OK)
		return FAIL;
	i2c_flags |= I2C_WAITING_MEM_RX;
	while ((i2c_flags & I2C_WAITING_MEM_RX) && I2C_TIMEOUT_CHK) {;}
	if (i2c_flags & I2C_ERROR) return FAIL;
	for (i = 0; i < nbytes; i++) printf("%d: 0x%x\r\n", i, i2c_rx[i]);
	return CMD_SUCCESS;
}

command_status try_mem_write(uint8_t addr, uint16_t memAddr, uint16_t memAddrSize, uint16_t nbytes, const uint8_t *data) {
	uint32_t ticks_start = ticks;
	memcpy(i2c_tx, data, nbytes);
	if (HAL_I2C_Mem_Write_IT(&i2c_handle, addr << 1, memAddr, memAddrSize, i2c_tx, nbytes) != HAL_OK)
		return FAIL;
	i2c_flags |= I2C_WAITING_MEM_TX;
	while ((i2c_flags & I2C_WAITING_MEM_TX) && I2C_TIMEOUT_CHK) {;}
	return ((i2c_flags & I2C_ERROR) || !I2C_TIMEOUT_CHK) ? FAIL : CMD_SUCCESS;
}

command_status do_i2c(int argc, char *argv[]) {

	uint8_t addr;
	uint16_t nbytes, memAddr, i;

	if (argc == 1) return USAGE;

	I2C_CLEAR_ERROR;

	/* init (or re-init) the i2c subsystem */
	if (!strcmp("init", argv[1]))
		return do_init();

	/* search the entire i2c address space for nodes */
	else if (!strcmp("scan", argv[1]))
		return try_scan();

	else if (!strcmp("dump", argv[1]))
		return try_dump();

	if (argc == 2) return USAGE;

	addr = (uint8_t) strtol(argv[2], NULL, 16);
	if (addr > 127) {
		printf("'%s' is too large\r\n", argv[2]);
		return FAIL;
	}

	/* query a specific node address for an ACK */
	if (!strcmp("query", argv[1]))
		return try_query(addr);

	if (argc == 3) return USAGE;

	/* read n bytes */
	if (!strcmp("r", argv[1])) {
		nbytes = (uint16_t) strtol(argv[3], NULL, 10);
		return try_read(addr, nbytes);
	}

	/* write remaining input data to the specified address */
	else if (!strcmp("w", argv[1])) {
		nbytes = argc - 3;
		for (i = 0; i < nbytes; i++)
			i2c_transfer[i] = (uint8_t) strtol(argv[i + 3], NULL, 16);
		return try_write(addr, nbytes, i2c_transfer);
	}

	if (argc == 4) return USAGE;

	memAddr = (uint16_t) strtol(argv[3], NULL, 16);

	/* read from a single-byte memory address */
	if (!strcmp("mr1", argv[1])) {
		nbytes = (uint16_t) strtol(argv[4], NULL, 10);
		return try_mem_read(addr, memAddr, 1, nbytes);
	}

	/* write to a single-byte memory address */
	else if (!strcmp("mw1", argv[1])) {
		nbytes = argc - 4;
		for (i = 0; i < nbytes; i++)
			i2c_transfer[i] = (uint8_t) strtol(argv[i + 4], NULL, 16);
		return try_mem_write(addr, memAddr, 1, nbytes, i2c_transfer);
	}

	/* read from a two-byte memory address */
	else if (!strcmp("mr2", argv[1])) {
		nbytes = (uint16_t) strtol(argv[4], NULL, 10);
		return try_mem_read(addr, memAddr, 2, nbytes);
	}

	/* write to a two-byte memory address */
	else if (!strcmp("mw2", argv[1])) {
		nbytes = argc - 4;
		for (i = 0; i < nbytes; i++)
			i2c_transfer[i] = (uint8_t) strtol(argv[i + 4], NULL, 16);
		return try_mem_write(addr, memAddr, 2, nbytes, i2c_transfer);
	}

	return USAGE;
}

COMMAND_ENTRY(
	"i2c",
	"i2c <subcommand> [args...]\r\n\r\n"
	"init\r\n"
	"scan\r\n"
	"dump\r\n"
	"query <addr>\r\n"
	"r <addr> <num_bytes>\r\n"
	"w <addr> <val1 val2 . . .>\r\n"
	"mr1 <addr> <memAddr> <num_bytes>\r\n"
	"mw1 <addr> <memAddr> <val1 val2 . . .>\r\n"
	"mr2 <addr> <memAddr> <num_bytes>\r\n"
	"mw2 <addr> <memAddr> <val1 val2 . . .>\r\n",
	"Interact with the I2C subsystem.",
	do_i2c
)

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
