#pragma  once

#include "../hci/hci_include.h"



// define the l2cap CID for BLE
#define L2CAP_CID_NULL                   0x0000
#define L2CAP_CID_ATTR_PROTOCOL          0x0004
#define L2CAP_CID_SIG_CHANNEL            0x0005
#define L2CAP_CID_SMP                    0x0006
#define L2CAP_CID_GENERIC                0x0007
#define L2CAP_CID_DYNAMIC                0x0040

#define L2CAP_CID_DYNAMIC                0x0040

#define L2CAP_HEADER_LENGTH              0x0004
#define L2CAP_MTU_SIZE                   23

#define L2CAP_CMD_REJECT                 0x01
#define L2CAP_CMD_DISC_CONN_REQ          0x06
#define L2CAP_CMD_DISC_CONN_RESP         0x07
#define L2CAP_CMD_CONN_UPD_PARA_REQ      0x12
#define L2CAP_CMD_CONN_UPD_PARA_RESP     0x13
#define L2CAP_CMD_CONN_REQ               0x14
#define L2CAP_CMD_CONN_RESP              0x15
#define L2CAP_CMD_FLOW_CTRL_CRED         0x16


#define L2CAP_SIGNAL_MSG_TYPE            0x01
#define L2CAP_DATA_MSG_TYPE              0x02

/** 
 * Command Reject: Reason Codes
 */
  // Command not understood
#define L2CAP_REJECT_CMD_NOT_UNDERSTOOD  0x0000

  // Signaling MTU exceeded
#define L2CAP_REJECT_SIGNAL_MTU_EXCEED   0x0001

  // Invalid CID in request
#define L2CAP_REJECT_INVALID_CID         0x0002

// Response Timeout expired
#define L2CAP_RTX_TIMEOUT_MS             2000

#define NEXT_SIG_ID()                    ( ++l2capId == 0 ? l2capId = 1 : l2capId )

#define L2CAP_HCI2L2CAP_REQ(req)         l2cap_postTask(&l2cap_hci2l2capQ, req)
#define L2CAP_UPPER2L2CAP_REQ(req)       l2cap_postTask(&l2cap_upper2l2capQ, req)


#define L2CAP_PKT_HANDLER_SIZE           6
 
 
// l2cap handler type
#define L2CAP_CMD_PKT_HANDLER            0
#define L2CAP_USER_CB_HANDLER            1

// l2cap pb flag type
#define L2CAP_FRIST_PKT_H2C              0x00
#define L2CAP_CONTINUING_PKT             0x01
#define L2CAP_FIRST_PKT_C2H              0x02


#define L2CAP_CONNECTION_PARAMETER_ACCEPTED        0x0000
#define L2CAP_CONNECTION_PARAMETER_REJECTED        0x0001

// private structs
typedef enum {
    L2CAP_STATE_CLOSED = 1,           // no baseband
    L2CAP_STATE_SEND_DISCONNECTION_REQUEST,
    L2CAP_STATE_WAIT_DISCONNECTION_RESPONSE,
    L2CAP_STATE_SEND_CONNECTION_UPDATE_REQUEST,
    L2CAP_STATE_WAIT_CONNECTION_UPDATE_RESPONSE,   
    L2CAP_STATE_SEND_CONNECTION_REQUEST,
    L2CAP_STATE_WAIT_CONNECTION_RESPONSE,
} L2CAP_STATE;


typedef void (*l2cap_cbFn_t) (void *pData);


// info regarding an actual coneection
typedef struct {

    L2CAP_STATE state;    
    u8   id;
    u16  localCid;
    u16  remoteCid;    
    

    u16  handle;    
    
    ev_time_event_t rtx; // also used for ertx

    u8 psm;
        
} l2cap_channel_t;



// define the l2cap packet command table struct 
typedef struct {
    u16 handle;
    u16 cid;
    u8 *data;
    u8 size;
} l2cap_pkt_t;


// define the l2cap header struct 
typedef struct {
#ifdef WIN32
	u8 reserved[5]; // reserved for ll
#else
	u8 reserved[8]; // reserved for ll
#endif
    u16 len;
    u16 cid;
} l2cap_rxHdr_t;

// define the l2cap signal header struct 
typedef struct {
    u8 code;
    u8 id;
    u16 len;
} l2cap_sigHdr_t;

typedef struct {
    l2cap_rxHdr_t hdr;
    l2cap_sigHdr_t sigHdr;
    u8 cmd[1];
} l2cap_rxSigCmdPkt_t;


typedef struct {
    l2cap_rxHdr_t hdr;
    u8 data[1];
} l2cap_rxDataPkt_t;

