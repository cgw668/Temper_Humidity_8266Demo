#pragma once
#include "../include/ll_include.h"


/*********************************************************************
 * CONSTANTS
 */

/**
 *  @brief  Definition for Maximum white list size
 */
#define MAX_WHITE_LIST_SIZE                  4

/**
 *  @brief  Definition for the invalid value in the white list table
 */
#define INVALID_WHITE_LIST_ITEM              0xff

/**
 *  @brief  Definition for Marco to detect if a entry is empty
 */
#define EMPTY_WHITE_LIST_ENTRY(type)         (type == INVALID_WHITE_LIST_ITEM)


/*********************************************************************
 * ENUMS
 */



/*********************************************************************
 * TYPES
 */

/**
 *  @brief  Definition for White List Table
 */
typedef struct {
	u8 curNum;
	addr_t wl_devices[MAX_WHITE_LIST_SIZE];
} ll_whiteListTbl_t;



/*********************************************************************
 * GLOBAL VARIABLES
 */
extern ll_whiteListTbl_t ll_whiteList_tbl;


/*********************************************************************
 * Public Functions
 */
void ll_whiteList_init(void);
addr_t* ll_whiteList_search(u8 type, u8* addr);
addr_t* ll_whiteList_searchEmpty(void);


ble_sts_t ll_whiteList_reset(void);
ble_sts_t ll_whiteList_add(u8 type, u8* addr);
ble_sts_t ll_whiteList_delete(u8 type, u8* addr);

ble_sts_t ll_whiteList_getSize(u8* returnSize);