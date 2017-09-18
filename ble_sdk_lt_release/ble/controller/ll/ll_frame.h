#pragma once

#include "../../../proj/tl_common.h"
#include "../../ble_common.h"


/*********************************************************************
 * CONSTANTS
 */

// Advertise channel PDU Type
#define LL_TYPE_ADV_IND                             0
#define LL_TYPE_ADV_DIRECT_IND                      1
#define LL_TYPE_ADV_NONCONN_IND                     2
#define LL_TYPE_SCAN_REQ                            3
#define LL_TYPE_SCAN_RSP                            4
#define LL_TYPE_CONNNECT_REQ                        5
#define LL_TYPE_ADV_SCAN_IND                        6

// Bitmask of advertising channel header
#define LL_TYPE_MASK                                0x0f
#define LL_TX_ADDR_MASK                             0x40
#define LL_RX_ADDR_MASK                             0x80


// Control PDU Name
#define LL_CONNECTION_UPDATE_REQ                    0x00
#define LL_CHANNEL_MAP_REQ                          0x01
#define LL_TERMINATE_IND                            0x02
#define LL_ENC_REQ                                  0x03
#define LL_ENC_RSP                                  0x04
#define LL_START_ENC_REQ                            0x05
#define LL_START_ENC_RSP                            0x06
#define LL_UNKNOWN_RSP                              0x07
#define LL_FEATURE_REQ                              0x08
#define LL_FEATURE_RSP                              0x09
#define LL_PAUSE_ENC_REQ                            0x0A
#define LL_PAUSE_ENC_RSP                            0x0B
#define LL_VERSION_IND                              0x0C
#define LL_REJECT_IND                               0x0D

// Hop & SCA mask
#define LL_CONNECT_REQ_HOP_MASK                     0x1F
#define LL_CONNECT_REQ_SCA_MASK                     0xE0

#define LL_RF_RESERVED_LEN                          4


// LL Header Bit Mask
#define LL_HDR_LLID_MASK                            0x03
#define LL_HDR_NESN_MASK                            0x04
#define LL_HDR_SN_MASK                              0x08
#define LL_HDR_MD_MASK                              0x10

#define LL_PDU_HDR_LLID_RESERVED                    0
#define LL_PDU_HDR_LLID_DATA_PKT_NEXT               1
#define LL_PDU_HDR_LLID_DATA_PKT_FIRST              2
#define LL_PDU_HDR_LLID_CONTROL_PKT                 3

// Macro to judgement the LL data type
#define IS_PACKET_LL_DATA(p)                        ((p & LL_HDR_LLID_MASK) != LL_PDU_HDR_LLID_CONTROL_PKT)
#define IS_PACKET_LL_CTRL(p)                        ((p & LL_HDR_LLID_MASK) == LL_PDU_HDR_LLID_CONTROL_PKT)
#define IS_PACKET_LL_INVALID(p)                     ((p & LL_HDR_LLID_MASK) == LL_PDU_HDR_LLID_RESERVED)


/*********************************************************************
 * ENUMS
 */



/*********************************************************************
 * TYPES
 */
typedef union {
	struct {
		u16 type   :4;
		u16 rfu1   :2;
		u16 txAddr :1;
		u16 rxAddr :1;
		u16 len    :6;
		u16 rfu2   :2;
	} fields;
	u16 value;
} ll_advHdr_t;


typedef struct {
	ll_advHdr_t hdr;
	u8 payload[1];
} ll_advPdu_t;


typedef struct {
    union {
        struct {
            u8 llid    :2;
            u8 nesn    :1;
            u8 sn     :1;
		    u8 md     :1;
		    u8 rfu1   :3;
        } bf;
        u8 byteVal;
    } hdr; 
    u8 len;
} ll_dataHdr_t;

typedef struct {
	ll_dataHdr_t hdr;
	u8 payload[1];
} ll_dataPdu_t;

typedef struct {
	ll_dataHdr_t hdr;
	u8 opcode;
	u8 ctrData[1];
} ll_ctrlPdu_t;


typedef struct {
	u8 advA[BLE_ADDR_LEN];
	u8 advData[1];
} advInd_pld_t;


typedef struct {
	u8 advA[BLE_ADDR_LEN];
	u8 initA[BLE_ADDR_LEN];
} advDirectInd_pld_t;

typedef struct {
	u8 advA[BLE_ADDR_LEN];
	u8 advData[1];
} advNonconnInd_pld_t;

typedef struct {
	u8 advA[BLE_ADDR_LEN];
	u8 advData[1];
} advScanInd_pld_t;

typedef struct {
	u8 advA[BLE_ADDR_LEN];
	u8 initA[BLE_ADDR_LEN];
} scanReq_pld_t;

typedef struct {
	u8 advA[BLE_ADDR_LEN];
	u8 advData[1];
} scanRsp_pld_t;


typedef struct {
    u8 initA[BLE_ADDR_LEN];
    u8 advA[BLE_ADDR_LEN];
    u32 acceAddr;
    u8 crcInit[3];
    u8 winSize;
    u16 winOffset;
    u16 interval;
    u16 latency;
    u16 timeout;
    u8 chM[5];
    u8 hop_sca;
} connReq_pld_t;

typedef connReq_pld_t ll_connPara_t;


typedef struct {
	u8 winSize;
	u16 winOffset;
	u16 interval;
	u16 latency;
	u16 timeout;
	u16 instant;
} ll_connUpdateReq_pld_t;

typedef struct {
	u8 chm[5];
	u16 instant;
} ll_channleMapReq_pld_t;

typedef struct {
	u8 errorCode;
} ll_terminate_pld_t;



/*********************************************************************
 * Public Functions
 */
