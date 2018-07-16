#include "retro.h"
#include "nav_data.h"
#include "exti.h"
extern Nav_Data navData;

/*
 * Message Type:
 *      Updates data with current voted-on retro count
 *
 * TO:   CCP
 * FROM: NAV
 *
 * Data Byte 2: Voted Retro Count
 * Data Byte 3: Lim 1
 * Data Byte 4: Lim 2
 * Data Byte 5: Lim 3
 * Data Byte 6: 0
 * Data Byte 7: 0 */
void nav_tape_set(uint8_t data[]){
    int i;
    //TODO: Do error checking with badRetro
    data[2] = navData.retros.retroAgreement;
    //data[3] = interLine[3].count;
    for ( i = 3; i < 8; i++ ){
        data[i] = 0;
    }
    
    return;
}

/*
 * Message Type: 
 *      Updates data with current status whether we should stop or not
 *
 * TO:   CCP
 * FROM: NAV
 *
 * Data Byte 2: Should Stop
 * Data Byte 3: TODO: Staleness?
 * Data Byte 4: 0
 * Data Byte 5: 0
 * Data Byte 6: 0
 * Data Byte 7: 0 */
void nav_should_stop_set(uint8_t data[]){
    int i;

    data[2] = navData.shouldStop;

    for ( i = 3; i < 8; i++ ){
        data[i] = 0;
    }
    
    return;
}

/*
 * Message Type: 
 *      Updates data with latest value from Pressure Sensor 1
 * TO:   CCP
 * FROM: NAV
 *
 * Data Byte 2: Pressure 1 [7:0]
 * Data Byte 3: Pressure 1 [15:8]
 * Data Byte 4: Pressure 2 [7:0]
 * Data Byte 5: Pressure 2 [15:8]
 * Data Byte 6: TODO: Staleness??
 * Data Byte 7: 0 */
void nav_pressure1_set(uint8_t data[]){

    data[2] = 0xFF & navData.linePressures.pres_1;
    data[3] = 0xFF & (navData.linePressures.pres_1 >> 8);
    data[3] = 0xFF & navData.linePressures.pres_2;
    data[4] = 0xFF & (navData.linePressures.pres_2 >> 8);
    data[6] = 0; //TODO
    data[7] = 0;
    
    return;
}

/*
 * Message Type: 
 *      Updates data with latest value from Pressure Sensor 2
 * TO:   CCP
 * FROM: NAV
 *
 * Data Byte 2: Pressure 3 [7:0]
 * Data Byte 3: Pressure 3 [15:8]
 * Data Byte 4: Pressure 4 [7:0]
 * Data Byte 5: Pressure 4 [15:8]
 * Data Byte 6: TODO: Staleness??
 * Data Byte 7: 0 */
void nav_pressure2_set(uint8_t data[]){

    data[2] = 0xFF & navData.linePressures.pres_3;
    data[3] = 0xFF & (navData.linePressures.pres_3 >> 8);
    data[3] = 0xFF & navData.linePressures.pres_4;
    data[4] = 0xFF & (navData.linePressures.pres_4 >> 8);
    data[6] = 0; //TODO
    data[7] = 0;
    
    return;
}

/*
 * Message Type: 
 *      Updates data with latest value from Pressure Sensor 3
 *
 * TO:   CCP
 * FROM: NAV
 *
 * Data Byte 2: Pressure 5 [7:0]
 * Data Byte 3: Pressure 5 [15:8]
 * Data Byte 4: Pressure 6 [7:0]
 * Data Byte 5: Pressure 6 [15:8]
 * Data Byte 6: TODO: Staleness??
 * Data Byte 7: 0 */
void nav_pressure3_set(uint8_t data[]){

    data[2] = 0xFF & navData.linePressures.pres_5;
    data[3] = 0xFF & (navData.linePressures.pres_5 >> 8);
    data[3] = 0xFF & navData.linePressures.pres_6;
    data[4] = 0xFF & (navData.linePressures.pres_6 >> 8);
    data[6] = 0; //TODO
    data[7] = 0;
    
    return;
}

/*
 * Message Type: 
 *      Updates data with latest value from Pressure Sensor 4
 *
 * TO:   CCP
 * FROM: NAV
 *
 * Data Byte 2: Pressure 7 [7:0]
 * Data Byte 3: Pressure 7 [15:8]
 * Data Byte 4: Pressure 8 [7:0]
 * Data Byte 5: Pressure 8 [15:8]
 * Data Byte 6: TODO: Staleness??
 * Data Byte 7: 0 */
void nav_pressure4_set(uint8_t data[]){

    data[2] = 0xFF & navData.linePressures.pres_7;
    data[3] = 0xFF & (navData.linePressures.pres_7 >> 8);
    data[3] = 0xFF & navData.linePressures.pres_8;
    data[4] = 0xFF & (navData.linePressures.pres_8 >> 8);
    data[6] = 0; //TODO
    data[7] = 0;
    
    return;
}

/*
 * Message Type: 
 *      Updates data with latest value from Solenoid 1
 *
 * TO:   CCP
 * FROM: NAV
 *
 * Data Byte 2: Solenoid data [7:0]
 * Data Byte 3: TODO: Staleness??
 * Data Byte 4: 0
 * Data Byte 5: 0
 * Data Byte 6: 0
 * Data Byte 7: 0 */
void nav_solenoid1_set(uint8_t data[]){

    int i;

    data[2] |= navData.solenoids.solenoid_1.state;
    data[2] |= navData.solenoids.solenoid_2.state << 1;
    data[2] |= navData.solenoids.solenoid_4.state << 3;
    data[2] |= navData.solenoids.solenoid_6.state << 5;
    data[2] |= navData.solenoids.solenoid_7.state << 6;

    for( i = 3; i < 8; i++){
        data[i] = 0;
    }

    return;
}


/*
 * Message Type: 
 *      Updates data with latest value of Accleration, Velocity, and position
 *
 * TO:   CCP
 * FROM: NAV
 * 
 * Data Byte 2: Accel X
 * Data Byte 3: Vel X
 * Data Byte 4: Pos X
 * Data Byte 5: TODO: Staleness?
 * Data Byte 6: 0 
 * Data Byte 7: 0 */
void nav_accel_vel_pos_set(uint8_t data[]){

    data[2] = navData.motion.accX;
    data[3] = navData.motion.velX;
    data[4] = navData.motion.posX;
    data[5] = 0; //TODO
    data[6] = 0;
    data[7] = 0;
    
    return;
}
 
/*
 * Message Type: 
 *      Updates data with latest value from PV Pressure Sensor
 *
 * TO:   CCP
 * FROM: PV
 *
 * Data Byte 2: Pressure [7:0]
 * Data Byte 3: Pressure [15:8]
 * Data Byte 4: Pressure [23:16]
 * Data Byte 5: Pressure [31:24]
 * Data Byte 6: TODO: Staleness??
 * Data Byte 7: 0 */
void pv_pressure_set(uint8_t data[]){

    int i;

    for ( i = 0; i < 8; i++ ){
        data[i] = 0;
    }
    
    return;
}

/*
 * Message Type: 
 *      Updates data with current status of shutdown circuit
 *
 * TO:   CCP
 * FROM: PV
 *
 * Data Byte 2: Shutdown
 * Data Byte 3: TODO Staleness??
 * Data Byte 4: 0
 * Data Byte 5: 0
 * Data Byte 6: 0
 * Data Byte 7: 0 */
void pv_shutdown_set(uint8_t data[]){

    int i;

    for ( i = 0; i < 8; i++ ){
        data[i] = 0;
    }
    
    return;
}

