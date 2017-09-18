#ifndef _RF_BLE_LINK_LAYER_FRAME_H_
#define _RF_BLE_LINK_LAYER_FRAME_H_

#include "../../proj/tl_common.h"

////////////////////////////////////////////////////////////////
// ble mode
////////////////////////////////////////////////////////////////

enum {
	FLG_BLE_ADV_IND				= 0,
	FLG_BLE_ADV_DIRECT_IND		= 1,
	FLG_BLE_ADV_NONCONN_IND		= 2,
	FLG_BLE_SCAN_REQ			= 3,
	FLG_BLE_SCAN_RSP			= 4,
	FLG_BLE_CONNECT_REQ			= 5,
	FLG_BLE_ADV_DISCOVER_IND	= 6,
	FLG_BLE_TXADR_RANDOM		= BIT(6),
	FLG_BLE_RXADR_RANDOM		= BIT(7),

	FLG_BLE_LIGHT_ADV			= FLG_BLE_ADV_IND,
	FLG_BLE_LIGHT_SCAN_REQ		= FLG_BLE_SCAN_REQ,
	FLG_BLE_LIGHT_SCAN_RSP		= FLG_BLE_SCAN_RSP,
	FLG_BLE_LIGHT_CONNECT_REQ	= FLG_BLE_CONNECT_REQ,
	FLG_BLE_LIGHT_BROADCAST		= FLG_BLE_ADV_NONCONN_IND,
	FLG_BLE_LIGHT_DATA			= 0x01,
	FLG_BLE_LIGHT_DATA_RSP		= 0x01,

	FLG_SYS_LINK_IDLE					= 0,
	FLG_SYS_LINK_ADV					= 1,
	//FLG_SYS_LINK_SCAN					= 2,
	FLG_SYS_LINK_WAKEUP					= 3,
	FLG_SYS_LINK_LISTEN					= 4,
	FLG_SYS_LINK_BROADCAST				= 5,
	FLG_SYS_LINK_CONNECTTING			= 6,
	FLG_SYS_LINK_CONNECTED				= 7,
	FLG_SYS_LINK_BRIDGE					= FLG_SYS_LINK_BROADCAST,

	FLG_SYS_LINK_START					= 8,
	FLG_SYS_LINK_LOST					= 9,
	FLG_SYS_LINK_STOP					= 10,
	FLG_SYS_LINK_MISSING				= 11,
	FLG_SYS_DEVICE_FOUND				= 0x40,
};

typedef struct{
	u32 dma_len;            //won't be a fixed number as previous, should adjust with the mouse package number

	u8	type;				//RA(1)_TA(1)_RFU(2)_TYPE(4)
	u8  rf_len;				//LEN(6)_RFU(2)
	u8	advA[6];			//address
	u8	data[30];			//0-31 byte
}rf_packet_adv_ind_t;

typedef struct{
	u32 dma_len;            //won't be a fixed number as previous, should adjust with the mouse package number

	u8	type;				//RA(1)_TA(1)_RFU(2)_TYPE(4)
	u8  rf_len;				//LEN(6)_RFU(2)
	u8	advA[6];			//address
	u8	data[30];			//0-31 byte
}rf_packet_adv_ind_module_t;

typedef struct{
	u32 dma_len;            //won't be a fixed number as previous, should adjust with the mouse package number

	u8	type;				//RFU(3)_MD(1)_SN(1)_NESN(1)-LLID(2)
	u8  rf_len;				//LEN(5)_RFU(3)

	u16	l2cap;				//0x17
	u16	chanid;				//0x04,

	u16	op;
}rf_packet_ll_write_rsp_t;

typedef struct{
	u32 dma_len;            //won't be a fixed number as previous, should adjust with the mouse package number

	u8	type;				//RA(1)_TA(1)_RFU(2)_TYPE(4)
	u8  rf_len;				//LEN(6)_RFU(2)
	u8	advA[6];			//address
	u8	initA[6];			//init address

}rf_packet_adv_dir_ind_t;

