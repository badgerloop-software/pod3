#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "exti.h"
#include "pin_alias.h"
#include "retro.h"
#include "commands.h"

void printStamp (int pin, timeStamp * stamp) {
    int i;
    
    printf("addr %p\r\n", stamp);
    printf("%-10s:%d\r\n", "Pin", pin);
    printf("%-10s:%lu\r\n", "Previous", stamp->prev);
    printf("%-10s:%lu\r\n", "Current", stamp->curr);
    printf("%-10s:%lu\r\n", "Count", stamp->count);
    
    for (i = 0; i < AVERAGE_SIZE; i++){
        printf("Filter[%d]:%lu\r\n",i, stamp->filter[i]);
    }   
       
    printf("main\r\n");
    printf("addr %p\r\n", mainRetro);
    printf("%-10s:%lu\r\n", "Previous", mainRetro->prev);
    printf("%-10s:%lu\r\n", "Current", mainRetro->curr);
    printf("%-10s:%lu\r\n", "Count", mainRetro->count);
           
    for (i = 0; i < AVERAGE_SIZE; i++){
        printf("Filter[%d]:%lu\r\n", i,mainRetro->filter[i]);
    }
}

command_status do_exti(int argc, char *argv[]) {
           
    int pin = 0;

    if (argc < 2) return USAGE;
    pin = atoi((const char *) argv[1]);
           
    if ( pin == 0 ) return USAGE;
    
    printStamp(pin, getTimeStamps(pin));
    
//    int retro_count, vel, pos, acc;
    
//    getRetroTelemetry(&retro_count, &pos, &vel, &acc);  
//    printf("Pos: %dcm\tVelocity: %dcm/s\r\n", pos,vel);
        
    return CMD_SUCCESS;
}
        
COMMAND_ENTRY("exti", 
    "exti <pin>", 
    "Prints current and previous time stamp of the External Interrupt. Only on interrupt per pin number", 
    do_exti
) 
        
command_status do_retro(int argc, char *argv[]) {

    if( !strcmp( argv[1], "test")){
        incVel(4);
        return CMD_SUCCESS;
    }
    
    if(argc > 1) return USAGE;
               
//        int retro_count, pos, vel, acc;

    //    if(!getRetroTelemetry(&retro_count, &pos, &vel, &acc)) {
    //    printf("Function call failed %s\r\n", argv[1]);
    //    return FAIL;
    // }
            
//    printf("Count: %dPos: %dcm\tVel: %dcm/s\tAcc: %dcm/s^2\r\n", retro_count, pos, vel, acc);
    return CMD_SUCCESS;
}

COMMAND_ENTRY("retro", 
    "retro", 
    "Prints retros estimated position and velocity",
    do_retro
)
