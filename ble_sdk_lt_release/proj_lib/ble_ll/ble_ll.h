#ifndef _RF_LIGHT_LINK_LAYER_H_
#define _RF_LIGHT_LINK_LAYER_H_

#include "../../proj/tl_common.h"

#include "ble_common.h"
#include "att.h"
#include "gatt_uuid.h"
#include "service.h"
#include "gatt_server.h"
#include "gap_const.h"

typedef void (*irq_st_func) (void);

#define		SYS_LINK_NORMAL						0
#define		SYS_LINK_STOP_ON_RESPONSE			BIT(0)

#define		MASTER_CONNECT_ANY_DEVICE			BIT(6)

#define			LL_CHANNEL_MAP					{0xff, 0xff, 0xff, 0xff, 0x1f}
//#define			LL_CHANNEL_MAP					{0x0, 0x10, 0x0, 0x0, 0x0}
#define			LL_CHANNEL_HOP					0x0c

//74 4C 69 67 68 74 31 30
#define		TLIGHT_ID0			0x67694c74
#define		TLIGHT_ID1			0x30317468

#define			FLASH_ADR_MAC			0x8000
#define			FLASH_ADR_PAIRING		0x9000
#define			FLASH_ADR_NETWORK		0xa000
#define			FLASH_ADR_CFG			0xb000

#define			LGT_CMD_PAIRING					0x80
#define			LGT_CMD_PAIRING_RESPONSE_REQ	0x81

#define			LGT_CMD_PAIRING_RC_ID_REASE		0xc0
#define			LGT_CMD_PAIRING_LIGHT_OFF		0xc1
#define			LGT_CMD_PAIRING_LIGHT_ON		0xc2
#define			LGT_CMD_PAIRING_LIGHT_SEL		0xc3
#define			LGT_CMD_PAIRING_LIGHT_CONFIRM	0xc4
#define			LGT_CMD_PAIRING_LIGHT_ADD		0xc5
#define			LGT_CMD_PAIRING_LIGHT_DEL		0xc6
#define			LGT_CMD_PAIRING_LIGHT_GET		0xc7
#define			LGT_CMD_PAIRING_LIGHT_ENABLE	0xc8
#define			LGT_CMD_PAIRING_LIGHT_RESPONSE	0xc9
#define			LGT_CMD_PAIRING_LIGHT_STOP		0xcf
#define			LGT_CMD_PAIRING_LIGHT_ADD_ERR	0xd0

#define			LGT_CMD_LIGHT_ON				0x10
#define			LGT_CMD_LIGHT_OFF				0x11
#define			LGT_CMD_LIGHT_SET				0x12
#define			LGT_CMD_LIGHT_SCENE				0x18

#define			LGT_CMD_LUM_UP					0x20
#define			LGT_CMD_LUM_DOWN				0x21
#define			LGT_CMD_SAT_UP					0x22
#define			LGT_CMD_SAT_DOWN				0x23

#define			CUSTOM_DATA_MOUSE				0
#define			CUSTOM_DATA_KEYBOARD			1
#define			CUSTOM_DATA_SOMATIC				2
#define			CUSTOM_DATA_MIC					3


typedef struct{
	u32	src_id;

	u8	att;				//0x12 for master; 0x1b for slave
	u8	hl;					// assigned by master
	u8	hh;					//
	u8	sno;

	u16	nid;				// network ID
	u16	group;

	u32	dst_id;

	u8	cmd[11];			//byte

}	ll_packet_data_t;

typedef struct{
	u8	type;
	u8	len;
	u16	l2cap;
	u16	chanid;				//0x04,
	u8	att;				//0x12 for master; 0x1b for slave
	u8	hl;				// assigned by master
	u8	hh;				//
	u8	sid;
	u8	ctype;			//type of command: mouse/keyboard/mic
	u8	cmd[18];
}rf_custom_dat_t;


typedef struct{
	u32 dma_len;            //won't be a fixed number as previous, should adjust with the mouse package number
	u8	type;				//RFU(3)_MD(1)_SN(1)_NESN(1)-LLID(2)
	u8  rf_len;				//LEN(5)_RFU(3)
	u16	l2capLen;
	u16	chanId;
	u8  opcode;
	u8 handle;
	u8 handle1;
	u8 dat[1];
}rf_packet_att_write_data_t;

typedef struct{
	u32 dma_len;            //won't be a fixed number as previous, should adjust with the mouse package number
	u8	type;				//RFU(3)_MD(1)_SN(1)_NESN(1)-LLID(2)
	u8  rf_len;				//LEN(5)_RFU(3)
	u16	l2capLen;
	u16	chanId;
	u8  opcode;
	u8 dat[1];
}rf_packet_att_read_rsp_t;

typedef struct{
	u32 dma_len;            //won't be a fixed number as previous, should adjust with the mouse package number
	u8	type;				//RFU(3)_MD(1)_SN(1)_NESN(1)-LLID(2)
	u8  rf_len;				//LEN(5)_RFU(3)
	u16	l2capLen;
	u16	chanId;
	u8  opcode;
	u8  len;
	u16 attrHandle;
	u8 attrValue[1];
}rf_packet_att_read_by_type_rsp_t;




typedef struct{
	u32 dma_len;            //won't be a fixed number as previous, should adjust with the mouse package number
	u8	type;				//RFU(3)_MD(1)_SN(1)_NESN(1)-LLID(2)
	u8  rf_len;				//LEN(5)_RFU(3)
	u16	l2capLen;
	u16	chanId;
	u8  opcode;
	u8 	id;
	u16 data_len;
	u16 interval_min;
	u16 interval_max;
	u16 latency;
	u16 timeout;
}rf_packet_conn_para_update_req_t;

static inline int ble_word_match (u8 *ps, u8 * pd)
{
	return (ps[0] == pd[0]) && (ps[1] == pd[1]) && (ps[2] == pd[2]) && (ps[3] == pd[3]);
}

void	ble_set_tick_per_us (int tick);

void	irq_handler(void);
void	irq_ble_master_handler ();
void	irq_ble_slave_handler ();

u8		ble_master_get_sno ();
void	ble_master_set_sno (u8 sno);

void	ble_master_init ();
int		ble_master_start (u32 interval, u32 timeout, void *p_data, int mode);
int		ble_master_command (u8 * p);
void 	ble_master_stop ();
void *	ble_master_get_response ();
int		ble_master_status ();

void	ble_master_set_slave_mac (u8 *p);
int		ble_master_write (u8 *p);
int		ble_master_ll_data (u8 *p, int n);
void	ble_master_set_channel_mask (u8 * p);

void	ble_slave_init (u32 interval, u32 timeout, int sniffer_mode);
void	ble_slave_proc (void);
int		ble_slave_connected ();
void 	ble_set_debug_adv_channel (u8 chn);
void 	ble_slave_set_adv (u8 * tbl_pkt_adv, int n);
void 	ble_slave_set_adv_interval (int ms);
void	ble_slave_set_rsp (u8 * tbl_pkt_rsp, int n);

void	ble_slave_set_att_handler (u8 * pAtt);

//void	ble_event_callback (u8 status, u8 *p, u8 ble_rssi);
void 	ble_slave_data_callback (u8 *);
void	ble_master_data_callback (u8 *p);

int		ble_master_add_tx_packet (u32 p);

int		ble_ll_lastUnmappedCh;
int		ble_ll_channelNum;
u8		ble_ll_channelTable[40];
u8		ble_ll_chn;

unsigned short crc16 (unsigned char *pD, int len);

u8*		ble_slave_p_mac;

#endif