// define the l2cap header struct 
typedef struct {
	u8 reserved[10]; // reserved for ll
	u16 len;
	u16 cid;
} l2cap_txHdr_t;


typedef struct {
	l2cap_txHdr_t hdr;
	l2cap_sigHdr_t sigHdr;
	u8 cmd[1];
} l2cap_txSigCmdPkt_t;


typedef struct {
	l2cap_txHdr_t hdr;
	u8 data[1];
} l2cap_txDataPkt_t;


// define the l2cap packet handler
typedef ble_sts_t (*l2cap_handler) (l2cap_pkt_t *pData);


// define the l2cap packet handler 
typedef ble_sts_t (*l2cap_pktHandle) (u8 type, u16 handle, u16 size, u8 *pData);

// define the l2cap packet command table struct 
typedef struct {
    u8 cmd;
    l2cap_handler pktHandler;
    u8 len;
    bool fixedLen;
} l2cap_cmdTbl_t;


// define l2cap cmd reject
typedef struct {
    u16 reason;
    union {
        u16 signalMTU;
        struct {
            u16 local;
            u16 remote;
        } ivalidCID; 
    } data;
} l2cap_reject_t;


// define l2cap cmd disconnect request
typedef struct {
    u16 destCid; 
    u16 srcCid;
} l2cap_disconnReq_t;


// define l2cap cmd disconnect response
typedef struct {
    u16 destCid; 
    u16 srcCid;
} l2cap_disconnResp_t;

// define l2cap cmd connection parameter update request
typedef struct {
    u16 min_interval;
    u16 max_interval;
    u16 latency;
    u16 timeout;
} l2cap_connParUpReq_t;

// define l2cap cmd connection parameter update response
typedef struct {
    u16 result;
} l2cap_connParUpResp_t;

// define l2cap cmd credit based connection request
typedef struct {
    u16 psm;
    u16 srcCid;
    u16 mtu;
    u16 mps;
    u16 credits;
} l2cap_connReq_t;

// define l2cap cmd credit based connection response
typedef struct {
    u16 dstCid;
    u16 mtu;
    u16 mps;
    u16 credits;
    u16 result;
} l2cap_connResp_t;

// define l2cap cmd flow control credit
typedef struct {
    u16 cid;
    u16 credits;
} l2cap_flowCtrl_t;


typedef struct {
    u16 connHandle;
    u8 reserved;
    u8 *pData;
    /* used for single message */
    u8 status;
    u8 cmd;
    u8 id;
} l2cap_evtItem_t;


// info regarding potential connections
typedef struct {
     // service id
    u16  psm;
    
    // incoming MTU
    u16 mtu;
    
    // client connection
    void *connection;    
    
    // internal connection
    l2cap_cbFn_t cbFn;

} l2cap_service_t;

// the handler table for l2cap
typedef struct {
    u16 cid;
    l2cap_pktHandle cbFn;
    l2cap_cbFn_t userCb;
} l2cap_pktCb_t;


extern l2cap_channel_t l2cap_channels[];
extern const u8 l2cap_channelSize;

extern l2cap_cmdTbl_t ble_sigCmdTbl[];
extern const u8 ble_sigCmdTblSize;

extern l2cap_pktCb_t *l2cap_pktCbTbl;

// function declare 
ble_sts_t sig_cmd_reject(l2cap_pkt_t *pData);
ble_sts_t sig_disconn_req(l2cap_pkt_t *pData);
ble_sts_t sig_disconn_rsp(l2cap_pkt_t *pData);
ble_sts_t sig_conn_param_req(l2cap_pkt_t *pData);
ble_sts_t sig_conn_param_rsp(l2cap_pkt_t *pData);
ble_sts_t sig_ble_conn_req(l2cap_pkt_t *pData);
ble_sts_t sig_ble_conn_rsp(l2cap_pkt_t *pData);
ble_sts_t sig_ble_flowctl_creds(l2cap_pkt_t *pData);

ble_sts_t sig_sendConnParaUpdateReq(u16 connHandle, u16 connIntervalMin, u16 connIntervalMax, u16 latency, u16 timeout);


/*********************************************************************
 * @fn      l2cap_getChannelbyId
 *
 * @brief   get the channel entry by cid
 *
 * @param   id - id
 *
 * @return  l2cap_channel_t entry
 */
l2cap_channel_t *l2cap_getChannelbyId(u8 id);

/*********************************************************************
 * @fn      l2cap_getFreeChannel
 *
 * @brief   get the unused channel entry
 *
 * @param   queue - the handle queue
 *
 * @return  l2cap_channel_t entry
 */
l2cap_channel_t *l2cap_getFreeChannel(void);

