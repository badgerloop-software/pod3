#ifndef _I2C__H__
#define _I2C__H__

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
#define I2C_WAITING_RX		0x00000002
#define I2C_WAITING_MEM_TX	0x00000004
#define I2C_WAITING_MEM_RX	0x00000008
#define I2C_ERROR			0x00000010
#define I2C_CLEAR_ERROR		i2c_flags &= ~(I2C_ERROR)
#define I2C_TIMEOUT_CHK		(ticks - ticks_start < BLOOP_I2C_TIMEOUT)

void print_i2c_error(I2C_HandleTypeDef *handle);
const char *i2c_state_str(HAL_I2C_StateTypeDef value);
void print_i2c_state(I2C_HandleTypeDef *handle);

#endif
