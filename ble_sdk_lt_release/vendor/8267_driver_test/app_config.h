#pragma once

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

#define _USER_CONFIG_DEFINED_	1	// must define this macro to make others known
#define	__LOG_RT_ENABLE__		0
//#define	__DEBUG_PRINT__			0

#define	USB_PRINTER				1
//////////// product  Information  //////////////////////////////
#define ID_VENDOR				0x248a			// for report
#define ID_PRODUCT_BASE			0x8267
#define	ID_PRODUCT				(0x826a + USB_PRINTER)//

#define STRING_VENDOR			L"Telink"
#define STRING_PRODUCT			L"BLE Remote"
#define STRING_SERIAL			L"TLSR8267"

#define	USB_PRINTER_ENABLE 		USB_PRINTER	//

#define CHIP_TYPE				CHIP_TYPE_8267		// 8866-24, 8566-32
#define APPLICATION_DONGLE		0					// or else APPLICATION_DEVICE
#define	FLOW_NO_OS				1

#define	RF_FAST_MODE_1M		1


#define HID_MOUSE_ATT_ENABLE	0



/////////////////// PROJECT SELECT /////////////////////////

/////////////////// MODULE /////////////////////////////////
#define BLE_REMOTE_PM_ENABLE			1
#define BLE_REMOTE_SECURITY_ENABLE      1


#define AUDIO_CONN_INTERVAL_10MS		1


////////////////////////// MIC BUFFER /////////////////////////////
#define	ADPCM_PACKET_LEN				128
#define TL_MIC_ADPCM_UNIT_SIZE			248
#define	TL_SDM_BUFFER_SIZE				992
#define	TL_MIC_32K_FIR_16K				1

#if TL_MIC_32K_FIR_16K
	#define	TL_MIC_BUFFER_SIZE				1984
#else
	#define	TL_MIC_BUFFER_SIZE				992
#endif


//////////////////////////// GPIO //////////////////////////////////
#define	PC4_INPUT_ENABLE				0				//amic digital input disable
#define	PC5_INPUT_ENABLE				0				//amic digital input disable
#define	GPIO_LED				GPIO_PWM1NA4			//PWM1
#define	PWM_LED					GPIO_PWM1NA4			//PWM1
#define	PWMID_LED				1

#define	BUZZER_GPIO             GPIO_PWM1A3// PA3
#define	BUZZER_PWM_TMAX	  		0x1000
#define	BUZZER_PWM_TCMP	  		0x00f0

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

#define	MATRIX_ROW_PULL		PM_PIN_PULLDOWN_100K
#define	MATRIX_COL_PULL		PM_PIN_PULLUP_10K
#define	KB_LINE_HIGH_VALID	     		0
#define DEEPBACK_FAST_KEYSCAN_ENABLE    1

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

#define	PULL_WAKEUP_SRC_PD4		MATRIX_COL_PULL
#define	PULL_WAKEUP_SRC_PD5		MATRIX_COL_PULL
#define	PULL_WAKEUP_SRC_PD6		MATRIX_COL_PULL
#define	PULL_WAKEUP_SRC_PD7		MATRIX_COL_PULL
#define	PULL_WAKEUP_SRC_PE0		MATRIX_COL_PULL
#define	PULL_WAKEUP_SRC_PE1		MATRIX_COL_PULL


#define		KB_MAP_NUM		KB_MAP_NORMAL
#define		KB_MAP_FN		KB_MAP_NORMAL


///////////////////  Hardware  //////////////////////////////
#define GPIO_IR_LEARN_IN	GPIO_GP0
/////////////////// Clock  /////////////////////////////////
#define CLOCK_SYS_TYPE  		CLOCK_TYPE_PLL	//  one of the following:  CLOCK_TYPE_PLL, CLOCK_TYPE_OSC, CLOCK_TYPE_PAD, CLOCK_TYPE_ADC
#define CLOCK_SYS_CLOCK_HZ  	16000000

/////////////////// watchdog  //////////////////////////////
#define MODULE_WATCHDOG_ENABLE	0


/////////////////////// POWER OPTIMIZATION  AT SUSPEND ///////////////////////
//notice that: all setting here aims at power optimization ,they depends on
//the actual hardware design.You should analyze your hardware board and then
//find out the io leakage

//shut down the input enable of some gpios, to lower io leakage at suspend state
#ifndef PA2_INPUT_ENABLE
#define PA2_INPUT_ENABLE			0
#endif
#ifndef PA3_INPUT_ENABLE
#define PA3_INPUT_ENABLE			0
#endif
#ifndef PA4_INPUT_ENABLE
#define PA4_INPUT_ENABLE			0
#endif
#ifndef PA5_INPUT_ENABLE
#define PA5_INPUT_ENABLE			0
#endif
#ifndef PC6_INPUT_ENABLE
#define PC6_INPUT_ENABLE			0
#endif

