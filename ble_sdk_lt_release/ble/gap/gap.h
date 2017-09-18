#pragma once

#include "../../proj/tl_common.h"
#include "gap_const.h"


/*********************************************************************
 * CONSTANTS
 */

/*
 * Definition for GAP Event Code
 */
#define GAP_EVT_INIT_DONE                   1
#define GAP_EVT_SET_ADV_PARA_DONE           2
#define GAP_EVT_SET_ADV_DATA_DONE           3
#define GAP_EVT_SET_SCAN_RSP_DATA_DONE      4
#define GAP_EVT_SET_ADV_ENABLE_DONE         5
#define GAP_EVT_CONNECTION_ESTABLISHED      6
#define GAP_EVT_CONNECTION_TERMINATED       7
#define GAP_EVT_BOND_DONE                   8
#define GAP_EVT_ENCRYTP_DONE                9


/*
 * Definition for GAP ADV Type
 */
#define GAP_ADV_TYPE_ADV_IND                0x00  //!< Connectable undirected advertisement
#define GAP_ADV_TYPE_ADV_DIRECT_IND         0x01  //!< Connectable directed advertisement
#define GAP_ADV_TYPE_ADV_DISCOVER_IND       0x02  //!< Discoverable undirected advertisement
#define GAP_ADV_TYPE_ADV_NONCONN_IND        0x03  //!< Non-Connectable undirected advertisement
#define GAP_ADV_TYPE_SCAN_RSP_IND           0x04  //!< Only used in gapDeviceInfoEvent_t


#define DFLT_GAP_ADV_CHANNEL_MAP            ADV_CHANNEL_MAP_ALL

#define DFLT_GAP_ADV_FILTER_POLICY          ADV_FP_ALLOW_SCAN_WL_ALLOW_CONN_WL
#define DFLT_GAP_ADV_INTERVAL_MIN           T_GAP_LIM_DISC_ADV_INT_MIN_DEFAULT
#define DFLT_GAP_ADV_INTERVAL_MAX           T_GAP_LIM_DISC_ADV_INT_MAX_DEFAULT

#define GAP_INVALID_CONNECTION_HANDLE       0xFF
#define DFLT_GAP_CONNECTION_HANDLE          GAP_INVALID_CONNECTION_HANDLE

#define DEFAULT_MIN_CONN_INTERVAL           0x0006  // 100 milliseconds
#define DEFAULT_MAX_CONN_INTERVAL           0x0C80  // 4 seconds


#define DEFAULT_TIMEOUT_MULTIPLIER          1000

#define CONN_INTERVAL_MULTIPLIER            6

#define MIN_SLAVE_LATENCY                   0
#define MAX_SLAVE_LATENCY                   500


#define ADDR_TYPE_PUBLIC               0x00  //!< Use the BD_ADDR
#define ADDR_TYPE_STATIC               0x01  //!< Static address
#define ADDR_TYPE_PRIVATE_NONRESOLVE   0x02  //!< Generate Non-Resolvable Private Address
#define ADDR_TYPE_PRIVATE_RESOLVE      0x03  //!< Generate Resolvable Private Address

    
/*********************************************************************
 * ENUMS
 */
typedef enum {
	GAP_STATE_IDLE,
	GAP_STATE_INIT_BD_ADDR,
	GAP_STATE_INIT_BUFFER_SIZE,
	GAP_STATE_INIT_READY,
	GAP_STATE_INIT_ERROR,
} gap_initState_t;



/*
 * Definition for GAP Information base item id
 */
enum {
    /* General ID */
    GAP_ID_ADV_DATA                   = 0x00,
    GAP_ID_SCAN_RSP_DATA,
    GAP_ID_ADV_TYPE,
    GAP_ID_ADV_CHANNEL_MAP,
    GAP_ID_ADV_FILTER_POLICY,
	GAP_ID_ADV_INTERVAL_MIN,
	GAP_ID_ADV_INTERVAL_MAX,
	GAP_ID_ADV_DURATION,
    GAP_ID_DIR_ADDR_TYPE,
    GAP_ID_DIR_ADDRESS,

    GAP_ID_CONNECTION_HANDLE,
    GAP_ID_MIN_CONN_INTERVAL,
    GAP_ID_MAX_CONN_INTERVAL,
    GAP_ID_SLAVE_LATENCY,
    GAP_ID_TIMEOUT_MULTIPLIER,
    GAP_ID_CONN_INTERVAL,

};





/*********************************************************************
 * TYPES
 */

typedef struct {
    u8 advData[MAX_GAP_ADVERTISING_DATA_LEN];
    u8 scanRspData[MAX_GAP_ADVERTISING_DATA_LEN];
    u8 advType;
    u8 advChanelMap;
    u8 advFilterPolicy;
	u16 advIntervalMin;
	u16 advIntervalMax;
	u32 advDuration;
    
    u8 dirAddrType;
    u8 dirAddr[6];

    u16 connHandle;
    u16 minConnInterval;
    u16 maxConnInterval;
    u16 slaveLatency;
    u16 timeoutMultiplier;
    u16 connInterval;

} gap_ib_t;



typedef struct {
    u8 len;
    u8 payload[1];
} gap_advData_t;


typedef struct {
    u8 len;
    u8 payload[1];
} gap_scanRspData_t;

typedef struct {
	u8 opcode;
	u8 status;
	u8 parameters[1];
} gap_event_t;

typedef struct {
	u8  opcode;
    u8  status;
    u8  bdAddr[BLE_ADDR_LEN];
    u16 le_aclDataPktLen;
    u8  le_totalNumAclDataPkts;
} gap_initDone_evt_t;

typedef struct {
    u8 opcode;
    u8 status;
} gap_advSettingDone_evt_t;

typedef struct {
    u8  opcode;
    u8  status;
	hci_type_t resverd1;
    u8  reserved2;
    u16 connHandle;
    u8  role;
    u8  peerAddrType;
    u8  peerAddr[BLE_ADDR_LEN];
    u16 connInterval;
    u16 slaveLatency;
    u16 supervisionTimeout;
    u8  masterClkAccuracy;
} gap_connectionEstablished_evt_t;

typedef struct {
    u8  opcode;
    u8  status;
    u16 connHandle;
    u8  reason;
} gap_connectionTerminated_evt_t;

typedef struct {
    u8  opcode;
    u8  state;
    u16 connHandle;
} gap_bondDone_evt_t;

typedef struct {
    u8  opcode;
    u8  state;
    u16 connHandle;
} gap_encryptDone_evt_t;


/**
 *  @brief  Definition for GAP callback function to each role.
 */
typedef void (*gap_cb_t)( gap_event_t *pEvt );





/*********************************************************************
 * GLOBAL VARIABLES
 */

// for internal use


/*********************************************************************
 * Public Functions
 */

void gap_reset(void);
void gap_init(gap_cb_t cb);

u8 isLimitedDiscoverableMode(u8* advData);

/*********************************************************************
 * @fn      gap_bondDone_evt
 *
 * @brief   Send the GAP bond done event to upper layer
 *
 * @param   level - security level of the bond
 *
 * @return  None
 */
void gap_bondDone_evt(u8 state, u16 connHandle);

/*********************************************************************
 * @fn      gap_encryptDone_evt
 *
 * @brief   Send the GAP encrypt done event to upper layer
 *
 * @param   level - security level of the bond
 *
 * @return  None
 */
void gap_encryptDone_evt(u8 state, u16 connHandle);