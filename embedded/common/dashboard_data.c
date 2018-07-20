#include <stdlib.h>
#include <string.h>
#include "dashboard_data.h"
#include "uart.h"
#include "honeywell.h"
#include "i2c.h"
#include <time.h>
#include "can.h"
#include "rms.h"

#define SEND_BUF_SIZE 128

char packetBuffer[SEND_BUF_SIZE];

int dash_DAQ(Pod_Data_Handle *podData) {
//	ccp_parse_can_message(BADGER_CAN_ID, RxData, podData);	
	harvest_honeywell(podData);

	return 0;
}

void set_retro(Pod_Data_Handle *podData, uint8_t retroVal) {
	printf("RETRO: %u\r\n", retroVal);
	podData->retro.ui8data = retroVal;
	podData->retro.timestamp = time(NULL);
	podData->retro.freshness = FRESH;
}

void set_curr_adc(Pod_Data_Handle *podData, uint16_t val) {
	//printf("RETRO: %u\r\n", retroVal);
	podData->adc[0].ui16data = val;
	podData->adc[0].timestamp = time(NULL);
	podData->adc[0].freshness = FRESH;
}

void set_limit(Pod_Data_Handle *podData, uint8_t lim1, uint8_t lim2, uint8_t lim3) {
	//printf("RETRO: %u\r\n", retroVal);
	podData->limit[0].ui8data = lim1;
	podData->limit[0].timestamp = time(NULL);
	podData->limit[0].freshness = FRESH;
	
    podData->limit[1].ui8data = lim2;
	podData->limit[1].timestamp = time(NULL);
	podData->limit[1].freshness = FRESH;
	
    podData->limit[2].ui8data = lim3;
	podData->limit[2].timestamp = time(NULL);
	podData->limit[2].freshness = FRESH;
}
void set_accel_vel_pos(Pod_Data_Handle *podData, int8_t accel, int8_t vel, int8_t pos) {
	podData->position.i8data = pos;
	podData->position.freshness = FRESH;
	podData->position.timestamp = time(NULL);

	podData->velocity.i8data = vel;
	podData->velocity.freshness = FRESH;
	podData->velocity.timestamp = time(NULL);

	podData->acceleration.i8data = accel;
	podData->acceleration.freshness = FRESH;
	podData->acceleration.timestamp = time(NULL);
}


void set_stopping_dist(Pod_Data_Handle *podData) {
	podData->stopping_dist.ui16data = podData->stopping_dist.ui16data - (100 * podData->retro.ui8data);	
}

void set_solenoid_value(Pod_Data_Handle *podData, uint8_t solenoidsVal) {
	podData->solenoids.ui8data = solenoidsVal;
	podData->solenoids.timestamp = time(NULL);
	podData->solenoids.freshness = FRESH;
}

void set_pres_1_2(Pod_Data_Handle* podData, uint16_t pres1, uint16_t pres2) {
		podData->linePressures[0].ui16data = pres1;
		podData->linePressures[0].timestamp = time(NULL);
		podData->linePressures[0].freshness = FRESH;

		podData->linePressures[1].ui16data = pres2;
		podData->linePressures[1].timestamp = time(NULL);
		podData->linePressures[1].freshness = FRESH;
};

void set_pres_3_4(Pod_Data_Handle* podData, uint16_t pres1, uint16_t pres2) {
		podData->linePressures[2].ui16data = pres1;
		podData->linePressures[2].timestamp = time(NULL);
		podData->linePressures[2].freshness = FRESH;

		podData->linePressures[3].ui16data = pres2;
		podData->linePressures[3].timestamp = time(NULL);
		podData->linePressures[3].freshness = FRESH;
};

void set_pres_5_6(Pod_Data_Handle* podData, uint16_t pres1, uint16_t pres2) {
		podData->linePressures[4].ui16data = pres1;
		podData->linePressures[4].timestamp = time(NULL);
		podData->linePressures[4].freshness = FRESH;

		podData->linePressures[5].ui16data = pres2;
		podData->linePressures[5].timestamp = time(NULL);
		podData->linePressures[5].freshness = FRESH;
};


void package_bms_data(Pod_Data_Handle *podData, Bms *bms) {
	podData->BMSdata[0].ui16data  = (uint16_t) (bms->packCurrent * 1000);
	podData->BMSdata[1].ui16data = (uint16_t) (bms->packVoltage);
	podData->BMSdata[2].ui16data = bms->packDCL;
	podData->BMSdata[3].ui16data = bms->packCCL;
	podData->BMSdata[4].ui16data = bms->packResistance;
	podData->BMSdata[5].ui8data = bms->packHealth;
	podData->BMSdata[6].ui16data = (uint8_t) (bms->packOpenVoltage * 1000);
	podData->BMSdata[7].ui16data = bms->packCycles;
	podData->BMSdata[8].ui16data = bms->packAh;
	podData->BMSdata[9].ui8data = (uint8_t) (bms->inputVoltage * 1000);
	podData->BMSdata[10].ui8data = bms->Soc;
	podData->BMSdata[11].ui16data = bms->relayStatus;
	podData->BMSdata[12].ui8data = bms->highTemp;
	podData->BMSdata[13].ui8data = bms->lowTemp;
	podData->BMSdata[14].ui16data = bms->cellMaxVoltage;
	podData->BMSdata[15].ui16data = bms->cellMinVoltage;
	podData->BMSdata[16].ui16data = bms->cellAvgVoltage;
	podData->BMSdata[17].ui8data = bms->maxCells;
	podData->BMSdata[18].ui8data = bms->numCells;

	int i;
	for (i = 0; i < 19; i++) {
		podData->BMSdata[i].freshness = FRESH;
	}
}

