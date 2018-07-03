#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "commands.h"
#include "stm32l4xx_hal_conf.h"
#include "i2c.h"
#include "iox.h"

command_status do_init(void) {
	return (i2c_init() == HAL_OK) ? CMD_SUCCESS : FAIL;
}

command_status try_query(uint8_t addr) {
	HAL_StatusTypeDef retval = i2c_query(addr);
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
	i2c_dump();
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
	if (i2c_start_write(addr, nbytes, data) != HAL_OK)
		return FAIL;
	if (i2c_block(I2C_WAITING_TX, ticks_start) || i2c_errors_present())
		return FAIL;
	return CMD_SUCCESS;
}

command_status try_read(uint8_t addr, uint16_t nbytes) {
	uint16_t i;
	uint32_t ticks_start = ticks;
	if (i2c_start_read(addr, nbytes) != HAL_OK)
		return FAIL;
	if (i2c_block(I2C_WAITING_RX, ticks_start) || i2c_errors_present())
		return FAIL;
	for (i = 0; i < nbytes; i++) printf("%d: 0x%x\r\n", i, i2c_rx[i]);
	return CMD_SUCCESS;
}

command_status try_mem_read(
	uint8_t addr, uint16_t memAddr, uint16_t memAddrSize, uint16_t nbytes
) {
	uint16_t i;
	uint32_t ticks_start = ticks;
	if (i2c_start_mem_read(addr, memAddr, memAddrSize, nbytes) != HAL_OK)
		return FAIL;
	if (i2c_block(I2C_WAITING_MEM_RX, ticks_start) || i2c_errors_present())
		return FAIL;
	for (i = 0; i < nbytes; i++) printf("%d: 0x%x\r\n", i, i2c_rx[i]);
	return CMD_SUCCESS;
}

command_status try_mem_write(
	uint8_t addr, uint16_t memAddr, uint16_t memAddrSize, uint16_t nbytes,
	const uint8_t *data
) {
	uint32_t ticks_start = ticks;
	if (i2c_start_mem_write(
		addr, memAddr, memAddrSize, nbytes, data
	) != HAL_OK) return FAIL;
	if (i2c_block(I2C_WAITING_MEM_TX, ticks_start) || i2c_errors_present())
		return FAIL;
	return CMD_SUCCESS;
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
	"query <addr (hex)>\r\n"
	"r <addr (hex)> <num_bytes (int)>\r\n"
	"w <addr (hex)> <val1 val2 . . . (hex)>\r\n"
	"mr1 <addr (hex)> <memAddr (hex)> <num_bytes (int)>\r\n"
	"mw1 <addr (hex)> <memAddr (hex)> <val1 val2 . . . (hex)>\r\n"
	"mr2 <addr (hex)> <memAddr (hex)> <num_bytes (int)>\r\n"
	"mw2 <addr (hex)> <memAddr (hex)> <val1 val2 . . . (hex)>\r\n",
	"Interact with the I2C subsystem.",
	do_i2c
)

command_status do_iox(int argc, char *argv[]) {
	iox_pin_t pin;
	uint8_t read_val;

	if (argc == 1) return USAGE;

	if (!strcmp("read", argv[1])) {
		if (!iox_start_read()) {
			printf("%s: couldn't read from i2c:\r\n", __func__);
			i2c_dump();
			return FAIL;
		}
		if (i2c_block(I2C_WAITING_RX, ticks)) {
			printf("%s: waiting for read timed out\r\n", __func__);
			i2c_dump();
			return FAIL;
		}
		if (!iox_read(&read_val)) {
			printf("%s: call to 'iox_read' failed\r\n", __func__);
			i2c_dump();
			return FAIL;
		}
		printf("hex value read: 0x%x\r\n", read_val);
		iox_dump();
		return CMD_SUCCESS;
	} else if (!strcmp("dump", argv[1])) {
		iox_dump();
		return CMD_SUCCESS;
	}

	if (argc == 2) return USAGE;

	/* validate pin input */
	pin = str_to_pin(argv[2]);
	if (pin == IOX_INVALID) {
		pin = (iox_pin_t) (argv[2][0] - 'a');
		if (pin >= IOX_INVALID) {
			printf("Invalid pin valid: %d\r\n", pin);
			return FAIL;
		}
	}

	if (!strcmp("set", argv[1]))
		return iox_set(pin) ? CMD_SUCCESS : FAIL;
	else if (!strcmp("clear", argv[1]))
		return iox_clear(pin) ? CMD_SUCCESS: FAIL;

	return USAGE;
}

COMMAND_ENTRY(
	"iox",
	"iox {read|dump|set <pin_alias (int or str)>|clear <pin_alias (int or str)>}",
	"Interact with the PCF8574 I/O Expander.",
	do_iox
)