/*********************************************************************
 * @fn      l2cap_resetChannel
 *
 * @brief   reset channel entry
 *
 * @param   l2cap_channel_t entry
 *
 * @return  None
 */
void l2cap_resetChannel(l2cap_channel_t *entry);


/*********************************************************************
 * @fn      l2cap_getServiceBypsm
 *
 * @brief   get the unused channel entry
 *
 * @param   queue - the handle queue
 *
 * @return  l2cap_channel_t entry
 */
l2cap_service_t * l2cap_getServiceBypsm(u16 psm);

/*********************************************************************
 * @fn      l2cap_getCmd
 *
 * @brief   get the cmd entry by cmd id
 *
 * @param   l2cap_cmdTbl_t - cmd table
 * @param   size - size of cmd table
 * @param   cmd - cmd id
 *
 * @return  cmd table entry
 */
l2cap_cmdTbl_t *l2cap_getCmd(l2cap_cmdTbl_t *tbl, u8 size, u8 cmd);

/*********************************************************************
 * @fn      l2cap_trxTimeoutCb
 *
 * @brief   callback function for l2cap trx timer
 *
 * @param   l2cap_channel_t entry
 *
 * @return  None
 */
int l2cap_trxTimeoutCb(void *arg);

/*********************************************************************
 * @fn      l2cap_startRtxTimer
 *
 * @brief   start the RTX timer for l2cap
 *
 * @param   l2cap_channel_t entry
 *
 * @return  None
 */
void l2cap_startRtxTimer(l2cap_channel_t *entry);

/*********************************************************************
 * @fn      l2cap_stopRtxTimer
 *
 * @brief   stop the RTX timer for l2cap
 *
 * @param   l2cap_channel_t entry
 *
 * @return  None
 */
void l2cap_stopRtxTimer(l2cap_channel_t *entry);
/*********************************************************************
 * @fn      l2cap_sendCmd
 *
 * @brief   send l2cap signal command
 *
 * @param   handle connection handler to use
 *
 * @param   cmd command code
 *
 * @param   id identifier
 *
 * @param   pData command payload
 *
 * @return  None
 */
ble_sts_t l2cap_sendCmd(u16 connHandle, u8 cmd, u8 id, u8 *pData);

/*********************************************************************
 * @fn      l2cap_sendSigMsgToUpperLayer
 *
 * @brief   send l2cap signal message to upper layer 
 *
 * @param   cid cid
 *
 * @param   connHandle connection handler to use
 *
 * @param   statue the status of this sessage.
 *
 * @param   cmd command code
 *
 * @param   id identifier
 *
 * @param   pData command payload
 *
 * @return  None
 */
ble_sts_t l2cap_sendMsgToUpperLayer(u16 cid, u16 connHandle, u8 status, u8 cmd, u8 id, u8 *pData);


/*********************************************************************
 * @fn      l2cap_sendReq
 *
 * @brief   send l2cap signal command
 *
 * @param   handle connection handler to use
 *
 * @param   cmd command code
 *
 * @param   state the state when sending thsi request
 *
 * @param   pData command payload
 *
 * @param   fn the callback function to handler this packet
 *
 * @return  None
 */
ble_sts_t l2cap_sendReq(u16 connHandle, u8 cmd, L2CAP_STATE state, u8 *pData) ;


/*********************************************************************
 * @fn      l2cap_sendData
 *
 * @brief   send l2cap data
 *
 * @param   connHandle connection handler to use
 *
 * @param   pData data payload
 *
 * @param   fn the callback function to handler this packet
 *
 * @return  None
 */
ble_sts_t l2cap_sendData(u16 connHandle, u8 *pData, u8 len, u16 cid);

/*********************************************************************
 * @fn      l2cap_task
 *
 * @brief   L2CAP Main Task. Check the queues and call proper handler
 *
 * @param   arg   - Not used.
 *
 * @return  None
 */
void l2cap_task(void* arg);

/*********************************************************************
 * @fn      l2cap_init
 *
 * @brief   Initialize BLE L2CAP Layer
 *
 * @param   None
 *
 * @return  None
 */
void l2cap_init(void);

/*********************************************************************
 * @fn      l2cap_registerPktHander
 *
 * @brief   register the packet handler for l2cap
 *
 * @param   cid cid
 *
 * @param   fn the callback function to handler this packet
 *
 * @return  None
 */
ble_sts_t l2cap_registerPktHander(u16 cid, l2cap_pktHandle handler);

/*********************************************************************
 * @fn      l2cap_findPkthandler
 *
 * @brief   register the packet handler for l2cap
 *
 * @param   cid cid
 *
 * @param   fn the callback function to handler this packet
 *
 * @return  None
 */
void *l2cap_findPkthandler(u16 cid, u8 type);