#pragma once

#include "../../proj/tl_common.h"

/** @addtogroup  TELINK_BLE_STACK TELINK BLE Stack
 *  @{
 */

/** @addtogroup  GAP_PERIPHERAL_Module GAP Peripheral
 *  @{
 */
 
/** @addtogroup  GAP_PERIPHERAL_Constant GAP Peripheral Constants
 *  @{
 */
#define DEFAULT_LONGEST_TIME_INTERVAL      (60 * 1000)

/** @addtogroup gap_pheripheral_state_t GAP Peripheral Role States
 * States of GAP peripheral Role
 * @{
 */
/**
 *  @brief  Definition for Peripheral State
 */
typedef enum {
	PERIPHERAL_STATE_RESET,                   //!< Reset State
	PERIPHERAL_STATE_STANDBY,                 //!< Standby state
	PERIPHERAL_STATE_PREPARE_ADVERTISING,     //!< Perpareing advertisement state. Not notifiable
	PERIPHERAL_STATE_ADVERTISING,             //!< Currently Advertising
	PERIPHERAL_STATE_CONNECTED,               //!< In a connection
	PERIPHERAL_STATE_TERMINATED,              //!< Terminated State
	PERIPHERAL_STATE_ERROR,
	PERIPHERAL_STATE_ENCRYPTED,
	PERIPHERAL_STATE_BOND,
} gap_pheripheral_state_t;

/** @} end of group gap_pheripheral_state_t */


/** @addtogroup  GAP_Peripheral_Types GAP Peripheral Types
 *  @{
 */

/**
 *  @brief  Definition GAP state change callback function's argument type
 */
typedef struct {
	gap_pheripheral_state_t newState;
	u16 connHandle;
} gap_stateChanged_t;

/**
 *  @brief  Definition GAP state change callback function type for user application
 */
typedef void (*gap_stateChangeCb_t)( gap_stateChanged_t *pStateChangedEvt );

/**
 *  @brief  Definition for foundation command callbacks.
 */
typedef struct gap_AppCallbacks_s {
	gap_stateChangeCb_t  stateChangeCbFunc;   //!< gap state change callback function  
} gap_AppCallbacks_t;


// Internal Use

/**
 *  @brief  Definition for GAP event handler function
 */
typedef void ( *gap_peripheral_evHandler_t )( u8* pData );

/**
 *  @brief  Definition for GAP state machine.
 *          Note this state machine table is implemented in each role.
 */
typedef struct
{
	gap_pheripheral_state_t     curState;         //!< The GAP State in which the event handler can be used */
	u8                          event;            //!< The event to decide which GAP operation be triggered */
	gap_peripheral_evHandler_t  evHandlerFunc;    //!< The corresponding event handler */
} gap_peripheral_stateMachine_t;


/** @} end of group GAP_Peripheral_Types */



/*********************************************************************
 * GLOBAL VARIABLES
 */

// for internal use
extern gap_pheripheral_state_t gap_peripheral_curState;


/** @addtogroup  GAP_PERIPHERAL_Fountions GAP Peripheral APIs
 *  @{
 */

/**
 * @brief   Reset GAP Peripharal Module
 *
 * @param   None
 *
 * @return  None
 */
void      gap_peripheral_reset(void);

/**
 * @brief   Initialize GAP Peripharal Module
 *
 * @param   None
 *
 * @return  Status
 */
ble_sts_t gap_peripheral_init(void);

/**
 * @brief      Add callback to GAP Peripheral Role
 *             Note, there could be more than one callback in the GAP peripheral layer.
 *
 * @param[in]  cb - The callback function to be registered
 *
 * @return     Status
 */
ble_sts_t gap_peripheral_addListener(gap_AppCallbacks_t* cb);


/**
 * @brief      API to set the GAP information base item's value
 *
 * @param[in]  id   - Specified GAP Information base ID
 * @param[in]  data - Payload of scan response data
 * @param[in]  len  - Length of the scan response data
 *
 * @return     Status
 */
ble_sts_t gap_peripheral_set(u8 id, u8* value, u8 len);

/**
 * @brief      API to enable/disable device to advertising mode
 *
 * @param[in]  fEnable - Flag to indication enable/disable advertising
 *
 * @return     Status
 */
ble_sts_t gap_peripheral_enableAdvertising(u8 fEnable);

/**
 * @brief      API to set the GAP Peripheral state
 *
 * @param[in]  state - GAP peripheral state. @ref gap_pheripheral_state_t
 *
 * @return     None
 */
#define GAP_PERIPHERAL_SET_STATE(state)      (gap_peripheral_curState = state)

/**
 * @brief      API to get the GAP Peripheral state
 *
 * @param[in]  None
 *
 * @return     GAP Peripheral State
 */
#define GAP_PERIPHERAL_GET_STATE()           (gap_peripheral_curState)


/** @} end of group GAP_PERIPHERAL_Fountions */

/** @} end of group GAP_PERIPHERAL_Module */

/** @} end of group TELINK_BLE_STACK */
