#ifndef _BMS_H__
#define _BMS_H__

#include <stdint.h>
#include <stdbool.h>
#include "system.h"

/*****************************************************************************/
/*                                 BMS Enums                                 */
/*****************************************************************************/

typedef enum {
	RELAY_STATUS = 0xF004
	CELL_COUNT = 0xF007
	CHARGE_CURR_LIM = 0XF00A
	DISCHARG_CURR_LIM = 0X00B
	CURR_PACK = 0XF00C
	VOLT_PACK = 0XF00D
	OPEN_VOLT_PACK = 0XF00E
	STATE_OF_CHARGE_PACK = 0XF00F
	AMP_HRS_PACK = 0XF010
	RESISTANCE_PACK = 0XF011
	DEPTH_OF_DISCHARGE_PACK = 0XF012
	HEALTH_PACK = 0XF013
	SUMMED_VOLTAGE_PACK = 0XF014
	LV_BATT_IN = 0XF046

	TEMP_HIGH_PACK = 0XF028
	TEMP_LOW_PACK = 0XF029
	TEMP_AVG_PACK = 0XF02a

	VOLT_LOW_CELL = 0XF032
	VOLT_LOW_CELL_NUM = 0XF03E
	VOLT_HIGH_CELL = 0XF033
	VOLT_HIGH_CELL_NUM = 0XF03D
	VOLT_AVG_CELL =0XF034
	VOLT_CELL_1_12 = 0XF100
	VOLT_CELL_13_24 = 0XF101
	VOLT_CELL_25_36 = 0XF102
	VOLT_CELL_37_48 = 0XF103
        VOLT_CELL_49_60 = 0XF104
	VOLT_CELL_61_72 = 0XF105	

	RES_LOW_CELL = 0XF038
	RES_LOW_CELL_NUM = 0XF042
	RES_HIGH_CELL = 0XF039
	RES_HIGH_CELL_NUM = 0XF041
	RES_AVG_CELL = 0XF03A
	RES_CELL_1_12 = 0XF200
	RES_CELL_13_24 = 0XF201
	RES_CELL_25_36 = 0XF202
	RES_CELL_37_48 = 0XF203
        RES_CELL_49_60 = 0XF204
	RES_CELL_61_72 = 0XF205	

	
}

/*                                  Getters                                  */
int bms_getRelayStatus(void);
int bms_getCellCount(void);
int bms_getChargeCurrLimit(void);
int bms_getDischargeCurrLimit(void);
int bms_getCurrPack(void);
int bms_getVoltagePack(void);
int bms_getOpenVoltagePack(void);
int bms_getStateOfChargePack(void);
int bms_getAmpHrsPack(void);
int bms_getResistancePack(void);
int bms_getDepthDischargePack(void);
int bms_getHealthPack(void);
int bms_getSummedVoltagePack(void);
int bms_getTempHighPack(void);
int bms_getLvBatIn(void);

int bms_getHighTemp(void);
int bms_getLowTemp(void);
int bms_getAvgTemp(void);

int bms_getVoltLowCell(void);
int bms_getVoltLowCellNum(void);
int bms_getVoltHighCell(void);
int bms_getVoltHighCellNum(void);
int bms_getVoltAvgCell(void);

int bms_getResCell1_12(void);
int bms_getResCell13_24(void);
int bms_getResCell25_36(void);
int bms_getResCell37_48(void);
int bms_getResCell49_60(void);
int bms_getResCell61_72(void);

int bms_getResLowCell(void);
int bms_getResLowCellNum(void);
int bms_getResHighCell(void);
int bms_getResHighCellNum(void);
int bms_getResAvgCell(void);

int bms_getResCell1_12(void);
int bms_getResCell13_24(void);
int bms_getResCell25_36(void);
int bms_getResCell37_48(void);
int bms_getResCell49_60(void);
int bms_getResCell61_72(void);

