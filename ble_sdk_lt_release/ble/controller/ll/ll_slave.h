#pragma once


/*********************************************************************
 * CONSTANTS
 */

#define MAX_SUPERVISION_TIMEOUT


/*********************************************************************
 * ENUMS
 */



/*********************************************************************
 * TYPES
 */
typedef struct {
    u16 connHandle;
    u8  txPointerInBB;    // this stores the value of 0x52b, which is write_pointer in TX fifo, it is used to check whether it is be acked.
    u8  reserved[1];
    u8  txData[1];
} ll_slave_txEvt_t;




/*********************************************************************
 * GLOBAL VARIABLES
 */
extern u32 ll_anchor_point;
extern ev_queue_t ll_slave_txEvtQ;


/*********************************************************************
 * Public Functions
 */

#define LL_UPDATE_ANCHOR_POINT(timestamp)    (ll_connEntry.anchorPoint = timestamp)


void ll_slave_begin(void);
void ll_slave_connectionEvtEnd(u8* arg);
void ll_slave_rxHandler(u8* arg);
void ll_slave_connectionEvtMissingHandler(u8* arg);
void ll_slave_tx(u16 connHandle, u8 pbFlag, u8* data, u8 len);
u8   ll_slave_usedBufferNum(void);
void ll_slave_onBrxFirstTimeout(u8* arg);
void ll_slave_onPhyTimeout(u8* arg);