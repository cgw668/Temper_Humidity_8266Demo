#pragma once


/*********************************************************************
 * CONSTANTS
 */

/* Advertisement Type */
typedef enum{
  ADV_TYPE_CONNECTABLE_UNDIRECTED             = 0x00,  // ADV_IND
  ADV_TYPE_CONNECTABLE_DIRECTED_HIGH_DUTY    = 0x01,  // ADV_INDIRECT_IND (high duty cycle)
  ADV_TYPE_NONCONNECTABLE_UNDIRECTED             = 0x02 , // ADV_NONCONN_IND
  ADV_TYPE_SCANNABLE_UNDIRECTED                      = 0x03 , // ADV_SCAN_IND
  ADV_TYPE_CONNECTABLE_DIRECTED_LOW_DUTY     = 0x04,  // ADV_INDIRECT_IND (low duty cycle)
}advertising_type;

/* Filter Policy */
#define ADV_FP_ALLOW_SCAN_ANY_ALLOW_CONN_ANY        0x00  // Process scan and connection requests from all devices
#define ADV_FP_ALLOW_SCAN_WL_ALLOW_CONN_ANY         0x01  // Process connection requests from all devices and only scan requests from devices that are in the White List.
#define ADV_FP_ALLOW_SCAN_ANY_ALLOW_CONN_WL         0x02  // Process scan requests from all devices and only connection requests from devices that are in the White List..
#define ADV_FP_ALLOW_SCAN_WL_ALLOW_CONN_WL          0x03  // Process scan and connection requests only from devices in the White List.

/* Advertising Access Address */
#define ADV_ACCESS_ADDR                             0x8E89BED6

/* Advertising Timing Threshold */
#define ADV_INTERVAL_10MS                           16
#define ADV_INTERVAL_20MS                           32
#define ADV_INTERVAL_30MS                           48
#define ADV_INTERVAL_100MS                          160
#define ADV_INTERVAL_1_28_S                         0x0800
#define ADV_INTERVAL_10_24S                         16384
#define ADV_TIMER(t)                                ((t*5)>>3)  // t*0.625ms
#define ADV_INTERVAL_MIN_DFLT                       ADV_INTERVAL_1_28_S
#define ADV_INTERVAL_MAX_DFLT                       ADV_INTERVAL_1_28_S

/* Advertising Channel Bit Map */
#define ADV_CHANNEL_MAP_MASK_37                     0x01
#define ADV_CHANNEL_MAP_MASK_38                     0x02
#define ADV_CHANNEL_MAP_MASK_39                     0x04
#define ADV_CHANNEL_MAP_ALL                         0x07
#define ADV_CHANNEL_MAP_DFLT                        ADV_CHANNEL_MAP_ALL  

/* Advertising Maximum data length */
#define ADV_MAX_DATA_LEN                            31

/* Advertising Enable/Disable */
#define ADV_ENABLE                                  1
#define ADV_DISABLE                                 0

/*********************************************************************
 * ENUMS
 */





/*********************************************************************
 * TYPES
 */
 
typedef struct {
	u16 intervalMin;      // Minimum advertising interval for non-directed advertising, time = N * 0.625ms
	u16 intervalMax;      // Maximum advertising interval for non-directed advertising, time = N * 0.625ms
	u8  advType;          // Advertising
	u8  ownAddrType;
	u8  directAddrType;
	u8  directAddr[BLE_ADDR_LEN];
	u8  channelMap;
	u8  filterPolicy;
} adv_para_t;


/* Definition for Advertising channel header format */
typedef union {
    struct {
        u16 type:4;
        u16 rfu1:2;
        u16 txAddr:1;
        u16 rxAddr:1;
        u16 len:6;
        u16 rfu2:2;
    } bf;
    u16 value;
} adv_hdr_t;


/* Definition for Advertising channel PDU format */
typedef struct {
    adv_hdr_t hdr;
    u8  payload[1];
} adv_pdu_t;

typedef struct {
	adv_hdr_t hdr;
	u8 payload[1];
} scan_rsp_t;


typedef struct {
	u8  scanType;
	u16 scanInterval; 
	u16 scanWindow;;
	u8  ownAddrType;
	u8  filterPolicy;
} scan_para_t;

typedef struct {
	u16 connIntervalMin;
	u16 connIntervalMax;
	u16 connLatency;
	u16 supervisionTimeout;
	u16 minCELen;
	u16 maxCELen;
} conn_para_t;

typedef struct {
	u16 scanInterval;
	u16 scanWindow;
	u8  initiatorFilterPolicy;
	u8  peerAddrType;
	u8  peerAddr[BLE_ADDR_LEN];
	u8  ownAddrType;
	conn_para_t connPara;
} createConn_t;


/*********************************************************************
 * Public Functions
 */
u8   ll_adv_setPara(adv_para_t* para);
u8   ll_adv_setData(u8 *data, u8 len);
u8   ll_adv_setEnable(u8 fEnable);



u8   ll_scan_setRspData(u8 *data, u8 len);
u8   ll_scan_setPara(scan_para_t* para);
u8   ll_scan_setEnable(u8 fEnable, u8 filterDuplicates);

u8   ll_connection_create(createConn_t *connPara);
u8   ll_connection_createCancel(void);
u8   ll_connection_update(conn_para_t* para);

// Internal use
void ll_adv_phyTimeoutHandler(u8* arg);
void ll_adv_rxHandler(u8* arg);
void ll_slave_brxDoneHandler(u8* arg);
void ll_adv_phyCmdDoneHandler(u8* arg);
void ll_channelTable_calc(u8* channelMap);
u8   ll_conn_getNextChannel(u8* channelMap, u8 curCh, u8 hop);