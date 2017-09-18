#pragma once

#include "gatt.h"


/** @addtogroup  GATT_Server_Module GATT server
 *  @{
 */

/** @addtogroup  GATT_Server_Types GATT server types
 *  @{
 */

/**
 *  @brief  Definition for client characteristic configuration change callback function
 */
typedef ble_sts_t (*clientCharChangeFunc_t)(u16 handle);

/**
 *  @brief  Definition for write requset check callback function
 */
typedef ble_sts_t (*writeReqChkFunc_t)(writeReq_t* pReq);

/**
 *  @brief  Definition for authentication callback function
 */
typedef ble_sts_t (*authAttrFunc_t)(void);

/**
 *  @brief  Definition for GATT service attribute
 */

typedef int (*att_readwrite_callback_t)(void* p);
typedef struct attribute
{
  u8  attNum;
  u8  uuidLen;
  u8  attrLen;
  u8  attrMaxLen;
  u8* uuid;
  u8* pAttrValue;
  att_readwrite_callback_t w;
  att_readwrite_callback_t r;
} attribute_t;

/**
 *  @brief  Definition for GATT service callback function table
 */
typedef struct serivceCbFuncTbl
{
  clientCharChangeFunc_t      clientCharChangeCb;
  writeReqChkFunc_t           writeReqChkCb;
  authAttrFunc_t              authAttr;
} serivceCbFuncTbl_t;

/**
 *  @brief  Definition for GATT service information list element
 */
typedef struct serviceInfoList
{
  struct serviceInfoList* next;
  attribute_t* pAttrSet;
  serivceCbFuncTbl_t* serviceCbFuncTbl;
  u16 serviceAttrNum;
} serviceInfoList_t;
/** @} end of group GATT_Server_Types */


/** @addtogroup GATT_Attr_Num_Calc GATT attribute number calculation
 * @{
 */
#define GATT_CALC_ATTR_NUM( attrArray )       (sizeof(attrArray) / sizeof(attribute_t))
/** @} end of group GATT_Attr_Num_Calc */


/** @addtogroup  GATT_Server_Fountions GATT server fountions
 *  @{
 */

/**
 * @brief   Initialize GATT common module for client or server
 *
 * @param   None
 *
 * @return  None
 */
void gatt_serverInit(void);

/**
 * @brief   Initialize GATT common module for client or server
 *
 * @param   None
 *
 * @return  None
 */
ble_sts_t gatt_registerService(serviceInfoList_t *serviceInfo);

/**
 * @brief   Initialize GATT common module for client or server
 *
 * @param   None
 *
 * @return  None
 */
ble_sts_t gatt_handleValueNotification(u16 connHandle, handleValueNoti_t *pNoti);
/** @} end of group GATT_Server_Fountions */

/** @} end of group GATT_Server_Module */


