#pragma  once

/*********************************************************************
 * @fn      smp_registerPktHandler
 *
 * @brief   this function is used to register smp callback handler
 *
 * @param   pData - sm_pktHandler_t
 *
 * @return  None
 */
void smp_registerPktHandler(l2cap_cbFn_t cb);


/*********************************************************************
 * @fn      smp_setAcceptedSTKGenerationMethods
 *
 * @brief   this function is used to set the acceptable STK generation methods
 *
 * @param   stkGenMethods - acceptable STK generation methods
 *
 * @return  None
 */
void smp_setAcceptedSTKGenerationMethods(u8 stkGenMethods);


/*********************************************************************
 * @fn      smp_setEncKeySizeRange
 *
 * @brief   this function is used to set the minimun and maximum encryption key size
 *
 * @param   minSize - minimun encryption key size
 *
 * @param   maxSize - maximun encryption key size
 *
 * @return  None
 */
void smp_setEncKeySizeRange(u8 minSize, u8 maxSize);





/*********************************************************************
 * @fn      smp_setKey
 *
 * @brief   set the key
 *
 * @param   dst - the key to set
 *
 * @param   src - the key value from
 *
 * @return  None
 */

void smp_setKey(smp_key_t dst, smp_key_t src);



/*********************************************************************
 * @fn      smp_setRequestSecurity
 *
 * @brief   this function is used to set request security flag
 *
 * @param   enable -
 *
 * @return  None
 */
void smp_handleInvalidCmd(void);

/*********************************************************************
 * @fn      smp_setRequestSecurity
 *
 * @brief   this function is used to set request security flag
 *
 * @param   enable -
 *
 * @return  None
 */
int sm_updateKeyDistributionFlags(u8 key_set);


/*********************************************************************
 * @fn      smp_timeoutTimerCb
 *
 * @brief   time out handler for smp timer
 *
 * @param   arg -
 *
 * @return  -1
 */
int smp_timeoutTimerCb(void *arg);

/*********************************************************************
 * @fn      smp_waitKeyDiscTimerCb
 *
 * @brief   time out handler for smp timer
 *
 * @param   arg -
 *
 * @return  -1
 */
int smp_waitKeyDiscTimerCb(void *arg);

/*********************************************************************
 * @fn      smp_startTimer
 *
 * @brief   this function is used to start the time out timer for sm
 *
 * @param   timeout - timeout value (uS)
 *
 * @return  None
 */
void smp_startTimer(ev_timer_callback_t cb, u32 timeout);

/*********************************************************************
 * @fn      smp_stopTimer
 *
 * @brief   this function is used to start the time out timer for sm
 *
 * @param   timeout - timeout value (uS)
 *
 * @return  None
 */
void smp_stopTimer(void);


/*********************************************************************
 * @fn      smp_timeoutTimerCb
 *
 * @brief   time out handler for smp timer
 *
 * @param   arg -
 *
 * @return  -1
 */
int smp_waitPairFailTimerCb(void *arg);

/*********************************************************************
 * @fn      smp_setGenMethod
 *
 * @brief   this function is used to get then stk generation method
 *
 * @param   None -
 *
 * @return  None
 */
void smp_setGenMethod(u8 slaveHasOob, u8 masterHasOob, u8 slaveAuthReq, u8 masterAuthReq, u8 slaveIoCap, u8 masterIoCap);

/*********************************************************************
 * @fn      smp_sendMsgToUpperLayer
 *
 * @brief   this function is used to send the smp info to the upper layer
 *
 * @param   type -
 *
 * @param   addrType -
 *
 * @param   addr -
 *
 * @param   passkey -
 *
 * @param   index -
 *
 * @return  None
 */
void smp_sendMsgToUpperLayer(u8 type, u16 connHandle, u8 len, u8 *data);

/*********************************************************************
 * @fn      smp_sendAuthToUpperLayer
 *
 * @brief   this function is used to send the authentication result to the upper layer
 *
 * @param   type -
 *
 * @param   addrType -
 *
 * @param   addr -
 *
 * @param   passkey -
 *
 * @param   index -
 *
 * @return  None
 */
void smp_sendAuthToUpperLayer(u8 type, u8 addrType, u8 *addr, u8 result);


/*********************************************************************
 * @fn      sm_keyDistributionDone
 *
 * @brief   check the key distribution is done or not
 *
 * @param   None
 *
 * @return  None
 */
bool sm_keyDistributionDone(void);

/*********************************************************************
 * @fn      smp_runStateMachine
 *
 * @brief   check the key distribution is done or not
 *
 * @param   cmdCode  - command code for smp
 * 
 * @param   pData    - hci data
 *
 * @return  ble_sts_t
 */
ble_sts_t smp_runStateMachine(u8 cmdCode, u8 *pData);


/*********************************************************************
 * @fn      smp_sendPairRequest
 *
 * @brief   send pair request cmd
 *
 * @param   connHandle  - connHandle
 *
 * @return  ble_sts_t
 */
ble_sts_t smp_sendPairRequest(u16 connHandle);

/*********************************************************************
 * @fn      smp_sendPairResponse
 *
 * @brief   send pair response cmd
 *
 * @param   connHandle  - connHandle
 *
 * @return  ble_sts_t
 */
ble_sts_t smp_sendPairResponse(u16 connHandle);

