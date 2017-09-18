
#pragma once

#if (__PROJECT_BLE_MASTER_KMA_DONGLE__)
	#include "../8266_ble_master_kma_dongle/app_config.h"
#elif (__PROJECT_8266_BLE_REMOTE__)
	#include "../8266_ble_remote/app_config.h"
#elif (__PROJECT_8266_HOST_CONTROLLER__)
	#include "../8266_host_controller/app_config.h"
#elif(__PROJECT_REMOTE_OTA_BOOT__)
	#include "../8266_ota_boot/app_config.h"
#elif (__PROJECT_BLE_REMOTE__)
	#include "../8267_ble_remote/app_config.h"
#elif (__PROJECT_BEACON__)
	#include "../8267_beacon/app_config.h"
#elif (__PROJECT_CUSTOMER_REMOTE__)
	#include "../8267_customer_remote/app_config.h"
#elif (__PROJECT_BLE_REMOTE__VIN)
	#include "../8267_vin_remote/app_config.h"
#elif (__PROJECT_BLE_REMOTE__JS)
	#include "../8267_js_remote/app_config.h"
#elif (__PROJECT_DRIVER_TEST__)
	#include "../8267_driver_test/app_config.h"
#elif (__PROJECT_KT_REMOTE__)
    #include "../8267_kt_remote/app_config.h"
#elif (__PROJECT_BLE_HEADPHONE__)
	#include "../8267_ble_lite_SDK/app_config.h"
#elif (__PROJECT_AUDIO_DONGLE__)
	#include "../8267_audio_dongle/app_config.h"
#elif (__PROJECT_SNIFFER_8267__)
	#include "../8267_sniffer/app_config.h"
#elif (__PROJECT_8267_MULTI_MODE_)
	#include "../8267_multi_mode/app_config.h"
#elif (__PROJECT_MESH_LIGHT__ || __PROJECT_MESH_SWITCH__)
	#include "../8263_mesh_light/app_config.h"
#elif (__PROJECT_8263_BLE_MODULE__)
	#include "../8263_ble_module/app_config.h"
#elif (__PROJECT_ANTILOSS_8263__)
	#include "../8263_antiloss/app_config.h"
#elif (__PROJECT_SELFIE_8263__)
	#include "../8263_selfie/app_config.h"
#elif (__PROJECT_8263_BEACON__)
	#include "../8263_beacon/app_config.h"
#else
	#include "../common/default_config.h"
#endif

