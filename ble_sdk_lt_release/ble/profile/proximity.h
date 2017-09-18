
#pragma once

#include "../../proj/tl_common.h"



/** @addtogroup  TELINK_BLE_STACK TELINK BLE Stack
 *  @{
 */

/** @addtogroup  PROFILE_MODULE Profiles
 *  @{
 */


/** @addtogroup  BATTERY_SERVICE_Module Battery Service
 *  @{
 */
 
/** @addtogroup  BATTERY_SERVICE_Constant Battery Service Constants
 *  @{
 */


/**
 *  @brief  Definition for Profile ROLES
 */
#define PROXIMITY_ROLE_REPORTER                          0
#define PROXIMITY_ROLE_MONITOR                           1


/**
 *  @brief  Definition for Services Enable Mask
 */
#define SERVICE_MASK_LINK_LOSS                           0x01  // Mandatory
#define SERVICE_MASK_IMMEDIATE_ALERT                     0x02  // Optional
#define SERVICE_MASK_TXPOWER                             0x04  // Optional

/**
 *  @brief  Definition for Profile APP State
 */
#define PROXIMITY_APP_STATE_IDLE                         0     //!< Current dvice is idle and reseted
#define PROXIMITY_APP_READY                              1     //!< Current dvice started successful and ready for further procedure
#define PROXIMITY_APP_STATE_DEV_DISCOVERYING             2     //!< Current device is under device discovering state
#define PROXIMITY_APP_STATE_DEV_DISCOVERY_TIMEOUT        3     //!< Timeout after 180 second's device discovering
#define PROXIMITY_APP_STATE_BONDED                       4     //!< Current device has bonded with an Proximity Central Device
#define PROXIMITY_APP_STATE_DISCONNECTED                 5     //!< Current device disconnected with an bonded Proximity Central Device 




/*********************************************************************
 * ENUMS
 */


/*********************************************************************
 * TYPES
 */


/**
 *  @brief  Definition GAP state change callback function type for user application
 */
typedef void (*proximity_stateChangeCb_t)( u8 profileState );

/**
 *  @brief  Definition for foundation command callbacks.
 */
typedef struct {
	proximity_stateChangeCb_t  stateChangeCbFunc;    //!< proximity state change callback function


} proximity_callbacks_t;


/*********************************************************************
 * GLOBAL VARIABLES
 */


/*********************************************************************
 * Public Functions
 */
void proximity_init(u8 role, u8 serviceMask, proximity_callbacks_t* cb);
ble_sts_t proximity_enableDeviceDiscovery(u8 fEnable);


ble_sts_t proximity_set_linkLossAlertLevel(u8 level);
ble_sts_t proximity_set_immediateAlertLevel(u8 level);
ble_sts_t proximity_set_txpowerLevel(u8 level);
