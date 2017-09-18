
#pragma  once


#define smp_startGenLTKfromER(er, div, result) smp_startGenKeyCommon(er, div, 0, result)
#define smp_startGenCSRKfromER(er, div, result) smp_startGenKeyCommon(er, div, 1, result)

#define smp_startGenIRKfromIR(ir, result) smp_startGenKeyCommon(ir, 1, 0, result)
#define smp_startGenDHKfromIR(ir, result) smp_startGenKeyCommon(ir, 3, 0, result)


/*********************************************************************
 * @fn      smp_truncateKey
 *
 * @brief   truncate Key size
 *          "For example, if a 128-bit encryption key is 0x123456789ABCDEF0123456789ABCDEF0
 *          and it is reduced to 7 octets (56 bits), then the resulting key is 0x0000000000000000003456789ABCDEF0.""
 *
 * @param   key     -  key
 *
 * @param   max_encSize    - 3 bytes
 *
 * @return  None
 */
void smp_truncateKey(smp_key_t key, u8 max_encSize);


/*********************************************************************
 * @fn      smp_startC1Encyption
 *
 * @brief   this function is used to gengerate step1 of C1 function
 *          c1 (k, r, preq, pres, iat, rat, ia , ra) = e(k, e(k, r XOR p1) XOR p2)
 *          used to generate r XOR p1
 *
 * @param   key -  key
 *
 * @param   r -   r key
 *
 * @param   preq -  7 bytes
 *
 * @param   pres -  7 bytes
 *
 * @param   iat -   iat
 *
 * @param   rat -   rat
 *
 * @return  None
 */
void smp_startC1Encyption(smp_key_t key, smp_key_t r, u8 *preq, u8 *pres, u8 iat, u8 rat, u8 *ia, u8 *ra, smp_key_t result);



/*********************************************************************
 * @fn      smp_s1EncyptionStep1
 *
 * @brief   this function is used to generate the  STK during the pairing 
 *          process
 *
 * @param   key -  key
 *
 * @param   r -   r key
 *
 * @param   r1 -  16 bytes
 *
 * @param   r2 -  16 bytes
 *
 * @return  None
 */
void smp_startS1Encyption(smp_key_t key, u8 *r1, u8 *r2, smp_key_t result);



/*********************************************************************
 * @fn      smp_startGenEvid
 *
 * @brief   used to generate evid
 *          generate r'   r'= padding || r
 *
 * @param   dhk     - dhk key
 *
 * @param   rand    - r 8 bytes
 *
 * @return  None
 */
void smp_startGenEvid(smp_key_t dhk, u8 *randNum, u16 div, smp_key_t result);



/*********************************************************************
 * @fn      smp_recoveryDiv
 *
 * @brief   used to recovery div
 *
 * @param   y  - y 2 bytes
 *
 * @param   div    - div 2 bytes
 *
 * @param   result - evid result (2 bytes)
 *
 * @return  None
 */
void smp_recoveryDiv(smp_key_t dhk, u8 *randNum, u16 ediv, u8 *result);

/*********************************************************************
 * @fn      smp_getKeyResultCommon
 *
 * @brief   used to generate evid
 *          generate r'   r'= padding || r
 *
 * @param   encryptText  - encryption result
 *
 * @param   result - key result (8 bytes)
 *
 * @return  None
 */
void smp_startGenKeyCommon(smp_key_t key, u16 d, u16 r, smp_key_t result);


/*********************************************************************
 * @fn      smp_startLLEncryption
 *
 * @brief   start encryption with stk
 *
 * @param   connHandle     -  connHandle
 *
 * @param   tk     -  key
 *
 * @param   div     -  div
 *
 * @param   pRand     -  pRand
 *
 * @param   keySize    - 3 bytes
 *
 * @return  None
 */
ble_sts_t smp_startLLEncryption(u16 connHandle, smp_key_t tk, u16 div, u8 *pRand, u8 keySize);

/*********************************************************************
 * @fn      smp_startAddrHash
 *
 * @brief   used d1 to generate keys
 *
 * @param   key     -  key
 *
 * @param   r    - 3 bytes
 *
 * @return  None
 */
void smp_startAddrHash(smp_key_t key, u8 *r, smp_key_t result);


/*********************************************************************
 * @fn      smp_startSignEncryption
 *
 * @brief   start encryption with stk
 *
 * @param   connHandle     -  connHandle
 *
 * @param   tk     -  key
 *
 * @param   div     -  div
 *
 * @param   pRand     -  pRand
 *
 * @param   keySize    - 3 bytes
 *
 * @return  None
 */
ble_sts_t smp_startSignEncryption(smp_key_t key, u16 msgLen, u8 *msg, const u8 *signCounter, u8 macLen, u8 *mac);

/*********************************************************************
 * @fn      smp_verifySignature
 *
 * @brief   start encryption with stk
 *
 * @param   key     -  key
 *
 * @param   msgLen     -  msgLen
 *
 * @param   msg     -  msg
 *
 * @param   keySize    - 3 bytes
 *
 * @return  None
 */
ble_sts_t smp_verifySignature(smp_key_t key, u16 msgLen, u8 *msg);

/*********************************************************************
 * @fn      smp_resolvPrivateAddr
 *
 * @brief   used d1 to generate keys
 *
 * @param   key     -  key
 *
 * @param   r    - 3 bytes
 *
 * @return  None
 */
ble_sts_t smp_resolvPrivateAddr(smp_key_t key, u8 *addr);

/*********************************************************************
 * @fn      smp_genResoPrivateAddr
 *
 * @brief   used d1 to generate keys
 *
 * @param   key     -  key
 *
 * @param   r    - 3 bytes
 *
 * @return  None
 */
void smp_genResolvablePrivateAddr(smp_key_t key, u8 *addr);


/*********************************************************************
 * @fn      smp_getIRK
 *
 * @brief   return irk
 *
 * @param   None
 *
 * @return  irk
 */
u8 *smp_getIRK(void);

/*********************************************************************
 * @fn      smp_getCSRK
 *
 * @brief   return csrk
 *
 * @param   None
 *
 * @return  csrk
 */
u8 *smp_getCSRK(void);



/*********************************************************************
 * @fn      smp_getCSRK
 *
 * @brief   return csrk
 *
 * @param   None
 *
 * @return  csrk
 */
u8* smp_getSignCounter(void);

/*********************************************************************
 * @fn      smp_getCSRK
 *
 * @brief   return csrk
 *
 * @param   None
 *
 * @return  csrk
 */
void smp_setSignCounter(u32 cnt);

/*********************************************************************
 * @fn      smp_incSignCounter
 *
 * @brief   increase signature counter by 1
 *
 * @param   None
 *
 * @return  csrk
 */
void smp_incSignCounter(void);

/*********************************************************************
 * @fn      smp_getEncState
 *
 * @brief   get sign key info from key database 
 *
 * @param   connHandle     -  connHandle
 *
 * @return  sign key info
 */
u8 smp_getEncState(u16 connHandle);
