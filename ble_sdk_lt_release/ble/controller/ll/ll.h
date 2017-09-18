#pragma once
#include "../../../proj/tl_common.h"
#include "../include/ll_const.h"

/*********************************************************************
 * CONSTANTS
 */
#define LL_ROLE_MASTER                                 0
#define LL_ROLE_SLAVE                                  1



#define LL_FEATURE_ENCRYPTION                          1  // byte 0, bit 0
 
#define LL_SDK_MASTER_SIZE                             8
#define LL_SDK_SLAVE_SIZE                              8
 
#define LL_IV_MASTER_SIZE                              4
#define LL_IV_SLAVE_SIZE                               4


#define LL_SUPERVISION_TIMEOUT_TO_US(supertimeout)     (supertimeout*10000) // timeout  * 10ms   
#define LL_CONN_INTERVAL_TO_US(connInterval)           (connInterval*1250)  // interval * 1.25ms


#define LL_UPDATE_PARA_TIMING_BIT                      0x01
#define LL_UPDATE_PARA_CHANNEL_BIT                     0x02

/*********************************************************************
 * ENUMS
 */

/**
 *  @brief  Definition for Link Layer State
 */
typedef enum {
	LL_STATE_STANDBY,               //!< Standby state
	LL_STATE_CUA,                   //!< Connectable Undirected Advertiser State
	LL_STATE_CDA,                   //!< Connectable Directed Advertiser State
	LL_STATE_NCA,                   //!< Non-Connectable Advertiser State
	LL_STATE_DUA,                   //!< Discoverable Undirected Advertiser State
	LL_STATE_ACTIVE_SCAN,           //!< Active scan state
	LL_STATE_PASSIVE_SCAN,          //!< Passive scan state
	LL_STATE_INITIATOR,             //!< Initiator state
	LL_STATE_ADVERTISING_DONE,      //!< Advertising Done state, this state is quick state and should enter MASTER/SLAVE then
	LL_STATE_MASTER,                //!< Master state
	LL_STATE_SLAVE,                 //!< Slave state
} ll_state_t;


/**
 *  @brief  Definition for Link Layer Events
 */
typedef enum {
    LL_EVENT_HCI_CMD,
        
    LL_EVENT_ADV_EVT_END,
    LL_EVENT_ADV_TIMEOUT,

    LL_EVENT_BB_RX_TIMEOUT,
    LL_EVENT_BB_RX_CMDDONE,
    LL_EVENT_BB_RX_PACKET,
    LL_EVENT_BB_BRX_FIRST_TIMEOUT,
} ll_evt_t;



/*********************************************************************
 * TYPES
 */

typedef void ( *ll_evHandler_t )( u8* pData );


/** @brief  LL state machine */
typedef struct
{
	ll_state_t curState;             /*! The LL State in which the event handler can be used */
	ll_evt_t   event;                /*! The event to decide which LL operation be triggered */
	ll_evHandler_t evHandlerFunc;    /*! The corresponding event handler */
} ll_stateMachine_t;


// Feature Set Data
typedef struct
{
  u8 featureRspRcved;                             
  u8 featureSet[8];
} featureSet_t;


/** @brief  LL security related info */
typedef struct {
    u32 txPktCnt;
    u32 rxPktCnt;
    u8 IV[LL_IV_MASTER_SIZE + LL_IV_SLAVE_SIZE];
    u8 SDK[LL_SDK_MASTER_SIZE + LL_SDK_SLAVE_SIZE];
    u8 ltk[16];
    u8 sessionKey[16];
    u8 random[8];
    u16 ediv;  
    u8 encRestart;  
} sec_info_t;


/**
 *  @brief  Definition the parameter format from PHY to LL
 */
typedef struct {

	u8  channel;
	u8  rssi;
	u8  len;
	u32 timestamp;
	u8* packet;


	u8  txAcked;
	u8* txEvt; // the Acked tx event. Used only when txAcked = 1;

} ll_phyReq_t;


typedef struct {
    connReq_pld_t connPara;
    u16 connHandle;
    u32 anchorPoint;
    u32 nextExpectTime;
    ev_time_event_t *supervisionTimer;
    u8  encEnableFlag;           // indication current connection is encryption or not 
    u8  fTxNewConnectionEvent;
    u8  fNewConnectionEvent;
    featureSet_t feature;
    u16 connEventCounter;
    sec_info_t secInfo;
    u8  fUpdatePara;
} ll_connEntry_t;

typedef struct {
    u8 reserved[8];
    ll_dataHdr_t hdr;
    u8 data[1];
} ll_pkt_t;


typedef struct {
    u8 chMap[5];
    u8 winSize;
	u16 winOffset;
	u16 interval;
	u16 latency;
	u16 timeout;
	u16 instant;
} ll_updateConnPara_t;


/*********************************************************************
 * GLOBAL VARIABLES
 */

// Just for internal use
extern ll_state_t ll_curState;
extern ev_queue_t ll_hci2llQ;
extern ev_queue_t ll_phy2llQ;

extern addr_t ll_selfAddr;
extern addr_t ll_peerAddr;

extern ll_connEntry_t ll_connEntry;
extern featureSet_t ll_defaultFeature;
extern ll_updateConnPara_t ll_newConnPara;

extern u8 ll_featureSupports[];

/*********************************************************************
 * Public Functions
 */
void ll_reset(void);
void ll_init(void);
void ll_postTask(ev_queue_t *q, u8* pData, ll_evt_t ll_evt);


u8 ll_isConnectionValid(u16 connHandle);

ble_sts_t ll_getLocalSupportedFeatures(u8* features);
ble_sts_t ll_getRemoteSupportedFeatures(u16 connHandle);
ble_sts_t ll_setRandomAddr(u8* randomAddr);
ble_sts_t ll_readBDAddr(u8* addr);
void ll_data_send(u16 connHandle, u8 pbFlag, u8* data, u8 len);
void ll_cmd_send(u16 connHandle, u8* data, u8 len);


ble_sts_t ll_channelMap_update(u8* channelMap);
ble_sts_t ll_channelMap_read(u16 connHandle, u8* returnChannelMap);

ble_sts_t ll_encrypt(u8* key, u8* plainText, u8* cypherText);
ble_sts_t ll_startEncrypt(u16 connHandle, u8* random, u16 encryptedDiv, u8* ltk);
ble_sts_t ll_random(u8* returnData, u8 len);
ble_sts_t ll_encLtkReply(u16 connHandle, u8* ltk);
ble_sts_t ll_encLtkNegReply(u16 connHandle);

ble_sts_t ll_readSupportedStates(u8* returnStates);

ble_sts_t ll_encryption(u16 connHandle, u8 *plainText, u8 len, u8 hdr, u8 *result, u8 *mic);
ble_sts_t ll_decryption(u16 connHandle, u8 *plainText, u8 len, u8 hdr, u8 *result, u8 *mic);

u8 ll_getTxBufferSize(void);
u8 ll_readControllerUsedBufferNum(void);

ble_sts_t ll_terminateConn(u16 connHandle, u8 reason);

#define LL_HCI2LL_REQ(ll_evt, para)      ll_postTask(&ll_hci2llQ, para, ll_evt)
#define LL_PHY2LL_REQ(ll_evt, para)      ll_postTask(&ll_phy2llQ, para, ll_evt)


#define LL_SET_STATE(state)              (ll_curState = state)
#define LL_GET_STATE()                   (ll_curState)
