#pragma once

/** @addtogroup  TELINK_BLE_STACK TELINK BLE Stack
 *  @{
 */

/** @addtogroup  GAP_ATT_SERVER_Module GAP ATT Server
 *  @{
 */
 
/** @addtogroup  GAP_ATT_SERVER_Constant GAP ATT Server Constants
 *  @{
 */

/** @defgroup GAP_ATT_PROP_BITMAPS_DEFINES GAP ATT Characteristic Properties Bit Fields
 * @{
 */
#define GAP_ATT_PROP_BCAST                  0x01 //!< Permits broadcasts of the Characteristic Value
#define GAP_ATT_PROP_READ                   0x02 //!< Permits reads of the Characteristic Value
#define GAP_ATT_PROP_WRITE_NO_RSP           0x04 //!< Permits writes of the Characteristic Value without response
#define GAP_ATT_PROP_WRITE                  0x08 //!< Permits writes of the Characteristic Value with response
#define GAP_ATT_PROP_NOTIFY                 0x10 //!< Permits notifications of a Characteristic Value without acknowledgement
#define GAP_ATT_PROP_INDICATE               0x20 //!< Permits indications of a Characteristic Value with acknowledgement
#define GAP_ATT_PROP_AUTHEN                 0x40 //!< Permits signed writes to the Characteristic Value
#define GAP_ATT_PROP_EXTENDED               0x80 //!< Additional characteristic properties are defined in the Characteristic Extended Properties Descriptor
/** @} End GAP_ATT_PROP_BITMAPS_DEFINES */
  

/**
 *  @brief  Definition for ATT UUID Size
 */ 
#define ATT_UUID_SIZE                       2

/**
 *  @brief  Definition for length of GAP device name 
 */ 
#define GAP_DEVICE_NAME_LEN                 (20)    

/**
 *  @brief  Definition for default device name length
 */ 
#define DLT_DEVICE_NAME_LEN                 6

/**
 *  @brief  Definition for length of perrference connection parameter 
 */ 
#define GAP_PERI_CONN_PAR_LEN               sizeof(gap_periConnectParams_t)


/** @defgroup GAP_ATT_SERVER_ACCESS_Permissions GAP Attribute Access Permissions
 * @{
 */
#define gattPermitRead( a )              ( (a) & ATT_PERMISSIONS_READ )
#define gattPermitWrite( a )             ( (a) & ATT_PERMISSIONS_WRITE )
#define gattPermitAuthenRead( a )        ( (a) & ATT_PERMISSIONS_AUTHEN_READ )
#define gattPermitAuthenWrite( a )       ( (a) & ATT_PERMISSIONS_AUTHEN_WRITE )
#define gattPermitAuthorRead( a )        ( (a) & ATT_PERMISSIONS_AUTHOR_READ )
#define gattPermitAuthorWrite( a )       ( (a) & ATT_PERMISSIONS_AUTHOR_WRITE )  

/** @} end of group GAP_ATT_SERVER_ACCESS_Permissions */    
    
    
/** @addtogroup  GAP_ATT_SERVER_Types GAP_ATT_SERVER Types
 *  @{
 */
    
/**
 *  @brief  Definition for the types of peripheral connect parameters
 */ 
typedef struct
{
  /** Minimum value for the connection event (interval. 0x0006 - 0x0C80 * 1.25 ms) */
  u16 intervalMin;
  /** Maximum value for the connection event (interval. 0x0006 - 0x0C80 * 1.25 ms) */
  u16 intervalMax;
  /** Number of LL latency connection events (0x0000 - 0x03e8) */
  u16 latency;
  /** Connection Timeout (0x000A - 0x0C80 * 10 ms) */
  u16 timeout;
} gap_periConnectParams_t;

/** @} end of group GAP_ATT_SERVER_Types */  
    
    

extern const u16 primaryServiceUUID; 
extern const u16 characterUUID;
extern const u16 devNameUUID;
extern const u16 appearanceUIID;
extern const u16 periConnParamUUID;

/** @addtogroup  GAP_ATT_Server_Fountions GAP ATT Server APIs
 *  @{
 */

/**
 * @brief   API to add gap service.
 *
 * @param   None
 *
 * @return  Status
 */
ble_sts_t gap_addService(void);

/**
 * @brief      API to find the gap attribute by uuid.
 *
 * @param[in]  uuid - the uuid of this attribute
 * @param[in]  len - length of this attribute 
 *
 * @return     Status
 */
attribute_t *gap_findAttribute(u16 uuid, u8 len);


/** @} end of group GAP_ATT_Server_Fountions */

/** @} end of group GAP_ATT_Server_Module */

/** @} end of group TELINK_BLE_STACK */