/*********************************************************************
 * @fn      smp_sendPairFailed
 *
 * @brief   send pair failed cmd
 *
 * @param   connHandle  - connHandle
 *
 * @param   reason  - failed reason
 *
 * @return  ble_sts_t
 */
ble_sts_t smp_sendPairFailed(u16 connHandle, u8 reason);


/*********************************************************************
 * @fn      smp_sendPairConfirm
 *
 * @brief   send pair confirm cmd
 *
 * @param   connHandle  - connHandle
 *
 * @param   cnf  - confrim value
 *
 * @return  ble_sts_t
 */
ble_sts_t smp_sendPairConfirm(u16 connHandle, u8 *cnf);

/*********************************************************************
 * @fn      smp_sendPairRandom
 *
 * @brief   send pair random cmd
 *
 * @param   connHandle  - connHandle
 *
 * @param   randVal  - random value
 *
 * @return  ble_sts_t
 */
ble_sts_t smp_sendPairRandom(u16 connHandle, u8 *randVal);

/*********************************************************************
 * @fn      smp_sendEncInfo
 *
 * @brief   send encryption infomation cmd
 *
 * @param   connHandle  - connHandle
 *
 * @param   ltk  - ltk value
 *
 * @return  ble_sts_t
 */
ble_sts_t smp_sendEncInfo(u16 connHandle, u8 *ltk);

/*********************************************************************
 * @fn      smp_sendIdInfo
 *
 * @brief   send identify infomation cmd
 *
 * @param   connHandle  - connHandle
 *
 * @param   irk  - irk value
 *
 * @return  ble_sts_t
 */
ble_sts_t smp_sendIdInfo(u16 connHandle, u8 *irk);

/*********************************************************************
 * @fn      smp_sendSecurityReq
 *
 * @brief   send security request
 *
 * @param   connHandle  - connHandle
 *
 * @param   ediv  - ediv
 *
 * @param   rd  - rd value
 *
 * @return  ble_sts_t
 */
ble_sts_t smp_sendMasterIdReq(u16 connHandle, u8 *ediv, u8 *rd);

/*********************************************************************
 * @fn      smp_sendSigInfo
 *
 * @brief   send signing infomation cmd
 *
 * @param   connHandle  - connHandle
 *
 * @param   sigKey  - sigKey value
 *
 * @return  ble_sts_t
 */
ble_sts_t smp_sendSigInfo(u16 connHandle, u8 *sigKey);

/*********************************************************************
 * @fn      smp_sendSecurityReq
 *
 * @brief   send security request cmd
 *
 * @param   connHandle  - connHandle
 *
 * @param   authReq  - authReq
 *
 * @return  ble_sts_t
 */
ble_sts_t smp_sendSecurityReq(u16 connHandle, u8 authReq);

/*********************************************************************
 * @fn      smp_sendIdAddrInfo
 *
 * @brief   send identify address information
 *
 * @param   connHandle  - connHandle
 *
 * @param   addrType  - addrType
 *
 * @param   bdAddr  - bdAddr value
 *
 * @return  ble_sts_t
 */
ble_sts_t smp_sendIdAddrInfo(u16 connHandle, u8 addrType, u8 *bdAddr);



/*********************************************************************
 * @fn      smp_cmac_handleAesReady
 *
 * @brief   handler for AES ready
 *
 * @param   None
 *
 * @return  None
 */
void smp_cmac_handleAesReady(void);

/*********************************************************************
 * @fn      smp_updateParaAfterSendResp
 *
 * @brief   update parameter after send pair response
 *
 * @param   None
 *
 * @return  None
 */
void smp_updateParaAfterSendResp(void);


/*********************************************************************
 * @fn          smp_leftShit
 *
 * @brief       Left shift 128 bits
 * 
 * @param       pInp - input buffer
 * @param       pOutp - output buffer
 *
 * @return      None
 */
void smp_leftShit( u8 *pInp, u8 *pOutp );

/*********************************************************************
 * @fn      sm_keyDistributionDone
 *
 * @brief   check the key distribution is done or not
 *
 * @param   None
 *
 * @return  None
 */
bool sm_keySendDistributionDone(void);

/*********************************************************************
 * @fn      sm_keyDistributionDone
 *
 * @brief   check the key distribution is done or not
 *
 * @param   None
 *
 * @return  None
 */
bool sm_keyRecvDistributionDone(void);

/*********************************************************************
 * @fn      smp_startPairRequest
 *
 * @brief   send pair request cmd
 *
 * @param   connHandle  - connHandle
 *
 * @return  ble_sts_t
 */
ble_sts_t smp_startPairProcedure(u16 connHandle, u8 initiator, smp_pairingReq_t req, u8 *oob);

/*********************************************************************
 * @fn          smp_updatePassKey
 *
 * @brief       handler for end pairing procedure 
 * 
 * @param       status - status
 *
 * @return      None
 */
ble_sts_t smp_updatePassKey(smp_key_t key, u16 connHandle);

/*********************************************************************
 * @fn          smp_endPairing
 *
 * @brief       handler for end pairing procedure 
 * 
 * @param       status - status
 *
 * @return      None
 */
void smp_endPairing(u8 status);

void smp_freeSmpPara(void);

ble_sts_t smp_handleEncrytionChange(u16 connHandle, u8 reason);

/*********************************************************************
 * @fn          smp_savePeerAddr
 *
 * @brief       save peer address info
 * 
 * @param       status - status
 *
 * @return      None
 */
void smp_savePeerAddr(u8 addrType, u8 *addr);
