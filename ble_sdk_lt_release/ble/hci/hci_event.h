#pragma once

#include "../../proj/tl_common.h"
#include "hci.h"


/*********************************************************************
 * CONSTANTS
 */
#define MAX_CONNECTION_HANDLE_SUPPORT      4


/*********************************************************************
 * ENUMS
 */



/*********************************************************************
 * TYPES
 */

/**
 *  @brief  Definition for general HCI event packet
 */
typedef struct {
	hci_type_t type;
	u8         eventCode;
    u8         paraLen;
	u8         parameters[1];
} hci_event_t;

/**
 *  @brief  Definition for HCI command complete event
 */
typedef struct {
	hci_type_t type;
	u8         eventCode;
	u8         numHciCmds;
	u16        opCode;
	u8         paraLen;
	u8         parameters[1];
} hci_cmdCompleteEvt_t;

/**
 *  @brief  Definition for HCI command status event
 */
typedef struct {
	hci_type_t type;
	u8         eventCode;
	u8         numHciCmds;
	u16        opCode;
	u8         status;
} hci_cmdStatusEvt_t;

/**
 *  @brief  Definition for HCI command status event
 */
typedef struct {
	hci_type_t type;
	u8         eventCode;
	u8         handlesNum;
	u16        handles[MAX_CONNECTION_HANDLE_SUPPORT];
	u16        completedPktsNum[MAX_CONNECTION_HANDLE_SUPPORT];
} hci_numOfCompletedPktsEvt_t;


/**
 *  @brief  Definition for HCI LE Meta event
 */
typedef struct {
	hci_type_t type;
	u8         eventCode;
	u8         subEventCode;
	u8         parameters[1];
} hci_le_metaEvt_t;


/**
 *  @brief  Definition for HCI connection complete event
 */
typedef struct {
	hci_type_t type;
	u8         eventCode;
	u8         subEventCode;
	u8         status;
	u16        connHandle;
	u8         role;
	u8         peerAddrType;
	u8         peerAddr[BLE_ADDR_LEN];
	u16        connInterval;
	u16        slaveLatency;
	u16        supervisionTimeout;
	u8         masterClkAccuracy;
} hci_le_connectionCompleteEvt_t;

/**
 *  @brief  Definition for HCI disconnection complete event
 */
typedef struct {
    hci_type_t type;
	u8         eventCode;
    u8         status;
    u16        connHandle;
    u8         reason;
} hci_disconnectionCompleteEvt_t;


/**
 *  @brief  Definition for HCI long term key request event
 */
typedef struct {
    hci_type_t type;
	u8         eventCode;
	u8         subEventCode;
    u16        connHandle;
    u8         random[8];
    u16        ediv;
} hci_le_longTermKeyRequestEvt_t;


/**
 *  @brief  Definition for HCI disconnection complete event
 */
typedef struct {
    hci_type_t type;
	u8         eventCode;
	u8         subEventCode;
	u8 		   status;
    u16        connHandle;
    u8         encEnable;
} hci_encryptionChangeEvt_t;



/**
 *  @brief  Definition for HCI event handler callback function type
 */
typedef void (*hci_eventHandler_t)( hci_event_t *pEvt );

/*********************************************************************
 * GLOBAL VARIABLES
 */
extern u8 hci_eventMask;

/*********************************************************************
 * Public Functions
 */
void hci_eventCb_dispatcher(hci_event_t *pEvt);
void hci_event_registerGapCB(hci_eventHandler_t gapCb);
void hci_cmdComplete_evt(u8 numHciCmds, u16 opCode, u8 paraLen, u8* para);
void hci_cmdStatus_evt(u8 numHciCmds, u16 opCode, u8 status);
void hci_numOfCompletedPkts_evt(u8 handlesNum, u16 *connHandles, u16 *completedPktsNum);
void hci_le_connectionComplete_evt(u8 status, u16 connHandle, u8 role, u8 peerAddrType, u8* peerAddr, 
	    u16 connInterval, u16 slaveLatency, u16 supervisionTimeout, u8 masterClkAccuracy);
void hci_disconnectionComplete_evt(u8 status, u16 connHandle, u8 reason);

/*********************************************************************
 * @fn      hci_le_longTermKeyRequest_evt
 *
 * @brief   API to send disconnection complete event to Host.
 *
 * @param   handlesNum       - The number of Connection_Handles
 *          connHandles      - Connection_Handles
 *          completedPktsNum - The number of HCI Data Packets that 
 *                             have been completed.
 *
 * @return  None
 */
void hci_le_longTermKeyRequest_evt(u16 connHandle, u8 *random, u16 ediv);

/*********************************************************************
 * @fn      hci_encryptionChange_evt
 *
 * @brief   API to send disconnection complete event to Host.
 *
 * @param   handlesNum       - The number of Connection_Handles
 *          connHandles      - Connection_Handles
 *          completedPktsNum - The number of HCI Data Packets that 
 *                             have been completed.
 *
 * @return  None
 */
void hci_encryptionChange_evt(u16 connHandle, u8 status, u8 encEnable);

/*********************************************************************
 * @fn      hci_event_registerSmpCB
 *
 * @brief   API to register SMP layer HCI event callback
 *
 * @param   smpCb - SMP layer HCI event callback  
 *
 * @return  None
 */
void hci_event_registerSmpCB(hci_eventHandler_t smpCb);