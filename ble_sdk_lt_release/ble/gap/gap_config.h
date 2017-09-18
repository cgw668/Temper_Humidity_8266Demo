#pragma once

#include "../../proj/tl_common.h"


/*********************************************************************
 * CONSTANTS
 */


/**
 *  @brief  Definition for GAP Profile Roles
 */
#define GAP_ROLE_BROADCASTER        1
#define GAP_ROLE_OBSERVER           2
#define GAP_ROLE_PERIPHERAL         4
#define GAP_ROLE_CENTRAL            8


/*********************************************************************
 * Following configuration is for user to configure
 */

/**
 *  @brief  Definition for Device ROLE
 */
#define DEVICE_ROLE                 GAP_ROLE_PERIPHERAL

/**
 *  @brief  Definition for Device Address Mode
 */
#define DEVICE_ADDR_MODE            BLE_ADDR_PUBLIC
//#define DEVICE_ADDR_MODE            BLE_ADDR_RANDOM

/**
 *  @brief  User Configured Definition for Peripheral Device
 */
#if (DEVICE_ROLE == GAP_ROLE_PERIPHERAL)

	/**
	 *  @brief  Definition for Discovery Mode 
	 */
    #define GAP_DISCOVERY_MODE_NON                                1  // M
    //#define GAP_DISCOVERY_MODE_LIMITED                          1  // O
    #define GAP_DISCOVERY_MODE_GENERAL                            1  // O 


	/**
	 *  @brief  Definition for Connection Mode and Procedures
	 */
	#define GAP_CONNECTION_MODE_NON                               1  // M
	//#define GAP_CONNECTION_MODE_DIRECTED                        1  // O
	#define GAP_CONNECTION_MODE_UNDIRECTED                        1  // M
	#define GAP_CONNECTION_MODE_NON                               1
	#define GAP_CONNECTION_PROCDURE_PARA_UPDATE                   1  // O
	#define GAP_CONNECTION_PROCDURE_TERMINATE_CONNECTION          1  // M


	/**
	 *  @brief  Definition for Bonding Mode and Procedure 
	 */
    #define GAP_BONDING_MODE_NON                                  1  // M
    //#define GAP_BONDING_MODE                                    1  // O
    //#define GAP_DISCOVERY_PROCEDURE                             1  // O 

#endif  /* DEVICE_ROLE == GAP_ROLE_PERIPHERAL */


/*********************************************************************
 * Following configuration is calcualted automatically
 */

/**
 *  @brief  Link layer features configuration for Peripheral
 */
#if (DEVICE_ROLE == GAP_ROLE_PERIPHERAL)

	/**
	 *  @brief  Definition for supported Link Layer States
	 */
	#define LL_FEATURE_ADVERTISING_STATE                          1
	#define LL_FEATURE_SCANNING_STATE                             0
	#define LL_FEATURE_INITIATING_STATE                           0
	#define LL_FEATURE_MASTER_ROLE                                0
	#define LL_FEATURE_SLAVE_ROLE                                 1

	/**
	 *  @brief  Definition for supported Link Layer Advertising events
	 */
	#if (LL_FEATURE_ADVERTISING_STATE)
		#define LL_FEATURE_CONNECTABLE_UNDIRECTED_EVENT           1
		#define LL_FEATURE_CONNECTABLE_DIRECTED_EVENT             1
		#define LL_FEATURE_NON_CONNECTABLE_UNDIRECTED_EVENT       1
		#define LL_FEATURE_SCANNABLE_UNDIRECTED_EVENT             1
	#endif  /* LL_FEATURE_ADVERTISING_STATE */


#endif  /* DEVICE_ROLE == GAP_ROLE_PERIPHERAL */


/**
 *  @brief  Link layer features configuration for Broadcaster
 */
#if (DEVICE_ROLE == GAP_ROLE_BROADCASTER)

	/**
	 *  @brief  Definition for supported Link Layer States
	 */
	#define LL_FEATURE_ADVERTISING_STATE                          1
	#define LL_FEATURE_SCANNING_STATE                             0
	#define LL_FEATURE_INITIATING_STATE                           0
	#define LL_FEATURE_MASTER_ROLE                                0
	#define LL_FEATURE_SLAVE_ROLE                                 0

	/**
	 *  @brief  Definition for supported Link Layer Advertising events
	 */
	#if (LL_FEATURE_ADVERTISING_STATE)
		#define LL_FEATURE_CONNECTABLE_UNDIRECTED_EVENT           0
		#define LL_FEATURE_CONNECTABLE_DIRECTED_EVENT             0
		#define LL_FEATURE_NON_CONNECTABLE_UNDIRECTED_EVENT       1
		#define LL_FEATURE_SCANNABLE_UNDIRECTED_EVENT             0
	#endif  /* LL_FEATURE_ADVERTISING_STATE */

#endif  /* DEVICE_ROLE == GAP_ROLE_BROADCASTER */


/** @} end of group GAP_PERIPHERAL_Fountions */

/** @} end of group GAP_PERIPHERAL_Module */

/** @} end of group TELINK_BLE_STACK */