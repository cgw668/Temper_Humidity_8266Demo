#pragma once

#include "../../proj/tl_common.h"


/*********************************************************************
 * CONSTANTS
 */

/*
 * Definition for GAP Event Code
 */

typedef struct {
    u16 connHandle;
    u8 publicAddr[6];
    u8 privateAddr[6];
    u16 state;
} gap_bondRecord_t;



/*********************************************************************
 * @fn      gapPairingCompleteCb
 *
 * @brief   callback function for pairing complete 
 *
 * @param   None
 *
 * @return  None
 */
u16 gap_resolveAddr(u8 addrType, u8 *addr, u8 *resoveAddr);


/*********************************************************************
 * @fn      gap_getBondState
 *
 * @brief   callback function for pairing complete 
 *
 * @param   None
 *
 * @return  None
 */ 
u16 gap_getBondState(u16 connHandle);

/*********************************************************************
 * @fn      gap_resetBondTable
 *
 * @brief   reset addr table
 *
 * @param   None
 *
 * @return  None
 */ 
void gap_resetBondTable(void);


/*********************************************************************
 * @fn      gap_getFreeBondRecordEntry
 *
 * @brief   get the free addr record entry
 *
 * @param   None
 *
 * @return  None
 */ 
gap_bondRecord_t *gap_getFreeBondRecordEntry(void);

/*********************************************************************
 * @fn      gap_findBondRecordByAddr
 *
 * @brief   callback function for pairing complete 
 *
 * @param   None
 *
 * @return  None
 */ 
gap_bondRecord_t *gap_findBondRecordEntry(u16 connHandle);

/*********************************************************************
 * @fn      gap_getBondCount
 *
 * @brief   get bond dev count 
 *
 * @param   None
 *
 * @return  None
 */ 
u16 gap_getBondCount(void);