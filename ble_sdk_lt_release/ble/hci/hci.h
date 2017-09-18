#pragma once

#include "../../proj/tl_common.h"


/*********************************************************************
 * CONSTANTS
 */



/*********************************************************************
 * ENUMS
 */


/**
 *  @brief  Definition for HCI request type
 */
typedef enum hci_type_e {
	HCI_TYPE_CMD,
	HCI_TYPE_ACL_DATA,
	HCI_TYPE_EVENT,
} hci_type_t;


/*********************************************************************
 * TYPES
 */

/**
 *  @brief  Definition for HCI general packet format
 */
typedef struct {
	hci_type_t type;
	u8         payload[1];
} hci_general_t;



/*********************************************************************
 * GLOBAL VARIABLES
 */

// Just for internal use
extern ev_queue_t hci_ll2hciQ;
extern ev_queue_t hci_upper2hciQ;

/*********************************************************************
 * Public Functions
 */
#include "hci_event.h"
#include "hci_data.h"

void hci_reset(void);
void hci_init(void);
void hci_postTask(ev_queue_t *q, hci_general_t* pData);
void hci_registerEventCb(hci_eventHandler_t eventCb);
void hci_registerDataCb(hci_dataHandler_t dataCb);

#define HCI_LL2HCI_REQ(req)         hci_postTask(&hci_ll2hciQ, req)
#define HCI_UPPER2HCI_REQ(req)      hci_postTask(&hci_upper2hciQ, req)
