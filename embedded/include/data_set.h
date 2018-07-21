#ifndef _DATA_SET__H__
#define _DATA_SET__H__

/* Telemetry data setup functions */
void nav_tape_set(uint8_t data[]);
void nav_should_stop_set(uint8_t data[]);
void nav_adc_set(uint8_t data[]);
void nav_pressure1_set(uint8_t data[]);
void nav_pressure2_set(uint8_t data[]);
void nav_pressure3_set(uint8_t data[]);
void nav_pressure4_set(uint8_t data[]);
void nav_pressure5_set(uint8_t data[]);
void nav_solenoid1_set(uint8_t data[]);
void nav_solenoid2_set(uint8_t data[]);
void nav_accel_vel_pos_set(uint8_t data[]);
void nav_post_run_set(uint8_t data[]);
void state_message_set(uint8_t data[]);

void pv_pressure_set(uint8_t data[]);
void pv_shutdown_set(uint8_t data[]);

#endif