void package_rms_data(Pod_Data_Handle *podData, Rms *rms) {
	podData->RMSdata[0].ui16data = rms->igbt_temp;
	podData->RMSdata[1].ui16data = rms->gate_driver_board_temp;
	podData->RMSdata[2].ui16data = rms->control_board_temp;
	podData->RMSdata[3].ui16data = rms->motor_temp;
	podData->RMSdata[4].ui16data = rms->motor_speed;
	podData->RMSdata[5].ui16data = rms->phase_a_current;
	podData->RMSdata[6].ui16data = rms->phase_b_current;
	podData->RMSdata[7].ui16data = rms->phase_c_current;
	podData->RMSdata[8].ui16data = rms->dc_bus_voltage;
//	podData->RMSdata[9].ui16data = rms->ouput_voltage_peak;
	podData->RMSdata[10].ui16data = rms->lv_voltage;

	podData->RMSdata[11].ui16data = 0xFFFF & (rms->can_code_1 >> 16);
	podData->RMSdata[12].ui16data = 0xFFFF & (rms->can_code_1);
	podData->RMSdata[13].ui16data = 0xFFFF & (rms->can_code_2 >> 16);
	podData->RMSdata[14].ui16data = 0xFFFF & (rms->can_code_2);
	podData->RMSdata[15].ui16data = 0xFFFF & (rms->fault_code_1 >> 16);
	podData->RMSdata[16].ui16data = 0xFFFF & (rms->fault_code_1);
	podData->RMSdata[17].ui16data = 0xFFFF & (rms->fault_code_2 >> 16);
	podData->RMSdata[18].ui16data = 0xFFFF & (rms->fault_code_2);

	podData->RMSdata[19].ui16data = rms->commanded_torque;
	podData->RMSdata[20].ui16data = rms->actual_torque;
	podData->RMSdata[21].ui16data = rms->relay_state;

	int i;
	for (i = 0; i < 22; i++) {
		podData->RMSdata[i].freshness = FRESH;
	}
}

void set_pv_honeywell(Pod_Data_Handle *podData, uint16_t pres, uint16_t temp) {
	podData->pv_temp.ui16data  = temp;
	podData->pv_temp.freshness = FRESH;
	podData->pv_temp.timestamp = time(NULL);

	podData->pv_pres.ui16data = pres;
	podData->pv_pres.freshness = FRESH;
	podData->pv_pres.ui16data  = time(NULL);

}

