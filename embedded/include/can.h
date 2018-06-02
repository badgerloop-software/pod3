#ifndef _CAN_H__
#define _CAN_H__

#include <stdint.h>
#include "system.h"

/*****************************************************************************/
/*                                 CAN Enums                                 */
/*****************************************************************************/


char* can_read();
void can_send(uint16_t can_id, uint64_t length, uint64_t data);
#endif
