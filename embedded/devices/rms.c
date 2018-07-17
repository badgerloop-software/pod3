#include <stdio.h>
#include <stdlib.h>
#include "rms.h"

Rms *rms;

void rms_init() {
	rms = malloc(sizeof(Rms));
	rms->igbt_temp = 0;
	rms->gate_driver_board_temp = 0;
	rms->control_board_temp = 0;
	rms->motor_temp = 0;
	rms->motor_speed = 0;
	rms->phase_a_current = 0;
	rms->phase_b_current = 0;
	rms->phase_c_current = 0;	
	rms->dc_bus_voltage=0;
	rms->output_voltage_peak = 0;
	rms->lv_voltage = 0;
	rms->can_code_1 = 0;
	rms->can_code_2 = 0;
	rms->fault_code_1 = 0;
	rms->fault_code_2 = 0;
	rms->commanded_torque = 0;
	rms->actual_torque = 0;
	rms->relay_state = 0;	
}

int rms_parser(uint32_t id, uint8_t *data){
	printf("received RMS message\r\n");
	switch(id){
		case (0xa0):
			rms->igbt_temp = data[1] | data[0] <<8;
			rms->gate_driver_board_temp = data[7] | data[6] << 8;
			break;
		case (0xa1):
			rms->control_board_temp = data[1] | data[0] << 8;
			break;
		case (0xa2):
			rms->motor_temp = data[5] | data[4] << 8;
			break;

		case (0xa5):
			rms->motor_speed = data[3] | data[2];
			break;
		case (0xa6):
			rms->phase_a_current = data[7] | data[6] << 8;
			break;
		case (0xa7):
			rms->dc_bus_voltage = data[1] | data[0] << 8;
			rms->output_voltage_peak = data[3] | data[2] << 8;
			break;
		case (0xa8):
			break;
		case (0xa9):
			rms->lv_voltage = data[7] | data[6];
			break;

		case (0xaa):
			rms->can_code_1 = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
			rms->can_code_2 = (data[4] << 24) | (data[5] << 16) | (data[6] << 8) | data[7];
			break;
		case (0xab):
			rms->fault_code_1 = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
			rms->fault_code_2 = (data[4] << 24) | (data[5] << 16) | (data[6] << 8) | data[7];
			break;
	
		case (0xac):
			rms->commanded_torque = data[1] | data[0];
			rms->actual_torque = data[3] | data[2];
			break;
		case (0xad):
			break;
	}
	return 1;

}
