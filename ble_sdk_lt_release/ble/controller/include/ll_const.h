#pragma once


/* Advertising Channel */
#define CHANNEL_ADV_37                37
#define CAHNNEL_ADV_38                38
#define CHANNEL_ADV_39                39
#define MAX_DATA_CHANNEL_NUM          37

/* Timing Constant */
#define INTER_FRAME_SPACE             150    // 150us


/**
 *  @brief  Definition for Link Layer Feature Support
 */
#define LL_FEATURE_SIZE                                      8
#define LL_FEATURE_MASK_ENCRYPTION                           0x01
#define LL_FEATURE_MASK_CONNECTION_PARA_REQUEST_PROCEDURE    0x02
#define LL_FEATURE_MASK_EXTENDED_REJECT_INDICATION           0x04
#define LL_FEATURE_MASK_SLAVE_INITIATED_FEATURES_EXCHANGE    0x08
#define LL_FEATURE_MASK_LE_PING                              0x10
#define LL_FEATURE_MASK_DEFAULT                              LL_FEATURE_MASK_ENCRYPTION


#define LL_TX_MASTER_TO_SLAVE_DIRECTION                         1
#define LL_TX_SLAVE_TO_MASTER_DIRECTION                         0
#define LL_RX_MASTER_TO_SLAVE_DIRECTION                         1
#define LL_RX_SLAVE_TO_MASTER_DIRECTION                         0
 
#define LL_COMMAND_PACKET                                    0x10 
 
#define L2CAP_FRIST_PKT_H2C              0x00
#define L2CAP_CONTINUING_PKT             0x01
#define L2CAP_FIRST_PKT_C2H              0x02