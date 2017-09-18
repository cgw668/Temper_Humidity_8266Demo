#pragma once

#include "../../proj/tl_common.h"
#include "hci.h"
#include "../controller/ll/ll_advertising.h"

/*********************************************************************
 * CONSTANTS
 */



/*********************************************************************
 * ENUMS
 */



/*********************************************************************
 * TYPES
 */


/**
 *  @brief  Definition for HCI command packet
 */
typedef struct {
	hci_type_t type;
	u16        opcode;
	u8         paraLen;
	u8         parameters[1];
} hci_cmd_t;


/*********************************************************************
 * GLOBAL VARIABLES
 */


/*********************************************************************
 * Public Functions
 */

// Information Parameters Commands. Vol 2, Part E, Chapter 7.4
ble_sts_t hci_readLocalVer_cmd(void);
ble_sts_t hci_readLocalSupportedCmds_cmd(void);
ble_sts_t hci_readBDAddr(void);

// LE Controller Commands. Vol 2, Part E, Chapter 7.8
ble_sts_t hci_le_setEventMask_cmd(u8* pEvtMask);
ble_sts_t hci_le_readBufferSize_cmd(void);
ble_sts_t hci_le_readLocalSupportedFeatures_cmd(void);
ble_sts_t hci_le_setRandomAddr_cmd(u8* randomAddr);
ble_sts_t hci_le_setAdvertisingParameters_cmd(adv_para_t* pCmd);
void hci_le_readAdvertisingChannelTxPower_cmd();
ble_sts_t hci_le_setAdvertisingData_cmd(u8* data, u8 dataLen);
ble_sts_t hci_le_setScanResponseData_cmd(u8* data, u8 dataLen);
ble_sts_t hci_le_setAdvertiseEnable_cmd(u8 fEnable);
ble_sts_t hci_le_setScanParameters_cmd(scan_para_t* para);
ble_sts_t hci_le_setScanEnable_cmd(u8 fEnable, u8 filterDuplicates);
ble_sts_t hci_le_createConnection_cmd(createConn_t* connPara);
ble_sts_t hci_le_createConnectionCancel_cmd(void);
ble_sts_t hci_le_readWhiteListSize_cmd(void);
ble_sts_t hci_le_clearWhiteList_cmd(void);
ble_sts_t hci_le_addDevToWhiteList_cmd(u8 addrType, u8* addr);
ble_sts_t hci_le_removeDevFromWhiteList_cmd(u8 addrType, u8* addr);
ble_sts_t hci_le_connectionUpdate_cmd(conn_para_t *para);
ble_sts_t hci_le_setHostChannelClassification_cmd(u8* channelMap);
ble_sts_t hci_le_readChannnelMap_cmd(u16 connHandle);
ble_sts_t hci_le_readRemoteUsedFeatures_cmd(u16 connHandle);
ble_sts_t hci_le_encrypt_cmd(u8* key, u8* plainText);
ble_sts_t hci_le_rand_cmd(void);
ble_sts_t hci_le_startEncrypt_cmd(u16 connHandle, u8* random, u16 encryptedDiv, u8* ltk);
ble_sts_t hci_le_longTermKeyRequestReply_cmd(u16 connHandle, u8* ltk);
ble_sts_t hci_le_longTermKeyRequestNegativeReply_cmd(u16 connHandle);
ble_sts_t hci_le_readSupportedStates_cmd(void);
void hci_le_receiverTest_cmd();
void hci_le_transmitterTest_cmd();
void hci_le_testEnd_cmd();
void hci_le_remoteConnectionParameterRequestReply_cmd();
void hci_le_remoteConnectionParameterRequestNegativeReply_cmd();
ble_sts_t hci_le_terminateConnection_cmd(u16 connHandle, u8 reason);

