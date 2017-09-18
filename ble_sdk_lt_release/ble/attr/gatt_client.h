#pragma once
#include "gatt.h"


typedef void (*gattClientCb_t)(attProtocolRspPdu_t* rspPdu);



void gatt_clientInit(void);


ble_sts_t gatt_clientRegister(u16 connHandle, gattClientCb_t rspCbFunc);





