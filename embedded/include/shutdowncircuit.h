#ifndef _SHUTDOWNCIRCUIT__H__
#define _SHUTDOWNCIRCUIT__H__

void mcu_toggle_shutdowncircuit(int state);
void motor_controller_toggle_power(int state);

extern uint8_t *shutdown_circuit_status;

#define MSTR_SW_FDBK_TEL 0x1
#define SET_MSTR_SW_FDBK	*shutdown_circuit_status |= MSTR_SW_FDBK_TEL
#define CLR_MSTR_SW_FDBK	*shutdown_circuit_status &= ~MSTR_SW_FDBK_TEL
#define GET_MSTR_SW_FDBK	(*shutdown_circuit_status & MSTR_SW_FDBK_TEL)
#define E_STOP_FDBK_TEL 0x2
#define SET_E_STOP_FDBK		*shutdown_circuit_status |= E_STOP_FDBK_TEL
#define CLR_E_STOP_FDBK		*shutdown_circuit_status &= ~E_STOP_FDBK_TEL
#define GET_E_STOP_FDBK		(*shutdown_circuit_status & E_STOP_FDBK_TEL)
#define PV_LIM_FDBK_TEL 0x4
#define SET_PV_LIM_FDBK		*shutdown_circuit_status |=PV_LIM_FDBK_TEL
#define CLR_PV_LIM_FDBK		*shutdown_circuit_status &= ~PV_LIM_FDBK_TEL
#define GET_PV_LIM_FDBK		(*shutdown_circuit_status & PV_LIM_FDBK_TEL)
#define HVD_FDBK_TEL 0x8
#define SET_HVD_FDBK		*shutdown_circuit_status |= HVD_FDBK_TEL
#define CLR_HVD_FDBK		*shutdown_circuit_status &= ~HVD_FDBK_TEL
#define GET_HVD_FDBK		(*shutdown_circuit_status & HVD_FDBK_TEL)
#define BMS_STAT_FDBK_TEL 0x10
#define SET_BMS_STAT_FDBK	*shutdown_circuit_status |= BMS_STAT_FDBK_TEL
#define CLR_BMS_STAT_FDBK	*shutdown_circuit_status &= ~BMS_STAT_FDBK_TEL
#define GET_BMS_STAT_FDK	(*shutdown_circuit_statuss & BMS_STAT_FDBK_TEL)
#define IMD_STAT_FDBK_TEL 0x20
#define SET_IMD_STAT_FDBK	*shutdown_circuit_status |= IMD_STAT_FDBK_TEL
#define CLR_IMD_STAT_FDBK	*shutdown_circuit_status &= ~IMD_STAT_FDBK_TEL
#define GET_IMD_STAT_FDBK	(*shutdown_circuit_status & IMD_STAT_FDBK_TEL)
#define HV_EN_FDBK_TEL 0x40
#define SET_HV_EN_FDBK		*shutdown_circuit_status |= HV_EN_FDBK_TEL
#define CLR_HV_EN_FDBK		*shutdown_circuit_status &= ~HV_EN_FDBK_TEL
#define GET_HV_EN_FDBK	(*shutdown_circuit_status & HV_EN_FDBK_TEL)endif
