
#pragma  once

// Central Device db implemenation using static memory
typedef struct smp_signDb_tag {
    u16 connHandle;
    u8 secState;
    smp_secKey_t ltkInfo;
    smp_secIdInfo_t idInfo;
    smp_secSigInfo_t sigInfo;
} smp_secDb_t;


// Link state flags
#define LINK_NOT_CONNECTED              0x00    // Link isn't connected
#define LINK_CONNECTED                  0x01    // Link is connected
#define LINK_AUTHENTICATED              0x02    // Link is authenticated
#define LINK_BOND                       0x04    // Link is bonded
#define LINK_ENCRYPTED                  0x10    // Link is encrypted


/*********************************************************************
 * @fn      smp_keyTableInit
 *
 * @brief   inti the key database 
 *
 * @param   None
 *
 * @return  None
 */
void smp_keyTableInit(void);

/*********************************************************************
 * @fn      smp_getKeyTableCnt
 *
 * @brief   get the valid key table size 
 *
 * @param   None
 *
 * @return  table size
 */
u8 smp_getKeyTableCnt(void);

/*********************************************************************
 * @fn      smp_getLTKEntryByDiv
 *
 * @brief   find the 
 *
 * @param   key     -  key
 *
 * @param   r    - 3 bytes
 *
 * @return  None
 */
smp_key_t *smp_getLTKEntryByDiv(u16 vid);

/*********************************************************************
 * @fn      smp_getFreeKeyEntry
 *
 * @brief   find a free key entry
 *
 * @return  key entry
 */
smp_secDb_t *smp_getFreeKeyEntry(void);


/*********************************************************************
 * @fn      smp_findEntryByConnHandle
 *
 * @brief   find a key entry in key database by connhandle
 *
 * @param   connHandle
 *
 * @return  entry
 */
smp_secDb_t *smp_findEntryByConnHandle(u16 connHandle);

/*********************************************************************
 * @fn      smp_rmKeyTableEntry
 *
 * @brief   reset key entry in key table
 *
 * @param   connHandle
 *
 * @return  ble_sts_t
 */
ble_sts_t smp_rmKeyTableEntry(u16 connHandle);

/*********************************************************************
 * @fn      smp_getLTKInfo
 *
 * @brief   get ltk key info from key database 
 *
 * @param   connHandle     -  connHandle
 *
 * @return  ltk key info
 */
smp_secKey_t *smp_getLTKInfo(u16 connHandle);

/*********************************************************************
 * @fn      smp_getIdInfo
 *
 * @brief   get id key info from key database 
 *
 * @param   connHandle     -  connHandle
 *
 * @return  id key info
 */
smp_secIdInfo_t *smp_getIdInfo(u16 connHandle);

/*********************************************************************
 * @fn      smp_getSignInfo
 *
 * @brief   get sign key info from key database 
 *
 * @param   connHandle     -  connHandle
 *
 * @return  sign key info
 */
smp_secSigInfo_t *smp_getSignInfo(u16 connHandle);

/*********************************************************************
 * @fn      smp_findEntryByIndex
 *
 * @brief   find a key entry in key database by connhandle
 *
 * @param   connHandle
 *
 * @return  entry
 */
smp_secDb_t *smp_findEntryByIndex(u8 index);