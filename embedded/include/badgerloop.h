#ifndef _BADGERLOOP__H__
#define _BADGERLOOP__H__

#include <stdint.h>
#include <stdbool.h>
#define SPACEX_IP	1
#define DASHBOARD_IP	10
#define DEV_IP		112

#define TEAM_ID		0x3 //TODO Update
#define CHECK_THRESHOLD(get_macro, upper, lower) \
	(get_macro > upper || get_macro < lower)

/*****************************************************************************/
/*                          Error checking parameters                        */
/*****************************************************************************/
#define VBATT_UPPER	14500 //mV
#define VBATT_LOWER	10500 //mV
#define IBATT_UPPER	13000 //mA
#define IBATT_LOWER 	0	//mA
#define TBATT_UPPER 	500	//0.1 deg c
#define TBATT_LOWER	150	//0.1 deg c

#define BRAKING_TANK_UPPER	3200	//PSI
#define BRAKING_TANK_LOWER	1000	//PSI
#define BRAKING_LINE_OK_UPPER	170	//PSI
#define BRAKING_LINE_OK_LOWER	130	//PSI
#define BRAKING_ON_UPPER	150 	//PSI
#define BRAKING_ON_LOWER	100 	//PSI
#define BRAKING_OFF_UPPER	40	//PSI
#define BRAKING_OFF_LOWER	0	//PSI

#define ACCEL_IDLE_UPPER	100	//cm/s^s
#define ACCEL_IDLE_LOWER	-100	//cm/s^s

#define TARGET_DECEL		-2943

/*****************************************************************************/
/*                          		setters		                     */
/*****************************************************************************/
extern int *acceleration_x,*acceleration_y, *acceleration_z, *velocity, *position, *should_stop;
#define SET_ACCEL_x(val)	*acceleration_x = (val)
#define GET_ACCEL_x		((int) (*acceleration_x))
#define SET_ACCEL_y(val)	*acceleration_y = (val)
#define GET_ACCEL_y		((int) (*acceleration_y))
#define SET_ACCEL_z(val)	*acceleration_z = (val)
#define GET_ACCEL_z		((int) (*acceleration_z))
#define SET_VEL(val)		*velocity = (val)
#define GET_VEL			((int) (*velocity))
#define SET_POS(val)		*position = (val)
#define GET_POS			((int) (*position))
#define SET_SHOULD_STOP(val) 	*should_stop = (val)
#define GET_SHOULD_STOP		((int) (*should_stop))
extern int *v_batt, *i_batt;
#define SET_VBATT(val)		*v_batt = (val)
#define GET_VBATT		((int) (*v_batt))
#define SET_IBATT(val)		*i_batt = (val)
#define GET_IBATT		((int) (*i_batt))
extern int *t_batt, *t_pod;
#define SET_TBATT(val)		*t_batt =(val)
#define GET_TBATT		((int) (*t_batt))
#define SET_TPOD(val)		*t_pod = (val)
#define GET_TPOD		((int) (*t_pod))

extern uint16_t *tube_pressure, *pv_pressure;
#define SET_TUBEPRESSURE(val)	*tube_pressure = (val)
#define GET_TUBEPRESSURE		(*tube_pressure)
#define SET_PVPRESSURE(val)	*pv_pressure = (val)
#define GET_PVPRESSURE			(*pv_pressure)


extern uint16_t * pres_tank_pri, *pres_tank_sec, *pres_line_pri, *pres_line_sec, *pres_actuator_pri, *pres_actuator_sec;
#define SET_PRES_TANK_PRI(val) *pres_tank_pri = (val)
#define GET_PRES_TANK_PRI 	(int) (*pres_tank_pri)
#define SET_PRES_TANK_SEC(val) *pres_tank_sec = (val)
#define GET_PRES_TANK_SEC 	(int) (*pres_tank_sec)
#define SET_PRES_LINE_PRI(val) *pres_line_pri = (val)
#define GET_PRES_LINE_PRI 	(int) (*pres_line_pri)
#define SET_PRES_LINE_SEC(val) *pres_line_sec = (val)
#define GET_PRES_LINE_SEC 	(int) (*pres_line_sec)
#define SET_PRES_ACTU_PRI(val) *pres_actuator_pri = (val)
#define GET_PRES_ACTU_PRI 	(int) (*pres_actuator_pri)
#define SET_PRES_ACTU_SEC(val) *pres_tank_pri = (val)
#define GET_PRES_ACTU_SEC	(int) (*pres_tank_pri)

extern uint8_t *top_retro_count, *mid_retro_count, *bot_retro_count;
#define SET_TOP_RETRO_COUNT(val) *top_retro_count = (val)
#define GET_TOP_RETRO_COUNT(val) (int) (*top_retro_count)
#define SET_MID_RETRO_COUNT(val) *mid_retro_count = (val)
#define GET_MID_RETRO_COUNT(val) (int) (*mid_retro_count)
#define SET_BOT_RETRO_COUNT(val) *bot_retro_count = (val)
#define GET_BOT_RETRO_COUNT(val) (int) (*bot_retro_count)

extern uint8_t *lim_states;
#define BLIM1 0x1
#define SET_BLIM1	*lim_states |= BLIM1
#define CLR_BLIM1	*lim_states &= ~BLIM1
#define GET_BLIM1	(*lim_states & BLIM1)
#define BLIM2 0x2
#define SET_BLIM2	*lim_states |= BLIM2
#define CLR_BLIM2	*lim_states &= ~BLIM2
#define GET_BLIM2	(*lim_states & BLIM2)
#define DLIM 0x4
#define SET_DLIM	*lim_states |= DLIM
#define CLR_DLIM	*lim_states &= ~DLIM
#define GET_DLIM	(*lim_states & DLIM)
#define PVLIM 0x8
#define SET_PVLIM	*lim_states |= PVLIM
#define CLR_PVLIM	*lim_states &= ~PVLIM
#define GET_PVLIM	(*lim_states & PVLIM)

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
#define GET_HV_EN_FDBK		(*shutdown_circuit_status & HV_EN_FDBK_TEL)

/* Actuation Functions */
void primary_brakes(int intensity);
void seconary_brakes(int intensity);
void vent_primary_brakes(bool open);
void vent_secondary_brakes(bool open);

#endif
