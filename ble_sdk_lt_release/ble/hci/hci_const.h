#pragma once

#include "../../proj/tl_common.h"


/*********************************************************************
 * CONSTANTS
 */


// LE Event mask - last octet
#define HCI_LE_EVT_MASK_NONE                                         0x00
#define HCI_LE_EVT_MASK_CONNECTION_COMPLETE                          0x01
#define HCI_LE_EVT_MASK_ADVERTISING_REPORT                           0x02
#define HCI_LE_EVT_MASK_CONNECTION_UPDATE_COMPLETE                   0x04
#define HCI_LE_EVT_MASK_READ_REMOTE_FEATURE                          0x08
#define HCI_LE_EVT_MASK_LONG_TERM_KEY_REQUEST                        0x10
#define HCI_LE_EVT_MASK_DEFAULT                                      0x1F



/****Events****/
#define HCI_CMD_DISCONNECTION_COMPLETE                               0x05
#define HCI_EVT_ENCRYPTION_CHANGE                                    0x08
#define HCI_EVT_READ_REMOTE_VER_INFO_COMPLETE                        0x0C
#define HCI_EVT_CMD_COMPLETE                                         0x0E
#define HCI_EVT_CMD_STATUS                                           0x0F
#define HCI_EVT_HW_ERROR                                             0x10
#define HCI_EVT_NUM_OF_COMPLETE_PACKETS                              0x13
#define HCI_EVT_DATA_BUF_OVERFLOW                                    0x1A
#define HCI_EVT_ENCRYPTION_KEY_REFRESH                               0x30
#define HCI_EVT_LE_META                                              0x3E

// LE Meta Event Codes
#define HCI_EVT_LE_CONNECTION_COMPLETE                               0x01
#define HCI_EVT_LE_ADVERTISING_REPORT                                0x02
#define HCI_EVT_LE_CONNECTION_UPDATE_COMPLETE                        0x03
#define HCI_EVT_LE_READ_REMOTE_USED_FEATURES_COMPLETE                0x04
#define HCI_EVT_LE_LONG_TERM_KEY_REQUESTED                           0x05

/****Commands****/

// Link Control Command
#define HCI_CMD_DISCONNECT                                           0x0406
#define HCI_CMD_READ_REMOTE_VER_INFO                                 0x041D

// Controller & Baseband Commands
#define HCI_CMD_SET_EVENT_MASK                                       0x0C01
#define HCI_CMD_RESET                                                0x0C03
#define HCI_CMD_READ_TX_POWER_LEVEL                                  0x0C2D
#define HCI_CMD_SET_CONTROLLER_TO_HOST_FLOW_CTRL                     0x0C31
#define HCI_CMD_HOST_BUF_SIZE                                        0x0C33
#define HCI_CMD_HOST_NUM_OF_COMPLETE_PACKETS                         0x0C35

// Information Parameters
#define HCI_CMD_READ_LOCAL_VER_INFO                                  0x1001
#define HCI_CMD_READ_LOCAL_SUPPORTED_CMDS                            0x1002
#define HCI_CMD_READ_LOCAL_SUPPORTED_FEATURES                        0x1003
#define HCI_CMD_READ_BD_ADDR                                         0x1009

// Status Parameters
#define HCI_CMD_READ_RSSI                                            0x1405

// LE Controller Commands
#define HCI_CMD_LE_SET_EVENT_MASK                                    0x2001
#define HCI_CMD_LE_READ_BUF_SIZE                                     0x2002
#define HCI_CMD_LE_READ_LOCAL_SUPPORTED_FEATURES                     0x2003
#define HCI_CMD_LE_SET_RANDOM_ADDR                                   0x2005
#define HCI_CMD_LE_SET_ADVERTISE_PARAMETERS                          0x2006
#define HCI_CMD_LE_READ_ADVERTISING_CHANNEL_TX_POWER                 0x2007
#define HCI_CMD_LE_SET_ADVERTISE_DATA                                0x2008
#define HCI_CMD_LE_SET_SCAN_RSP_DATA                                 0x2009
#define HCI_CMD_LE_SET_ADVERTISE_ENABLE                              0x200A
#define HCI_CMD_LE_SET_SCAN_PARAMETERS                               0x200B
#define HCI_CMD_LE_SET_SCAN_ENABLE                                   0x200C
#define HCI_CMD_LE_CREATE_CONNECTION                                 0x200D
#define HCI_CMD_LE_CREATE_CONNECTION_CANCEL                          0x200E
#define HCI_CMD_LE_READ_WHITE_LIST_SIZE                              0x200F
#define HCI_CMD_LE_CLEAR_WHITE_LIST                                  0x2010
#define HCI_CMD_LE_ADD_DEVICE_TO_WHITE_LIST                          0x2011
#define HCI_CMD_LE_REMOVE_DEVICE_FROM_WL                             0x2012
#define HCI_CMD_LE_CONNECTION_UPDATE                                 0x2013
#define HCI_CMD_LE_SET_HOST_CHANNEL_CLASSIFICATION                   0x2014
#define HCI_CMD_LE_READ_CHANNEL_MAP                                  0x2015
#define HCI_CMD_LE_READ_REMOTE_USED_FEATURES                         0x2016
#define HCI_CMD_LE_ENCRYPT                                           0x2017
#define HCI_CMD_LE_RANDOM                                            0x2018
#define HCI_CMD_LE_START_ENCRYPTION                                  0x2019
#define HCI_CMD_LE_LONG_TERM_KEY_REQUESTED_REPLY                     0x201A
#define HCI_CMD_LE_LONG_TERM_KEY_REQUESTED_NEGATIVE_REPLY            0x201B
#define HCI_CMD_LE_READ_SUPPORTED_STATES                             0x201C
#define HCI_CMD_LE_RECEIVER_TEST                                     0x201D
#define HCI_CMD_LE_TRANSMITTER_TEST                                  0x201E
#define HCI_CMD_LE_TEST_END                                          0x201F


/* HCI_CMD_LE_SET_ADVERTISE_ENABLE parameter */
#define HCI_ADV_DISABLE                                              0x00
#define HCI_ADV_ENABLE                                               0x01

#define HCI_SCAN_DISABLE                                             0x00
#define HCI_SCAN_ENABLE                                              0x01

#define HCI_DUPLICATE_FILTERING_DISABLE                              0x00
#define HCI_DUPLICATE_FILTERING_ENABLE                               0x01




/*********************************************************************
 * ENUMS
 */