typedef struct{
	u32 dma_len;            //won't be a fixed number as previous, should adjust with the mouse package number

	u8	type;				//RA(1)_TA(1)_RFU(2)_TYPE(4)
	u8  rf_len;				//LEN(6)_RFU(2)
	u8	scanA[6];			//
	u8	advA[6];			//

}rf_packet_scan_req_t;

typedef struct{
	u32 dma_len;            //won't be a fixed number as previous, should adjust with the mouse package number

	u8	type;				//RA(1)_TA(1)_RFU(2)_TYPE(4)
	u8  rf_len;				//LEN(6)_RFU(2)
	u8	advA[6];			//address
	u8	data[30];			//0-31 byte

}rf_packet_scan_rsp_t;

typedef struct{
	u32 dma_len;            //won't be a fixed number as previous, should adjust with the mouse package number
	u8	type;				//RA(1)_TA(1)_RFU(2)_TYPE(4): connect request PDU
	u8  rf_len;				//LEN(6)_RFU(2)
	u8	scanA[6];			//
	u8	advA[6];			//
	u8	aa[4];				// access code
	u8	crcinit[3];
	u8	wsize;
	u16	woffset;
	u16 interval;
	u16 latency;
	u16 timeout;
	u8	chm[5];
	u8	hop;				//sca(3)_hop(5)
}rf_packet_ll_init_t;

typedef struct{
	u32 dma_len;            //won't be a fixed number as previous, should adjust with the mouse package number
	u8	type;				//RA(1)_TA(1)_RFU(2)_TYPE(4): connect request PDU
	u8  rf_len;				//LEN(6)_RFU(2)
	u8	scanA[6];			//
	u8	advA[6];			//
	u8	aa[4];				// access code
	u8	crcinit[3];
	u8	wsize;
	u16	woffset;
	u16 interval;
	u16 latency;
	u16 timeout;
	u8	chm[5];
	u8	hop;				//sca(3)_hop(5)
}rf_packet_ll_broadcast_t;

/*
LLID(2) - NESN(1) - SN(1) - MD(1) - RFU(3) - Length(5) - RFU(3)
*/


typedef struct{
	u32 dma_len;            //won't be a fixed number as previous, should adjust with the mouse package number

	u8	type;				//RFU(3)_MD(1)_SN(1)_NESN(1)-LLID(2)
	u8  rf_len;				//LEN(5)_RFU(3)

	u16	l2cap;				//0x17
	u16	chanid;				//0x04,

	u8	att;				//0x12 for master; 0x1b for slave
	u8	hl;					// assigned by master
	u8	hh;					//
	u8	sno;

	u8	ctype;
	u8	cmd[18];				//byte
}rf_packet_ll_write_data_t;


typedef struct{
	u32 dma_len;            //won't be a fixed number as previous, should adjust with the mouse package number

	u8	type;				//RFU(3)_MD(1)_SN(1)_NESN(1)-LLID(2)
	u8  rf_len;				//LEN(5)_RFU(3)

	u16	l2cap;				//0x17
	u16	chanid;				//0x04,

	u8	att;				//0x12 for master; 0x1b for slave
	u8	hl;				// assigned by master
	u8	hh;				//
	u8	init;
	u8	dat[14];
}rf_packet_ll_data_rsp_t;

#define		FLG_RF_RC_DATA			0xccdd
typedef struct{
	u32 dma_len;            //won't be a fixed number as previous, should adjust with the mouse package number

	u8	type;				//RFU(3)_MD(1)_SN(1)_NESN(1)-LLID(2)
	u8  rf_len;				//LEN(5)_RFU(3)
	u16	flag;

	u32	src_id;

	u8	att;				//0x12 for master; 0x1b for slave
	u8	hl;					// assigned by master
	u8	hh;					//
	u8	sno;

	u16	nid;				// network ID
	u16	group;

	u32	dst_id;

	u8	cmd[11];			//byte
							// 10 xx xx xx xx xx xx		=> light on
							// 11 xx xx xx xx xx xx		=> light off
							// 12 rr gg bb ww uu vv		=> set

	//u32	mic[4];			//optional
}rf_packet_ll_rc_data_t;

#endif