void send_data(Pod_Data_Handle *pod_data) {
	Sensor_Data *sensor;
	int i;
    
    if (pod_data->current_pressure.freshness == FRESH) {
		pod_data->current_pressure.freshness = NOT_FRESH;
		sensor = &(pod_data->current_pressure);
		char *dataToSend = formatPacket(sensor);
		/*printf("%s", dataToSend);*/
		uart_send(dataToSend);
	}

	if (pod_data->lv_battery_temp.freshness == FRESH) {
		pod_data->lv_battery_temp.freshness = NOT_FRESH;
		sensor = &(pod_data->lv_battery_temp);
		char *dataToSend = formatPacket(sensor);
		//printf("%s\r\n", dataToSend);
		uart_send(dataToSend);
	}

	if (pod_data->retro.freshness == FRESH) {
		printf("Retro send\r\n");
		pod_data->retro.freshness = NOT_FRESH;
		char *dataToSend = formatPacket(&(pod_data->retro));
		uart_send(dataToSend);
	}

	if (pod_data->solenoids.freshness == FRESH) {
		pod_data->solenoids.freshness = NOT_FRESH;
		Sensor_Data sol1 = {"solenoid_1", 0, (pod_data->solenoids.ui8data & 0x1), 0, 0, 0, DT_UINT8};
		char *sol1str = formatPacket(&sol1);
		Sensor_Data sol2 = {"solenoid_2", 0, (pod_data->solenoids.ui8data & 0x2) >> 1, 0, 0, 0, DT_UINT8};
		char *sol2str = formatPacket(&sol2);
		Sensor_Data sol4 = {"solenoid_4", 0, (pod_data->solenoids.ui8data & 0x8) >> 3, 0, 0, 0, DT_UINT8};
		char *sol4str = formatPacket(&sol4);
		Sensor_Data sol5 = {"solenoid_5", 0, (pod_data->solenoids.ui8data & 0x20) >> 5, 0, 0, 0, DT_UINT8};
		char *sol5str = formatPacket(&sol5);
		Sensor_Data sol6 = {"solenoid_6", 0, (pod_data->solenoids.ui8data & 0x40) >> 6, 0, 0, 0, DT_UINT8};
		char *sol6str = formatPacket(&sol6);
		uart_send(sol1str);
		uart_send(sol2str);
		uart_send(sol4str);
		uart_send(sol5str);
		uart_send(sol6str);
	}

	if (pod_data->position.freshness == FRESH) {
		pod_data->position.freshness = NOT_FRESH;
		uart_send(formatPacket(&(pod_data->position)));
	}

	if (pod_data->velocity.freshness == FRESH) {
		pod_data->velocity.freshness = NOT_FRESH;
		uart_send(formatPacket(&(pod_data->velocity)));
	}

	if (pod_data->acceleration.freshness == FRESH) {
		pod_data->acceleration.freshness = NOT_FRESH;
		uart_send(formatPacket(&(pod_data->acceleration)));
	}

	if (pod_data->pv_pres.freshness == FRESH) {
		pod_data->pv_pres.freshness = NOT_FRESH;
		uart_send(formatPacket(&(pod_data->pv_pres)));
	}

	if (pod_data->pv_temp.freshness == FRESH) {
		pod_data->pv_temp.freshness = NOT_FRESH;
		uart_send(formatPacket(&(pod_data->pv_temp)));
	}

	
	for (i = 0; i < 6; i++){
		if (pod_data->linePressures[i].freshness == FRESH) {
			pod_data->linePressures[i].freshness = NOT_FRESH;
			uart_send(formatPacket(&(pod_data->linePressures[i])));
		}
	}

    for (i = 0; i < 2; i++) {
		if (pod_data->adc[i].freshness == FRESH) {
			pod_data->adc[i].freshness = NOT_FRESH;
			uart_send(formatPacket(&(pod_data->adc[i])));
		}
	}

    for (i = 0; i < 3; i++) {
		if (pod_data->limit[i].freshness == FRESH) {
			pod_data->limit[i].freshness = NOT_FRESH;
			uart_send(formatPacket(&(pod_data->limit[i])));
		}
	}

    for (i = 0; i < 19; i++) {
		if (pod_data->BMSdata[i].freshness == FRESH) {
			pod_data->BMSdata[i].freshness = NOT_FRESH;
			uart_send(formatPacket(&(pod_data->BMSdata[i])));
		}
	}
	for (i = 0; i < 22; i++) {
		if (pod_data->RMSdata[i].freshness == FRESH) {
			pod_data->RMSdata[i].freshness = NOT_FRESH;
			uart_send(formatPacket(&(pod_data->RMSdata[i])));
		}
	}
}

void harvest_honeywell(Pod_Data_Handle *pod_data) {
	int pres;
	int temp;
	if (honeywell_start_read()) {
	   	if (!i2c_block(I2C_WAITING_RX, ticks)) {
			if (honeywell_read(&temp, &pres)) {
				pod_data->lv_battery_temp.freshness = FRESH;
				pod_data->lv_battery_temp.timestamp = time(NULL);
				pod_data->lv_battery_temp.i8data    = (int8_t) temp;
				
				pod_data->current_pressure.freshness = FRESH;
				pod_data->current_pressure.timestamp = time(NULL);
				pod_data->current_pressure.ui16data  = (uint16_t) pres;
			} else printf("honeywell read fail\r\n");
		} else printf("failure to block\r\n");
	} else printf("honeywell start read fail\r\n");
}

char *formatPacket(Sensor_Data *sensorData) {
	char *sensName;
	uint8_t u8sensData;
	uint16_t u16sensData;
	int8_t i8sensData;
	int nChars;
	/* grab the values we want to send */
	sensName = sensorData->name;
        /* print the content to the buffer */
	/* the format string will be different by type */
	nChars = -1; 	// To indicate an error
	switch(sensorData->dtype) {
		case DT_UINT8:
			u8sensData = sensorData->ui8data;
			nChars = snprintf(packetBuffer, SEND_BUF_SIZE, "$|%s,%u\n", sensName, u8sensData);
			break;
		case DT_UINT16:
			u16sensData = sensorData->ui16data;
			nChars = snprintf(packetBuffer, SEND_BUF_SIZE, "$|%s,%u\n", sensName, u16sensData);
			break;

		case DT_INT8:
			i8sensData = sensorData->i8data;
			nChars = snprintf(packetBuffer, SEND_BUF_SIZE, "$|%s,%d\n", sensName, i8sensData);
			break;
	}

	if (nChars < 0 || nChars > SEND_BUF_SIZE) {
	        /* TODO this means our print failed or was too big */
	        /* TODO should do something smarter here */
	        printf("Problem encountered while creating sensor data payload\r\n");
	}
	return packetBuffer;
}
