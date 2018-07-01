#ifndef _I2C__H__
#define _I2C__H__

#include <stdbool.h>
#include "stm32l4xx_hal_conf.h"

extern volatile uint32_t ticks;

#define BLOOP_I2C_TIMEOUT	50
#define I2C_BUF_SIZE		64

extern I2C_HandleTypeDef i2c_handle;
extern uint8_t i2c_rx[I2C_BUF_SIZE];
extern uint8_t i2c_tx[I2C_BUF_SIZE];
extern uint8_t i2c_transfer[I2C_BUF_SIZE];

extern volatile uint32_t i2c_flags;
#define I2C_WAITING_TX		0x00000001
#define I2C_TX_READY		0x00000002
#define I2C_WAITING_RX		0x00000004
#define I2C_RX_READY		0x00000008
#define I2C_WAITING_MEM_TX	0x00000010
#define I2C_MEM_TX_READY	0x00000020
#define I2C_WAITING_MEM_RX	0x00000040
#define I2C_MEM_RX_READY	0x00000080
#define I2C_ERROR			0x00000100
#define I2C_CLEAR_ERROR		i2c_flags &= ~(I2C_ERROR)
#define I2C_TIMEOUT_CHK		(ticks - ticks_start < BLOOP_I2C_TIMEOUT)

/*****************************************************************************/

HAL_StatusTypeDef i2c_init(void);

HAL_StatusTypeDef i2c_query(uint8_t addr);

HAL_StatusTypeDef i2c_start_write(
	uint8_t addr, uint16_t nbytes, const uint8_t *data
);
bool i2c_write_ready(void);

HAL_StatusTypeDef i2c_start_read(uint8_t addr, uint16_t nbytes);
bool i2c_read_ready(void);

HAL_StatusTypeDef i2c_start_mem_write(
	uint8_t addr, uint16_t memAddr, uint16_t memAddrSize,
	uint16_t nbytes, const uint8_t *data
);
bool i2c_mem_write_ready(void);

HAL_StatusTypeDef i2c_start_mem_read(
	uint8_t addr, uint16_t memAddr, uint16_t memAddrSize, uint16_t nbytes
);
bool i2c_mem_read_ready(void);

int i2c_block(uint32_t flag, uint32_t ticks_start);

void i2c_dump(void);

bool i2c_errors_present(void);

/*****************************************************************************/

void print_i2c_error(I2C_HandleTypeDef *handle);
const char *i2c_state_str(HAL_I2C_StateTypeDef value);
void print_i2c_state(I2C_HandleTypeDef *handle);

#endif
