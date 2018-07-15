#include "retro.h"


/*
 * Message Type:
 *      Updates data with current voted-on retro count
 *
 * TO:   CCP
 * FROM: NAV
 *
 * Data Byte 2: Voted Retro Count
 * Data Byte 3: 0
 * Data Byte 4: 0
 * Data Byte 5: 0
 * Data Byte 6: 0
 * Data Byte 7: 0 */
void nav_tape_set(uint8_t data[]){
    int i;
    int badRetro = 0;
    getStripCount( &badRetro );
    
    //TODO: Do error checking with badRetro
    data[2] = getStripCount( &badRetro );
    
    for ( i = 3; i < 8; i++ ){
        data[i] = 0;
    }
    
    return;
}

/*
 * Message Type: 
 *
 * TO:   CCP
 * FROM: NAV
 *
 * Data Byte 2: 0
 * Data Byte 3: 0
 * Data Byte 4: 0
 * Data Byte 5: 0
 * Data Byte 6: 0
 * Data Byte 7: 0 */
void nav_should_stop_set(uint8_t data[]){
    int i;

    for ( i = 0; i < 8; i++ ){
        data[i] = 0;
    }
    
    return;
}

/*
 * Message Type: 
 *
 * TO:   CCP
 * FROM: NAV
 *
 * Data Byte 2: 0
 * Data Byte 3: 0
 * Data Byte 4: 0
 * Data Byte 5: 0
 * Data Byte 6: 0
 * Data Byte 7: 0 */
void nav_pressure1_set(uint8_t data[]){

    int i;

    for ( i = 0; i < 8; i++ ){
        data[i] = 0;
    }
    
    return;
}

/*
 * Message Type: 
 *
 * TO:   CCP
 * FROM: NAV
 *
 * Data Byte 2: 0
 * Data Byte 3: 0
 * Data Byte 4: 0
 * Data Byte 5: 0
 * Data Byte 6: 0
 * Data Byte 7: 0 */
void nav_pressure2_set(uint8_t data[]){

    int i;

    for ( i = 0; i < 8; i++ ){
        data[i] = 0;
    }
    
    return;
}

/*
 * Message Type: 
 *
 * TO:   CCP
 * FROM: NAV
 *
 * Data Byte 2: 0
 * Data Byte 3: 0
 * Data Byte 4: 0
 * Data Byte 5: 0
 * Data Byte 6: 0
 * Data Byte 7: 0 */
void nav_pressure3_set(uint8_t data[]){

    int i;

    for ( i = 0; i < 8; i++ ){
        data[i] = 0;
    }
    
    return;
}

/*
 * Message Type: 
 *
 * TO:   CCP
 * FROM: NAV
 *
 * Data Byte 2: 0
 * Data Byte 3: 0
 * Data Byte 4: 0
 * Data Byte 5: 0
 * Data Byte 6: 0
 * Data Byte 7: 0 */
void nav_pressure4_set(uint8_t data[]){

    int i;

    for ( i = 0; i < 8; i++ ){
        data[i] = 0;
    }
    
    return;
}

/*
 * Message Type: 
 *
 * TO:   CCP
 * FROM: NAV
 *
 * Data Byte 2: 0
 * Data Byte 3: 0
 * Data Byte 4: 0
 * Data Byte 5: 0
 * Data Byte 6: 0
 * Data Byte 7: 0 */
void nav_pressure5_set(uint8_t data[]){

    int i;

    for ( i = 0; i < 8; i++ ){
        data[i] = 0;
    }
    
    return;
}

/*
 * Message Type: 
 *
 * TO:   CCP
 * FROM: NAV
 *
 * Data Byte 2: 0
 * Data Byte 3: 0
 * Data Byte 4: 0
 * Data Byte 5: 0
 * Data Byte 6: 0
 * Data Byte 7: 0 */
void nav_solenoid1_set(uint8_t data[]){

    int i;

    for ( i = 0; i < 8; i++ ){
        data[i] = 0;
    }
    
    return;
}

/*
 * Message Type: 
 *
 * TO:   CCP
 * FROM: NAV
 *
 * Data Byte 2: 0
 * Data Byte 3: 0
 * Data Byte 4: 0
 * Data Byte 5: 0
 * Data Byte 6: 0
 * Data Byte 7: 0 */
void nav_solenoid2_set(uint8_t data[]){

    int i;

    for ( i = 0; i < 8; i++ ){
        data[i] = 0;
    }
    
    return;
}

/*
 * Message Type: 
 *
 * TO:   CCP
 * FROM: NAV
 *
 * Data Byte 2: 0
 * Data Byte 3: 0
 * Data Byte 4: 0
 * Data Byte 5: 0
 * Data Byte 6: 0
 * Data Byte 7: 0 */
void nav_accel_vel_pos_set(uint8_t data[]){

    int i;

    for ( i = 0; i < 8; i++ ){
        data[i] = 0;
    }
    
    return;
}
 
/*
 * Message Type: 
 *
 * TO:   CCP
 * FROM: PV
 *
 * Data Byte 2: 0
 * Data Byte 3: 0
 * Data Byte 4: 0
 * Data Byte 5: 0
 * Data Byte 6: 0
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
 *
 * TO:   CCP
 * FROM: PV
 *
 * Data Byte 2: 0
 * Data Byte 3: 0
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

