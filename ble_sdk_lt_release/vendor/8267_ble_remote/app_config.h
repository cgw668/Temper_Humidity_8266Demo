#pragma once

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

#define _USER_CONFIG_DEFINED_	1	// must define this macro to make others known
#define	__LOG_RT_ENABLE__		0
//#define	__DEBUG_PRINT__			0
//////////// product  Information  //////////////////////////////
#define ID_VENDOR				0x248a			// for report
#define ID_PRODUCT_BASE			0x880C
#define STRING_VENDOR			L"Telink"
#define STRING_PRODUCT			L"BLE Remote"
#define STRING_SERIAL			L"TLSR8267"

#define CHIP_TYPE				CHIP_TYPE_8267		// 8866-24, 8566-32
#define APPLICATION_DONGLE		0					// or else APPLICATION_DEVICE
#define	FLOW_NO_OS				1

#define		RF_FAST_MODE_1M		1
//////////////////////CAN NOT CHANGE CONFIG ABOVE ////////////////////////////



/////////////////// MODULE /////////////////////////////////
#define BLE_REMOTE_PM_ENABLE			1
#define BLE_REMOTE_SECURITY_ENABLE      1
#define UUID_LENGTH_16_BYTES  			1   //privaet service UUID must be 16 bytes(telink audio, OTA...)
#define BLE_IR_ENABLE					0
#define HID_MOUSE_ATT_ENABLE			0


////////////////////////// MIC BUFFER /////////////////////////////
#define BLE_DMIC_ENABLE					0  //0: Amic   1: Dmic
#define	ADPCM_PACKET_LEN				128
#define TL_MIC_ADPCM_UNIT_SIZE			248

#if BLE_DMIC_ENABLE
	#define	TL_MIC_32K_FIR_16K			0
#else
	#define	TL_MIC_32K_FIR_16K			1
#endif


#if TL_MIC_32K_FIR_16K
	#define	TL_MIC_BUFFER_SIZE				1984
#else
	#define	TL_MIC_BUFFER_SIZE				992
#endif

#define ATTRIBUTE_RAM_CODE_ENCRYPTION_SPEEDUP		1



//////////////////////////// KEYSCAN/MIC  GPIO //////////////////////////////////
#define	MATRIX_ROW_PULL					PM_PIN_PULLDOWN_100K
#define	MATRIX_COL_PULL					PM_PIN_PULLUP_10K

#define	KB_LINE_HIGH_VALID				0   //dirve pin output 0 when keyscan, scanpin read 0 is valid
#define DEEPBACK_FAST_KEYSCAN_ENABLE	1   //proc fast scan when deepsleep back trigged by key press, in case key loss
#define KEYSCAN_IRQ_TRIGGER_MODE		1
#define LONG_PRESS_KEY_POWER_OPTIMIZE	1   //lower power when pressing key without release

//stuck key
#define STUCK_KEY_PROCESS_ENABLE		0
#define STUCK_KEY_ENTERDEEP_TIME		60  //in s

//repeat key
#define KB_REPEAT_KEY_ENABLE			0
#define	KB_REPEAT_KEY_INTERVAL_MS		200
#define KB_REPEAT_KEY_NUM				1
#define KB_MAP_REPEAT					{VK_1, }


#define	PC4_INPUT_ENABLE				0	//amic digital input disable
#define	PC5_INPUT_ENABLE				0	//amic digital input disable

#define			CR_VOL_UP		0xf0
#define			CR_VOL_DN		0xf1
#define			CR_VOL_MUTE		0xf2
#define			CR_POWER		0xf3
#define			CR_SEL			0xf4
#define			CR_UP			0xf5
#define			CR_DN			0xf6
#define			CR_LEFT			0xf7
#define			CR_RIGHT		0xf8
#define			CR_HOME			0xf9
#define			CR_REWIND		0xfa
#define			CR_NEXT			0xfb
#define			CR_PREV			0xfc
#define			CR_STOP			0xfd