#define MODULE_USB_ENABLE 1
#if(!(MODULE_USB_ENABLE || __LOG_RT_ENABLE__))  //if USB not use
//PE2:DM   PE3:DP
#ifndef PE2_FUNC
#define PE2_FUNC					AS_GPIO
#endif
#ifndef PE3_FUNC
#define PE3_FUNC					AS_GPIO
#endif
#ifndef PE2_INPUT_ENABLE
#define PE2_INPUT_ENABLE			0
#endif
#ifndef PE3_INPUT_ENABLE
#define PE3_INPUT_ENABLE			0
#endif

#endif



///////////////////  ADC  /////////////////////////////////
#if(MODULE_ADC_ENABLE)
#define CHIP_TYPE_8266_A2			1

#define	ADC_CHNM_ANA_INPUT		FLD_ADC_CHN_C7
#define ADC_CHNM_REF_SRC		ADC_REF_1_3V

#define PC7_INPUT_ENABLE		1
#define PC7_OUTPUT_ENABLE		0
#define PC7_FUNC				AS_ADC
#define PC7_DATA_OUT			0
#define PULL_WAKEUP_SRC_PC7		PM_PIN_UP_DOWN_FLOAT

#endif

///////////////////  battery  /////////////////////////////////

#define BATT_ADC_CHANNEL		0
#define BATT_FULL_VOLT			(4100)	//  mV
#define BATT_LOW_VOLT			(3700)	//  mV
#define BATT_NO_PWR_VOLT		(3400)	//  mV
#define	ADC_CHN0_ANA_INPUT		ADC_CHN_INP_ANA_7
#define ADC_CHN0_REF_SRC		ADC_REF_SRC_INTERNAL

///////////////////  POWER MANAGEMENT  //////////////////


///////////////////  SIHUI DEBUG  //////////////////
#define SIHUI_DEBUG  						0
#if(SIHUI_DEBUG)

#define LOG_IN_RAM                  		0


#define SIHUI_DEBUG_GPIO					1
#if(SIHUI_DEBUG_GPIO)

#define PA2_INPUT_ENABLE					0
#define PA3_INPUT_ENABLE					0
#define PA5_INPUT_ENABLE					0
#define PC0_INPUT_ENABLE					0
#define PC1_INPUT_ENABLE					0
#define PC6_INPUT_ENABLE					0
#define PA2_OUTPUT_ENABLE					1
#define PA3_OUTPUT_ENABLE					1
#define PA5_OUTPUT_ENABLE					1
#define PC0_OUTPUT_ENABLE					1
#define PC1_OUTPUT_ENABLE					1
#define PC6_OUTPUT_ENABLE					1

#define CHN0_LOW		( *(unsigned char *)0x800583 &= (~0x04) )
#define CHN0_HIGH		( *(unsigned char *)0x800583 |= 0x04 )
#define CHN0_TOGGLE		( *(unsigned char *)0x800583 ^= 0x04 )
#define CHN1_LOW		( *(unsigned char *)0x800583 &= (~0x08) )
#define CHN1_HIGH		( *(unsigned char *)0x800583 |= 0x08 )
#define CHN1_TOGGLE		( *(unsigned char *)0x800583 ^= 0x08 )
#define CHN2_LOW		( *(unsigned char *)0x800583 &= (~0x20) )
#define CHN2_HIGH		( *(unsigned char *)0x800583 |= 0x20 )
#define CHN2_TOGGLE		( *(unsigned char *)0x800583 ^= 0x20 )
#define CHN3_LOW		( *(unsigned char *)0x800593 &= (~0x01) )
#define CHN3_HIGH		( *(unsigned char *)0x800593 |= 0x01 )
#define CHN3_TOGGLE		( *(unsigned char *)0x800593 ^= 0x01 )
#define CHN4_LOW		( *(unsigned char *)0x800593 &= (~0x02) )
#define CHN4_HIGH		( *(unsigned char *)0x800593 |= 0x02 )
#define CHN4_TOGGLE		( *(unsigned char *)0x800593 ^= 0x02 )
#define CHN5_LOW		( *(unsigned char *)0x800593 &= (~0x40) )
#define CHN5_HIGH		( *(unsigned char *)0x800593 |= 0x40 )
#define CHN5_TOGGLE		( *(unsigned char *)0x800593 ^= 0x40 )

#define LED_ON		gpio_set_output_en (GPIO_LED, 1);\
					gpio_write (GPIO_LED, 1)
#define LED_OFF		gpio_set_output_en (GPIO_LED, 0);\
					gpio_write (GPIO_LED, 0)


#endif  //end of SIHUI_DEBUG_GPIO

#define  SIHUI_DBG_FIFO						0
#define  SIHUI_DBG_BRX_MACHINE				0

#endif

/////////////////// set default   ////////////////

#include "../common/default_config.h"


/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
