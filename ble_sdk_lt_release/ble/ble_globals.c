
/**********************************************************************
 * INCLUDES
 */
#include "../proj/tl_common.h"
#include "ble_globals.h"

/**********************************************************************
 * LOCAL CONSTANTS
 */

/**********************************************************************
 * TYPEDEFS
 */


#ifndef GET_RSSI_MODE  
#define GET_RSSI_MODE GET_RSSI_FROM_PACKET
#endif

#ifndef AGC_GAIN_MODE
#define AGC_GAIN_MODE MAX_GAIN_MODE_ENABLE
#endif

/**********************************************************************
 * GLOBAL VARIABLES
 */
u8 ble_g_get_rssi_mode = GET_RSSI_MODE;
u8 ble_g_agc_or_maxgain_mode = AGC_GAIN_MODE;
