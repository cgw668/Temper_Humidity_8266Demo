#pragma  once


typedef u8 smp_key_t[16];
typedef u8 smp_rand_t[8];

// packet handler
typedef ble_sts_t (*sm_pktHandler_t) (u8 *pData);

typedef u8 (*passCodeCb_t) (u8 addrType, u8 *addr, u16 connHandle, u8 uiIn, u8 uiOut);

typedef struct {
    /* data */
    passCodeCb_t passKeyCb;
} smp_cb_t;

// define smp paring request
typedef struct {
    u8 code;
    u8 ioCapability;
    u8 obbDataFlag;
    u8 authReq;
    u8 maxEncKeySize;
    u8 initiatorKeyDist;
    u8 responderKeyDist;
} smp_pairingReq_t;

// define smp paring response
typedef struct {
    u8 code;
    u8 ioCapability;
    u8 obbDataFlag;
    u8 authReq;
    u8 maxEncKeySize;
    u8 initiatorKeyDist;
    u8 responderKeyDist;
} smp_pairingResp_t;

// define smp paring confirm
typedef struct {
    u8 code;
    u8 cnfVal[16];
} smp_pairingCnf_t;

// define smp paring random
typedef struct {
    u8 code;
    u8 randVal[16];
} smp_pairingRand_t;

// define smp paring random
typedef struct {
    u8 code;
    u8 reason;
} smp_pairingFailed_t;

// define encryption information
typedef struct {
    u8 code;
    u8 ltk[16];
} smp_encInfo_t;


// define master identification
typedef struct {
    u8 code;
    u8 ediv[2];
    u8 rd[8];
} smp_masterId_t;


// define identity information
typedef struct {
    u8 code;
    u8 irk[16];
} smp_idInfo_t;

// define identity address information
typedef struct {
    u8 code;
    addr_t addr;
} smp_idAddrInfo_t;


// define signing information
typedef struct {
    u8 code;
    u8 sigKey[16];
} smp_sigInfo_t;


// define signing information
typedef struct {
    u8 code;
    u8 authReq;
} smp_secReq_t;


// Security Manager 
typedef struct {
    u8   type;   
    addr_t addr;
    u8 msgLen;
    u8 msg[1];
} smp_event_t;

typedef struct {
    sm_pktHandler_t handler;
    smp_state_t curState;
    smp_state_t nextState;
    smp_cmd_t cmd;
} smp_stateMachine_t;


typedef struct {
    u8 pairRand[16];
    u8 pairCnf[16];
} smp_pairingInfo_t;

typedef struct {
    /* data */
    u8 ltk[16];
    u16 ediv;
    u16 div;
    u8 rand[8];
    u8 keySize;
} smp_secKey_t;

typedef struct {
    /* data */
    u8 irk[16];
	addr_t addr;
} smp_secIdInfo_t;

typedef struct {
    /* data */
    u8 csrk[16];
    u32 signCounter;
} smp_secSigInfo_t;



typedef struct {
    u8 initiator;             
    smp_state_t state;
    u16 connHandle;
    
    u8 key_disSendSet;
    u8 key_disRecvSet;
    u8 userResp;
    u8 supporteSTKGenMethod;
        
    
    smp_key_t tk;    
    smp_key_t oobData;
    
    u8 authState;
    smp_pairingReq_t req;
    smp_pairingResp_t resp;
    u8 stk_genMethod;

    smp_key_t stk;
    
    smp_pairingInfo_t *localDev;
    smp_pairingInfo_t *peerDve;
    
    smp_secKey_t *localSecKey;
    smp_secKey_t *peerSecKey;
    
    smp_secIdInfo_t *idInfo;
    smp_secSigInfo_t *sigInfo;
    
    addr_t masterAddr;
    addr_t slaveAddr;
    
} smp_para_t;


#define SMP_SET_STATE(newState)      ( (smp_parameter->state) = newState)
#define SMP_GET_STATE()           ( smp_parameter->state )


#define smp_setAes128Key(key)     smp_setKey(smp_aes128Key, (key))
#define smp_setAes128Plaintext(plaintext)  smp_setKey(smp_aes128Plaintext, (plaintext))
#define READ_NET_32( buffer, pos) ( ((u32) buffer[pos+3]) | (((u32)buffer[pos+2]) << 8) | (((u32)buffer[pos+1]) << 16) | (((u32) buffer[pos])) << 24)


extern u8 smp_max_encKeySize;
extern u8 smp_min_encKeySize;
extern u8 smp_actualEncKeySize;

// default er/ir key for current device.
extern smp_key_t smp_default_er;
extern smp_key_t smp_default_ir;

extern smp_key_t smp_persistent_dhk;
extern smp_key_t smp_persistent_irk;

extern u8 smp_actualEncKeySize;
extern u32 smp_signatureCounter;
extern smp_para_t *smp_parameter;
extern smp_stateMachine_t smp_stateTable[];
extern const u8 smp_stateTblSize;
extern ev_time_event_t smp_timeoutTimer;
extern l2cap_cbFn_t smp_cb;
extern addr_t  smp_perrAddr;

ble_sts_t smp_handlePairReq(u8 *pData);
ble_sts_t smp_handlePairResp(u8 *pData);
ble_sts_t smp_handlePairCnf(u8 *pData);
ble_sts_t smp_handlePairRand (u8 *pData);
ble_sts_t smp_handlePairFailed (u8 *pData);
ble_sts_t smp_handleEncryptInfo(u8 *pData);
ble_sts_t smp_handleMasterIdent(u8 *pData);
ble_sts_t smp_handleIdentInfo (u8 *pData);
ble_sts_t smp_handleIdentAddrInfo(u8 *pData);
ble_sts_t smp_handleSignInfo(u8 *pData);
ble_sts_t smp_security_request(u8 *pData);
void smp_initEncParamter(u16 connHandle, u8 initiator);
ble_sts_t smp_handle_LTK_request(u16 connHandle, u8 *pRand, u16 ediv);
void smp_updateNextStateInDistribut(void *arg);
void smp_pairingDone(void *arg);
int smp_sendKeyDistribution(void *arg);

/*********************************************************************
 * @fn      l2cap_handleSmpPkt
 *
 * @brief   Handle the l2cap security management frame
 *
 * @param   handle - connection to be used.
 * @param   cid - cid.
 * @param   size - the size of the data
 * @param   pData - pointer to packet data.
 *
 * @return  None
 */
ble_sts_t smp_handlePkt(u8 type, u16 handle, u16 size, u8 *pData);

/*********************************************************************
 * @fn      smp_cbFunction
 *
 * @brief   callback function for smp
 *
 * @param   None -
 *
 * @return  None
 */
void smp_cbFunction(void *pData);

/*********************************************************************
 * @fn      smp_init
 *
 * @brief   this function is used to init smp module
 *
 * @param   None -
 *
 * @return  None
 */
void smp_init(void);