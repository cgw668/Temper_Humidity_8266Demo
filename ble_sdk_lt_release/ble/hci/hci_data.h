#pragma once

#include "../../proj/tl_common.h"
#include "hci.h"


/*********************************************************************
 * CONSTANTS
 */
#define HCI_MAX_ACL_DATA_LEN              27
#define HCI_MAX_DATA_BUFFERS              4

/**
 *  @brief  Definition for HCI ACL Data PB Flag
 */
#define HCI_FIRST_NAF_PACKET              0x00
#define HCI_CONTINUING_PACKET             0x01
#define HCI_FIRST_AF_PACKET               0x02


/*********************************************************************
 * ENUMS
 */



/*********************************************************************
 * TYPES
 */

/**
 *  @brief  Definition for HCI ACL Data format
 */
typedef struct {
	hci_type_t type;
	u16        connHandle;
	u8         pbFlag;
	u16        dataLen;
	u8         data[1];
} hci_aclData_t;

/**
 *  @brief  Definition for HCI RX ACL Data callback function type
 */
typedef void (*hci_dataHandler_t)( hci_aclData_t *pEvt );

/*********************************************************************
 * GLOBAL VARIABLES
 */


/*********************************************************************
 * Public Functions
 */

ble_sts_t hci_aclData_request(u16 connHandle, u8 pbFlag, u8 *data, u16 dataLen);
void hci_aclData_received(u16 connHandle, u8 pbFlag, u8 *data, u16 dataLen);