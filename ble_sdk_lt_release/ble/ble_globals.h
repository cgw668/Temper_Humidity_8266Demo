#pragma once

#include "controller/phy/phy.h"

/**********************************************************************
 * INCLUDES
 */

/**********************************************************************
 * MARCOS
 */
#define TXPOWER_MASK_ADV                      0x0f
#define TXPOWER_MASK_CONNECTED                0xf0


/**********************************************************************
 * TYPES
 */
/*GET RSSI MODE*/
enum{
     GET_RSSI_FROM_PACKET = 0, //Get rssi from packet
     GET_RSSI_FROM_REGISTER = 1, //Get rssi from register 0x458
};

/*AGC MODE OR MAX GAIN MODE*/
enum{
    AGC_MODE_ENABLE=0, //AGC MODE
    MAX_GAIN_MODE_ENABLE=1, //MAX GAIN MODE, Can't display rssi larger than -58dbm
};


/**********************************************************************
 * VARIABLES
 */
extern u8 ble_g_get_rssi_mode;
extern u8 ble_g_agc_or_maxgain_mode;


/**********************************************************************
 * APIS
 */
