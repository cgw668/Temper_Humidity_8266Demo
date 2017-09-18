#pragma  once

// Authentication requirement flags
#define SMP_AUTHREQ_NO_BONDING 0x00
#define SMP_AUTHREQ_BONDING 0x01
#define SMP_AUTHREQ_MITM_PROTECTION 0x04

// Key distribution flags used by spec
#define SMP_KEYDIST_ENC_KEY 0X01
#define SMP_KEYDIST_ID_KEY  0x02
#define SMP_KEYDIST_SIGN    0x04

// Key distribution flags used internally
#define SMP_KEYDIST_FLAG_ENCRYPTION_INFORMATION       0x01
#define SMP_KEYDIST_FLAG_MASTER_IDENTIFICATION        0x02
#define SMP_KEYDIST_FLAG_IDENTITY_INFORMATION         0x04
#define SMP_KEYDIST_FLAG_IDENTITY_ADDRESS_INFORMATION 0x08
#define SMP_KEYDIST_FLAG_SIGNING_IDENTIFICATION       0x10

// STK Generation Methods
#define SMP_STK_GENERATION_METHOD_JUST_WORKS 0x01
#define SMP_STK_GENERATION_METHOD_OOB        0x02
#define SMP_STK_GENERATION_METHOD_PASSKEY    0x04

// Pairing Failed Reasons
#define SMP_REASON_RESERVED                     0x00
#define SMP_REASON_PASSKEYT_ENTRY_FAILED        0x01
#define SMP_REASON_OOB_NOT_AVAILABLE            0x02
#define SMP_REASON_AUTHENTHICATION_REQUIREMENTS 0x03
#define SMP_REASON_CONFIRM_VALUE_FAILED         0x04
#define SMP_REASON_PAIRING_NOT_SUPPORTED        0x05
#define SMP_REASON_ENCRYPTION_KEY_SIZE          0x06
#define SMP_REASON_COMMAND_NOT_SUPPORTED        0x07
#define SMP_REASON_UNSPECIFIED_REASON           0x08
#define SMP_REASON_REPEATED_ATTEMPTS            0x09


// max and min Encryption Key Size
#define SMP_MAX_ENCRYPTION_KEY_SIZE             16
#define SMP_MIN_ENCRYPTION_KEY_SIZE             7


#define SMP_PAIRING_REQUEST_LEN              7
#define SMP_PAIRING_RESPONSE_LEN             7
#define SMP_PAIRING_CONFIRM_LEN              17
#define SMP_PAIRING_RANDOM_LEN               17
#define SMP_PAIRING_FAILED_LEN               2
#define SMP_ENCRYPTION_INFORMATION_LEN       17
#define SMP_MASTER_IDENTIFICATION_LEN        11
#define SMP_IDENTITY_INFORMATION_LEN         17
#define SMP_IDENTITY_ADDRESS_INFORMATION_LEN 8
#define SMP_SIGNING_INFORMATION_LEN          17
#define SMP_SECURITY_REQUES_LEN              2


#define SMP_PAIRING_TIME_OUT            (3000*1000)

#define SMP_PAIRING_LONG_TIME_OUT       (60 * 1000 * 1000)

#define SMP_TRANSFER_KEY_INTERVAL       (100*1000)

#define CENTRAL_DEVICE_MEMORY_SIZE 1

#define SMP_KEYDIST_SLAVE_ENCKEY                   0x01  //!< Slave Encryption Key
#define SMP_KEYDIST_SLAVE_IDKEY                    0x02  //!< Slave IRK and ID information
#define SMP_KEYDIST_SLAVE_SIGN                     0x04  //!< Slave CSRK
#define SMP_KEYDIST_MASTER_ENCKEY                  0x08  //!< Master Encrypton Key
#define SMP_KEYDIST_MASTER_IDKEY                   0x10  //!< Master IRK and ID information
#define SMP_KEYDIST_MASTER_SIGN                    0x20  //!< Master CSRK


#define SMP_KEYDIST_ENCKEY                       0x01
#define SMP_KEYDIST_IDKEY                        0x02
#define SMP_KEYDIST_SIGN                         0x04


#define SMP_INITIAL_SIGNING_COUNTER              0xffffffff
// Bluetooth Spec definitions
typedef enum {
    SMP_CODE_PAIRING_REQUEST                      = 0x01,
    SMP_CODE_PAIRING_RESPONSE                     = 0x02,       
    SMP_CODE_PAIRING_CONFIRM                      = 0x03,
    SMP_CODE_PAIRING_RANDOM                       = 0x04,
    SMP_CODE_PAIRING_FAILED                       = 0x05,
    SMP_CODE_ENCRYPTION_INFORMATION               = 0x06,
    SMP_CODE_MASTER_IDENTIFICATION                = 0x07,
    SMP_CODE_IDENTITY_INFORMATION                 = 0x08,
    SMP_CODE_IDENTITY_ADDRESS_INFORMATION         = 0x09,
    SMP_CODE_SIGNING_INFORMATION                  = 0x0a,
    SMP_CODE_SECURITY_REQUEST                     = 0x0b,
} smp_cmd_t;


typedef enum {
    DKG_W4_WORKING,
    DKG_CALC_IRK,
    DKG_W4_IRK,
    DKG_CALC_DHK,
    DKG_W4_DHK,
    DKG_READY
} derived_keyGenState_t;

typedef enum {
    RAU_IDLE,
    RAU_GET_RANDOM,
    RAU_W4_RANDOM,
    RAU_GET_ENC,
    RAU_W4_ENC,
    RAU_SET_ADDRESS,
} random_addrUpdate_t;

