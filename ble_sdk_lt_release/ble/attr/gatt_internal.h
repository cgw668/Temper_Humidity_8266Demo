
#pragma once
#include "../hci/hci_include.h"

#define MEMCPY16(ptr, val)  					\
				do {              				\
					u8 *_dst = (u8 *)(ptr);    	\
					u8 *_src = (u8 *)(val);		\
					memcpy(_dst, _src, 2);		\
				} while (0)

#define MEMCPY32(ptr, val)   					\
				do {              				\
					u8 *_dst = (u8 *)(ptr);    	\
					u8 *_src = (u8 *)(val);		\
					memcpy(_dst, _src, 4);		\
				} while (0)
#define MEMCPY64(ptr, val)   					\
				do {              				\
					u8 *_dst = (u8 *)(ptr);    	\
					u8 *_src = (u8 *)(val);		\
					memcpy(_dst, _src, 8);		\
				} while (0)



typedef ble_sts_t (*gatt_pduHandlerFunc_t)(u16 connHandle, u8 *pData);

enum {
    CLIENT_CB,
    SERVER_CB
};



void gatt_registerCb(gatt_pduHandlerFunc_t pduHandler, u8 flag);

ble_sts_t gatt_receiveData(u8 type, u16 connHandle, u16 size, u8 *pHciData);




