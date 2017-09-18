#pragma once

#include "../../proj/tl_common.h"
#include "gap_const.h"

/*********************************************************************
 * CONSTANTS
 */


#define GAP_LINK_MITM_PROTECTION    0x0  
#define GAP_LINK_IO_CAPABILITIES    0x1  
#define GAP_LINK_OOB_ENABLED        0x2  
#define GAP_LINK_OOB_DATA           0x3  
#define GAP_LINK_KEY_DIST_LIST      0x4  
#define GAP_LINK_DEFAULT_PASSCODE   0x5  
#define GAP_LINK_KEYSIZE            0x6  
#define GAP_LINK_BONDING_ENABLED    0x7  

//#define GAP_LINK_ERASE_ALLBONDS     0x9  
//#define GAP_LINK_AUTO_SYNC_WL       0xD  
//#define GAP_LINK_BOND_COUNT         0xE  
//#define GAP_LINK_PAIRING_MODE       0x0  
//#define GAP_LINK_INITIATE_WAIT      0x1   



 
/*
 * Definition for GAP Event Code
 */

typedef struct {
    /* data */
    u16 connHandle;
    u8 authState;
    smp_secKey_t *ltkInfo;
    smp_secIdInfo_t *idInfo;
    smp_secSigInfo_t *sigInfo;
    smp_pairingReq_t pairReqResp;    
} gap_authPara_t;


/*********************************************************************
 * @fn      gapPairingCompleteCb
 *
 * @brief   callback function for pairing complete 
 *
 * @param   None
 *
 * @return  None
 */
void gapPairingCompleteCb( u8 status, u8 initiator, u16 connHandle, u8 authState, u8 maxKeySize,
            smp_secKey_t *localSecKey, smp_secKey_t *peerSecKey, smp_secIdInfo_t *idInfo, smp_secSigInfo_t *sigInfo);




ble_sts_t gap_updatePassKey(u16 connHandle) ;

/*********************************************************************
 * @fn      gap_satrtBond
 *
 * @brief   start bond after connection
 *
 * @param   None
 *
 * @return  None
 */
ble_sts_t gap_satrtBond(u16 connHandle, u8 addrType, u8 *addr, u8 role);


void gap_linkReset(void);
void gap_buildPairReq(smp_pairingReq_t *pairReq, u8 *oob, u8 addrType);


/*********************************************************************
 * @fn      gap_startBondAfterConnect
 *
 * @brief   start the bonding procedure after connectino
 *
 * @param   connHandle - connection handle
 *
 * @return  None
 */
void gap_startBondAfterConnect(u16 connHandle, u8 peerAddrType, u8 *peerAddr, u8 role);

u8 gap_getMaxKeySize(void);

/*********************************************************************
 * @fn      gap_stopAuthenticate
 *
 * @brief   stop authenticate 
 *
 * @param   None
 *
 * @return  None
 */
ble_sts_t gap_stopAuthenticate(u16 connHandle, u8 reason);

/*********************************************************************
 * @fn      gap_getPeerAddr
 *
 * @brief   get peer address
 *
 * @param   connHandle
 *
 * @return  addr
 */
addr_t *gap_getPeerAddr(u16 connHandle);

void gap_linkHandleDisconnection(u16 connHandle, u8 status, u8 reason);
void gap_linkDisconnectionReq(u16 connHandle, u8 reason);