typedef enum {
    CMAC_IDLE,
    CMAC_CALC_SUBKEYS,
    CMAC_W4_SUBKEYS,
    CMAC_CALC_MI,
    CMAC_W4_MI,
    CMAC_CALC_MLAST,
    CMAC_W4_MLAST
} cmac_state_t;

typedef enum {
    JUST_WORKS,
    PK_RESP_INPUT,  // Initiator displays PK, initiator inputs PK 
    PK_INIT_INPUT,  // Responder displays PK, responder inputs PK
    OK_BOTH_INPUT,  // Only input on both, both input PK
    OOB             // OOB available on both sides
} stk_generationMethod_t;

typedef enum {
    SMP_USER_RESPONSE_IDLE,
    SMP_USER_RESPONSE_PENDING,
    SMP_USER_RESPONSE_CONFIRM,
    SMP_USER_RESPONSE_PASSKEY,
    SMP_USER_RESPONSE_DECLINE
} sm_userResp_t;



// IO Capability Values
typedef enum {
    IO_CAPABILITY_DISPLAY_ONLY = 0,
    IO_CAPABILITY_DISPLAY_YES_NO,
    IO_CAPABILITY_KEYBOARD_ONLY,
    IO_CAPABILITY_NO_INPUT_NO_OUTPUT,
    IO_CAPABILITY_KEYBOARD_DISPLAY, // not used by secure simple pairing
    IO_CAPABILITY_UNKNOWN = 0xff
} io_capability_t;

// Authorization state
typedef enum {
    AUTHORIZATION_UNKNOWN,
    AUTHORIZATION_PENDING,
    AUTHORIZATION_DECLINED,
    AUTHORIZATION_GRANTED
} authorization_state_t;

// horizontal: initiator capabilities
// vertial:    responder capabilities
static const stk_generationMethod_t stk_generation_method[5][5] = {
    { JUST_WORKS,      JUST_WORKS,       PK_INIT_INPUT,   JUST_WORKS,    PK_INIT_INPUT },
    { JUST_WORKS,      JUST_WORKS,       PK_INIT_INPUT,   JUST_WORKS,    PK_INIT_INPUT },
    { PK_RESP_INPUT,   PK_RESP_INPUT,    OK_BOTH_INPUT,   JUST_WORKS,    PK_RESP_INPUT },
    { JUST_WORKS,      JUST_WORKS,       JUST_WORKS,      JUST_WORKS,    JUST_WORKS    },
    { PK_RESP_INPUT,   PK_RESP_INPUT,    PK_INIT_INPUT,   JUST_WORKS,    PK_RESP_INPUT },
};

typedef enum {
    SMP_LTK = 0,
    SMP_CSRK,
    SMP_IRK,
    SMP_DHK,
};

typedef enum {
    SMP_AES_IDLE,
    SMP_AES_W4_DM,
    SMP_AES_W4_D1_TLK,
    SMP_AES_W4_D1_CSRK,
    SMP_AES_W4_D1_IRK,
    SMP_AES_W4_D1_DHK,
    SMP_AES_W4_AH,

    SMP_AES_W4_C1_STEP1,
    SMP_AES_W4_C1_STEP2,
    SMP_AES_W4_S1,
} smp_aesState_t;


typedef enum {
    SMP_STATE_IDLE,                            //0
	SMP_STATE_W4_PAIR_REQ,
    SMP_STATE_W4_PAIR_RESP,
    SMP_STATE_W4_PAIR_CNF,
    SMP_STATE_W4_PAIR_RAND,                   //4
    
    SMP_STATE_W4_USER_RESP,    
    SMP_STATE_W4_KEY_DISTRIBUTION,
    SMP_STATE_W4_START_KEY_DISTRIBUTION,
    SMP_STATE_W4_ENC_INFO,                   //8
    
    SMP_STATE_W4_MASTER_ID,
    SMP_STATE_W4_IDENTITY_INFO,
    SMP_STATE_W4_ID_ADDR_INFO,
    SMP_STATE_W4_SIGN_INFO,
    
    SMP_STATE_NO_USED,
    
} smp_state_t;

#define SMP_AUTH_STATE_AUTHENTICATED       0x04  //! Authenticate requested
#define SMP_AUTH_STATE_BONDING             0x01  //! Bonding requested

#define SMP_PASSKEY_TYPE_INPUT   0x01    //!< Input the passkey
#define SMP_PASSKEY_TYPE_DISPLAY 0x02    //!< Display the passkey


#define SMP_JUST_WORKS_REQUEST                              0xD0
#define SMP_JUST_WORKS_CANCEL                               0xD1 
#define SMP_PASSKEY_DISPLAY_NUMBER                          0xD2
#define SMP_PASSKEY_DISPLAY_CANCEL                          0xD3
#define SMP_PASSKEY_INPUT_NUMBER                            0xD4
#define SMP_PASSKEY_INPUT_CANCEL                            0xD5
#define SMP_IDENTITY_RESOLVING_STARTED                      0xD6
#define SMP_IDENTITY_RESOLVING_FAILED                       0xD7
#define SMP_IDENTITY_RESOLVING_SUCCEEDED                    0xD8
#define SMP_AUTHORIZATION_REQUEST                           0xD9
#define SMP_AUTHORIZATION_RESULT                            0xDA

#define SMP_PAIRING_REQUEST_EVENT                           0xA0