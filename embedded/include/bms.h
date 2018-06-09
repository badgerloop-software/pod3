#ifndef _BMS_H__
#define _BMS_H__

#include <stdint.h>
#include <stdbool.h>
#include "system.h"

/*****************************************************************************/
/*                                 BMS Enums                                 */
/*****************************************************************************/
/*
typedef struct {
	uint64_t RELAY_STATUS = 0xF004;
	uint64_t CELL_COUNT = 0xF007;
	uint64_t CHARGE_CURR_LIM = 0XF00A;
	uint64_t DISCHARG_CURR_LIM = 0X00B;
	uint64_t CURR_PACK = 0XF00C;
	uint64_t VOLT_PACK = 0XF00D;
	uint64_t OPEN_VOLT_PACK = 0XF00E;
	uint64_t STATE_OF_CHARGE_PACK = 0XF00F;
	uint64_t AMP_HRS_PACK = 0XF010;
	uint64_t RESISTANCE_PACK = 0XF011;
	uint64_t DEPTH_OF_DISCHARGE_PACK = 0XF012;
	uint64_t HEALTH_PACK = 0XF013;
	uint64_t SUMMED_VOLTAGE_PACK = 0XF014;
	uint64_t LV_BATT_IN = 0XF046;

	uint64_t TEMP_HIGH_PACK = 0XF028;
	uint64_t TEMP_LOW_PACK = 0XF029;
	uint64_t TEMP_AVG_PACK = 0XF02a;

	uint64_t VOLT_LOW_CELL = 0XF032;
	uint64_t VOLT_LOW_CELL_NUM = 0XF03E;
	uint64_t VOLT_HIGH_CELL = 0XF033;
	uint64_t VOLT_HIGH_CELL_NUM = 0XF03D;
	uint64_t VOLT_AVG_CELL =0XF034;
	uint64_t VOLT_CELL_1_12 = 0XF100;
	uint64_t VOLT_CELL_13_24 = 0XF101;
	uint64_t VOLT_CELL_25_36 = 0XF102;
	uint64_t VOLT_CELL_37_48 = 0XF103;
        uint64_t VOLT_CELL_49_60 = 0XF104;
	uint64_t VOLT_CELL_61_72 = 0XF105;	

	uint64_t RES_LOW_CELL = 0XF038;
	uint64_t RES_LOW_CELL_NUM = 0XF042;
	uint64_t RES_HIGH_CELL = 0XF039;
	uint64_t RES_HIGH_CELL_NUM = 0XF041;
	uint64_t RES_AVG_CELL = 0XF03A;
	uint64_t RES_CELL_1_12 = 0XF200;
	uint64_t RES_CELL_13_24 = 0XF201;
	uint64_t RES_CELL_25_36 = 0XF202;
	uint64_t RES_CELL_37_48 = 0XF203;
        uint64_t RES_CELL_49_60 = 0XF204;
	uint64_t RES_CELL_61_72 = 0XF205;	

	
}BMS_QUERY;
*/

enum BMS_QUERY {
	RELAY_STATUS = 0xF004,
	CELL_COUNT = 0xF007,
};

uint8_t  bms_can_send(uint64_t address);
uint8_t bms_getRelayStatus(void);
uint8_t  bms_getPackVoltOpenCircuit(void);
uint8_t  bms_getPackVoltHigh(void);
uint8_t  bms_getPackVoltHighNum(void);
uint8_t  bms_getPackVoltLow(void);
uint8_t  bms_getPackVoltLowNum(void);
uint8_t  bms_getPackVoltCellAvg(void);
uint8_t  bms_getCellVoltAll(void);
uint8_t  bms_getPackRes(void);
uint8_t  bms_getCellTempLowNum(void);
uint8_t  bms_getCellTempAvg(void);
uint8_t  bms_getCellTempAll(void);
uint8_t  bms_getRelayState(void);
uint8_t  bms_getCellCount(void);
uint8_t  bms_getChargeCurrLimit(void);
uint8_t  bms_getDischargeCurrLimit(void);
uint8_t  bms_getPackCurrent(void);
uint8_t  bms_getStateOfCharge(void);
uint8_t  bms_getAmpHours(void);
uint8_t  bms_getDepthOfDischarge(void);
uint8_t  bms_getPackHealth(void);
uint8_t  bms_getCellResHigh(void);
uint8_t  bms_getCellResLowNum(void);
uint8_t  bms_getCellResHighNum(void);
uint8_t  bms_getCellResLow(void);
uint8_t  bms_getCellResAvg(void);
uint8_t  bms_getCellResAll(void);
uint8_t  bms_getCellTempHigh(void);
uint8_t  bms_getCellTempLow(void);
uint8_t  bms_getPackTempAll(void);
#endif