#define	GPIO_LED				GPIO_PA4

#define CONNECT_AUDIO_DRIVER_ENABLE 	0


#define		KB_MAP_NORMAL	{\
				{CR_VOL_MUTE,	VK_3,	  	VK_1,		VK_MEDIA,	}, \
				{VK_2,	 		VK_5,	  	VK_M,		VK_4,	 	}, \
				{CR_RIGHT,	 	VK_NONE,	CR_SEL,		CR_LEFT,	}, \
				{CR_REWIND,	 	VK_NONE,	CR_DN,		CR_HOME,	}, \
				{CR_VOL_UP,	 	VK_NONE,	VK_MMODE,	CR_VOL_DN,	}, \
				{VK_WEB,		VK_NONE,	CR_UP,		CR_POWER,	}, }

#define  KB_DRIVE_PINS  {GPIO_PB1, GPIO_PB2, GPIO_PB3, GPIO_PB6}
#define  KB_SCAN_PINS   {GPIO_PD4, GPIO_PD5, GPIO_PD6, GPIO_PD7, GPIO_PE0, GPIO_PE1}

#define	PULL_WAKEUP_SRC_PB1		MATRIX_ROW_PULL
#define	PULL_WAKEUP_SRC_PB2		MATRIX_ROW_PULL
#define	PULL_WAKEUP_SRC_PB3		MATRIX_ROW_PULL
#define	PULL_WAKEUP_SRC_PB6		MATRIX_ROW_PULL
#define PB1_INPUT_ENABLE		1
#define PB2_INPUT_ENABLE		1
#define PB3_INPUT_ENABLE		1
#define PB6_INPUT_ENABLE		1

#define	PULL_WAKEUP_SRC_PD4		MATRIX_COL_PULL
#define	PULL_WAKEUP_SRC_PD5		MATRIX_COL_PULL
#define	PULL_WAKEUP_SRC_PD6		MATRIX_COL_PULL
#define	PULL_WAKEUP_SRC_PD7		MATRIX_COL_PULL
#define	PULL_WAKEUP_SRC_PE0		MATRIX_COL_PULL
#define	PULL_WAKEUP_SRC_PE1		MATRIX_COL_PULL
#define PD4_INPUT_ENABLE		1
#define PD5_INPUT_ENABLE		1
#define PD6_INPUT_ENABLE		1
#define PD7_INPUT_ENABLE		1
#define PE0_INPUT_ENABLE		1
#define PE1_INPUT_ENABLE		1





#define		KB_MAP_NUM		KB_MAP_NORMAL
#define		KB_MAP_FN		KB_MAP_NORMAL





/////////////////////// POWER OPTIMIZATION  AT SUSPEND ///////////////////////
//notice that: all setting here aims at power optimization ,they depends on
//the actual hardware design.You should analyze your hardware board and then
//find out the io leakage

//shut down the input enable of some gpios, to lower io leakage at suspend state
//for example:  #define PA2_INPUT_ENABLE   0



/////////////////// Clock  /////////////////////////////////
#define CLOCK_SYS_TYPE  		CLOCK_TYPE_PLL	//  one of the following:  CLOCK_TYPE_PLL, CLOCK_TYPE_OSC, CLOCK_TYPE_PAD, CLOCK_TYPE_ADC
#define CLOCK_SYS_CLOCK_HZ  	16000000

//////////////////Extern Crystal Type///////////////////////
#define CRYSTAL_TYPE			XTAL_12M		//  extern 12M crystal


/////////////////// watchdog  //////////////////////////////
#define MODULE_WATCHDOG_ENABLE		0
#define WATCHDOG_INIT_TIMEOUT		500  //ms



/////////////open SWS digital pullup to prevent MCU err, this is must ////////////
#define PB0_DATA_OUT					1


/////////////////// set default   ////////////////

#include "../common/default_config.h"

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
