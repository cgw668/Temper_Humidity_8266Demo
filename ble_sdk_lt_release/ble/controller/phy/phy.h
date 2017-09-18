#pragma once


/*********************************************************************
 * CONSTANTS
 */

/**
 *  @brief  Definition for Maximum white list size
 */
#define PHY_ACCESS_ADDR_LEN        4

/**
 *  @brief  Definition for flag which indicate it's a TX buffer or RX buffer
 */
#define PHY_TX                     0
#define PHY_RX                     1

/**
 *  @brief  Definition for maximum RX length
 */
#define PHY_MAX_RX_LEN             120

/**
 *  @brief  Definition for BASEBAND working mode
 */
#define BB_MODE_BTX                0x81
#define BB_MODE_BRX                0x82
#define BB_MODE_STX                0x85
#define BB_MODE_SRX                0x86
#define BB_MODE_STR                0x87
#define BB_MODE_SRT                0x88
#define BB_MODE_STOP               0x80

/**
 *  @brief  Definition for maximum command white list table size
 */
#define PHY_MAX_CMD_WHITE_LIST_SIZE  3


/*********************************************************************
 * ENUMS
 */

enum {
    PHY_ID_CHANNEL,
    PHY_ID_POWER,
    PHY_ID_RX_ONOFF,
    PHY_ID_ACCESS_ADDR,
    PHY_ID_WL_MODE
};


/*********************************************************************
 * TYPES
 */

/**
 *  @brief  Definition command action function
 */
typedef void ( *phy_cmdAction_t)(u8* pCmd, u8 len);

/**
 *  @brief  Definition for specific RX callback function type
 */
typedef void ( *phy_specificRxCb_t)(u8* rxbuf);

/**
 *  @brief  Definition for command white list entry
 */
typedef struct {
    u8                cmd;
    phy_cmdAction_t   action;
} phy_cmdWhiteList_entry_t;

/**
 *  @brief  Definition for command white list table
 */
typedef struct {
    phy_cmdWhiteList_entry_t entries[PHY_MAX_CMD_WHITE_LIST_SIZE];
    u8 curNum;
} phy_cmdWhiteListTbl_t;

/**
 *  @brief  Definition for RX command callback
 */
typedef void ( *phy_rxCB_t )( void* arg );

/**
 *  @brief  Definition for connection request
 */
typedef struct {
    u8 initA[6];
    u8 advA[6];
    u32 acceAddr;
    u8 crcInit[3];
    u8 winSize;
    u16 winOffset;
    u16 interval;
    u16 latency;
    u16 timeout;
    u8 chM[5];
    u8 hop_sca;
} ll_connectionReq_t;


/*********************************************************************
 * GLOBAL VARIABLES
 */



/*********************************************************************
 * Public Functions
 */

#define PHY_PACKET_RX_TIMESTAMP(p)               (*(u32*)(&p[8]))

/**
 *  @brief  API to Enable BRX first time out mode
 */
#define PHY_ENABLE_RX_FIRST_TIMEOUT()            write_reg8(0xf03, read_reg8(0xf03)|0x02)

/**
 *  @brief  API to Disable BRX first time out mode
 */
#define PHY_DISABLE_RX_FIRST_TIMEOUT()           write_reg8(0xf03, read_reg8(0xf03)&0xfd)

/**
 *  @brief  API to Enable RX time out mode
 */
#define PHY_ENABLE_RX_TIMEOUT()                  write_reg8(0xf03, read_reg8(0xf03)|0x04)

 /**
 *  @brief  API to Disable RX time out mode
 */
#define PHY_DISABLE_RX_TIMEOUT()                 write_reg8(0xf03, read_reg8(0xf03)&0xfb)

 /**
 *  @brief  API to Enable continuous 2 CRC errir check
 */
#define PHY_ENABLE_CRC2_CHECK()                  write_reg8(0xf03, read_reg8(0xf03)|0x08)

 /**
 *  @brief  API to Disable continuous 2 CRC errir check
 */
#define PHY_DISABLE_CRC2_CHECK()                 write_reg8(0xf03, read_reg8(0xf03)&0xf7)

/**
 *  @brief  API to set baseband listening duration in system tick
 */
#define PHY_SET_FIRST_TIMEOUT_DURATION(d)        write_reg32(0xf28, d)

/**
 *  @brief  API to set baseband work mode
 */
#define PHY_SET_BB_MODE(m)                       write_reg8(0xf00, m)


#define PHY_ENABLE_MANUAL_RX()                   do{write_reg8(0xf02, 0x45);analog_write(0x06, 0x00);write_reg8(0x428, 0x81);write_reg8(0xf02, 0x65);}while(0);
#define PHY_DISABLE_MANUAL_RX()                  do{analog_write(0x06, 0xfe);write_reg8(0xf02, 0x65); write_reg8(0x428, 0x80);}while(0);


#define PHY_IS_ADVERTISING_CHANNEL(ch)           (ch > 36 && ch < 40)

/**
 *  @brief  API to reset phy layer
 */
void phy_reset(void);

/**
 *  @brief  API to initialize phy layer
 */
void phy_init(void);

/**
 *  @brief  API to set asccess code
 */
void phy_setAccessAddr(u32 accessAddr);

/**
 *  @brief  API to set PHY TXRX buffer
 */
void phy_setBuf(u8 txrx, u8* buf, u8 fifo);

/**
 *  @brief  API to set channel
 */
void phy_setChannel(u8 ch);

/**
 *  @brief  API to send data
 */
void phy_tx(u8 *buf, u8 len);

/**
 *  @brief  API to use schedule mode TRX
 */
void phy_setSchedule(u32 interval, u8 mode);
void phy_scheduleTx(u8* buf, u8 len);

/**
 *  @brief  API to set PHY layer white list command
 *          This API only used in advertising channel
 */
void phy_cmdWhiteList_add(u8 cmd, phy_cmdAction_t func);

/**
 *  @brief  API to reset PHY command white list
 *          This API only used in advertising channel
 */
void phy_cmdWhiteList_clear(void);


/* For internal use */
void phy_setRxSpecificCb(phy_specificRxCb_t cb);



