#pragma once

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

#define _USER_CONFIG_DEFINED_	1	// must define this macro to make others known
#define	__LOG_RT_ENABLE__		0
#define	USB_PRINTER				1
//////////// product  Information  //////////////////////////////
#define ID_VENDOR				0x248a			// for report
#define ID_PRODUCT_BASE			0x820a
// If ID_PRODUCT left undefined, it will default to be combination of ID_PRODUCT_BASE and the USB config USB_SPEAKER_ENABLE/USB_MIC_ENABLE...
#define ID_PRODUCT				(0x820a + USB_PRINTER)

#define STRING_VENDOR			L"Telink"
#define STRING_PRODUCT			L"BLE Remote KMA Dongle"
#define STRING_SERIAL			L"TLSR8266"

#define CHIP_TYPE				CHIP_TYPE_8266		// 8866-24, 8566-32
#define APPLICATION_DONGLE		1					// or else APPLICATION_DEVICE
#define	FLOW_NO_OS				1

#define	RF_FAST_MODE_1M			1					// BLE mode


// KMA dongle Hardware select
#define DONGLE_PCB_V3_4 		  1//default
#define BLE_EVB_BOARD_8266        0
#define DONGLE_8266_24PIN_F64	  0



////////// CUSTOM infomarion /////////////////////////////
#if (DONGLE_8266_24PIN_F64)
	#define				CFG_ADR_MAC				0xe000
	#define 			CUST_CAP_INFO_ADDR		0xf000
	#define 			CUST_TP_INFO_ADDR		0xf040
#else
	#define				CFG_ADR_MAC				0x76000
	#define 			CUST_CAP_INFO_ADDR		0x77000
	#define 			CUST_TP_INFO_ADDR		0x77040
#endif

/////////////////// PROJECT SELECT /////////////////////////
#define CUSTOMER_1_MODE					0  //.. by sihui
#define CUSTOMER_2_MODE					0  //.. by jw

#define VIN_TEST_MODE					0  //..by haven 

#if VIN_TEST_MODE
#define JS_REMOTE_ENABLE 	0
#define UEI_VOL_UP		((1))
#define UEI_VOL_DN		((1<<1))

#define UEI_POWER			((1<<2))
#define UEI_VOICE_SEARCH	((2<<2))
#define UEI_UP				((3<<2))
#define UEI_LEFT			((4<<2))	
#define UEI_SELECT			((5<<2))
#define UEI_RIGHT			((6<<2))
#define UEI_DOWN			((7<<2))
#define UEI_BACK			((8<<2))
#define UEI_HOME			((9<<2))
#define UEI_MENU			((10<<2))
#define UEI_LANUCH_TV		((11<<2))
#define UEI_MEDIA			((12<<2))
#define UEI_BROWER			((13<<2))
#define UEI_APPS			((14<<2))

#define DONGLE_AUDIO_CONTROL			1

#define DIS_KEY_PRESS_SEQ_ENABLE 	1
#if DIS_KEY_PRESS_SEQ_ENABLE
#define TEST_UNKOWN	0
#define TEST_OK		1
#define TEST_FAIL	2
#define AUIDO_TEST_BIT_POS	6
#define KEY_TEST_BIT_POS 	4
#endif 
#define TEST_END_SEND_SLEEP			1
#define FREQUENCE_1K_3K_5K_ENABLE 	1
#define DONGLE_TEST_ENABLE 		1
#define USER_DEFINE_TIMEOUT_ADR 0x60000
#define AUTO_VOICE_TIMEOUT_ADR  0x61000
//userdefine CODE 
enum{
	CMD_SEND_TIMEOUT =1,
	CMD_SEND_DEEP	 =2,
}USERDEFINE_CODE;
	#if JS_REMOTE_ENABLE	
#define			HID_HANDLE_CONSUME_REPORT_DONGLE			28
#define			HID_HANDLE_KEYBOARD_REPORT_DONGLE			32
#define			AUDIO_HANDLE_MIC_REPORT_DONGLE				54	
#define 		VALUE_REPORT_DONGLE_TIMEOUT						73

	#else
#define			HID_HANDLE_CONSUME_REPORT_DONGLE			(21+21)
#define			HID_HANDLE_KEYBOARD_REPORT_DONGLE			(25+21)
#define			AUDIO_HANDLE_MIC_REPORT_DONGLE				(43+21)
#define 		VALUE_REPORT_DONGLE_TIMEOUT						73
	#endif 
#endif 





/////////////////// MODULE /////////////////////////////////
#define  KMA_DONGLE_SECURITY_ENABLE		0

/////////////////// Clock  /////////////////////////////////

#define CLOCK_SYS_TYPE  		CLOCK_TYPE_PLL
#define CLOCK_SYS_CLOCK_HZ  	32000000

/////////////////// watchdog  //////////////////////////////
#define MODULE_WATCHDOG_ENABLE			0
#define WATCHDOG_INIT_TIMEOUT			200  //ms



#define KMA_DONGLE_OTA_ENABLE			1
#define REMOTE_OTA_DATA_HANDLE      	49


///////////////////  USB   /////////////////////////////////
#if(APPLICATION_DONGLE)
#define	USB_PRINTER_ENABLE 		USB_PRINTER	//
#define	USB_SPEAKER_ENABLE 		0
#define	USB_MIC_ENABLE 			1
#define	USB_MOUSE_ENABLE 		1
#define	USB_KEYBOARD_ENABLE 	1
#define	USB_SOMATIC_ENABLE      0   //  when USB_SOMATIC_ENABLE, USB_EDP_PRINTER_OUT disable
#define USB_CUSTOM_HID_REPORT	1
#endif

//////////////////// Audio /////////////////////////////////////
#define MIC_RESOLUTION_BIT		16
#define MIC_SAMPLE_RATE			16000
#define MIC_CHANNLE_COUNT		1
#define	MIC_ENOCDER_ENABLE		0

#define	MIC_ADPCM_FRAME_SIZE		128 //128
#define	MIC_SHORT_DEC_SIZE			248 //248

#define	TL_MIC_BUFFER_SIZE				512
//#define	TL_SDM_BUFFER_SIZE				1024
/////////////////// set default   ////////////////





//Button Setting
#if DONGLE_PCB_V3_4
#define	SW1_GPIO				GPIO_PD5
#define	SW2_GPIO				GPIO_PD4
#define PULL_WAKEUP_SRC_PD4           PM_PIN_PULLUP_1M  //btn
#define PULL_WAKEUP_SRC_PD5           PM_PIN_PULLUP_1M	//btn

#define PD4_INPUT_ENABLE		1
#define PD5_INPUT_ENABLE		1

#define	 GPIO_LED_GREEN			GPIO_PC0
#define	 GPIO_LED_RED			GPIO_PC4
#define	 GPIO_LED_BLUE			GPIO_PC2
#define	 GPIO_LED_WHITE			GPIO_PA1

#define  PA1_INPUT_ENABLE		0
#define	 PA1_OUTPUT_ENABLE		1
#define  PC2_INPUT_ENABLE		0
#define  PC2_OUTPUT_ENABLE		1
#define  PC4_INPUT_ENABLE		0
#define	 PC4_OUTPUT_ENABLE		1
#define  PC0_INPUT_ENABLE		0
#define	 PC0_OUTPUT_ENABLE		1

#define LED_ON_LEVAL 1 //gpio output high voltage to turn on led

#elif BLE_EVB_BOARD_8266
#define	SW1_GPIO				GPIO_PA6 // GP19, J11 PIN4, SW2-J33 PIN8
#define	SW2_GPIO				GPIO_PB0 //TL_P5, J11 PIN5, SW3-J33 PIN7
#define PULL_WAKEUP_SRC_PA6           PM_PIN_PULLUP_1M  //btn
#define PULL_WAKEUP_SRC_PB0           PM_PIN_PULLUP_1M	//btn

#define PA6_INPUT_ENABLE		1
#define PB0_INPUT_ENABLE		1

#define	 GPIO_LED_GREEN			GPIO_PB1
#define	 GPIO_LED_RED			GPIO_PE4 //TL_G16, J20 PIN6, PE[4], LED1 J33 PIN1
#define	 GPIO_LED_BLUE			GPIO_PB1
#define	 GPIO_LED_WHITE			GPIO_PE5 //TL_G17, J20 PIN5, PE[5], LED2 J33 PIN2

#define  PE4_INPUT_ENABLE		0
#define	 PE4_OUTPUT_ENABLE		1
#define  PE5_INPUT_ENABLE		0
#define	 PE5_OUTPUT_ENABLE		1
#define  PB1_INPUT_ENABLE		0
#define	 PB1_OUTPUT_ENABLE		1

#define LED_ON_LEVAL 0 //gpio output low voltage to turn on led

#elif(DONGLE_8266_24PIN_F64)
#define	SW1_GPIO				GPIO_PE5
#define	SW2_GPIO				GPIO_PE4
#define PULL_WAKEUP_SRC_PE4           PM_PIN_PULLUP_1M  //btn
#define PULL_WAKEUP_SRC_PE5           PM_PIN_PULLUP_1M	//btn

#define PE4_INPUT_ENABLE		1
#define PE5_INPUT_ENABLE		1

#define	 GPIO_LED_GREEN			GPIO_PC0
#define	 GPIO_LED_BLUE			GPIO_PC1
#define	 GPIO_LED_RED			GPIO_PC4
#define	 GPIO_LED_WHITE			GPIO_PF1

#define  PC0_INPUT_ENABLE		0
#define	 PC0_OUTPUT_ENABLE		1
#define  PC1_INPUT_ENABLE		0
#define	 PC1_OUTPUT_ENABLE		1
#define  PC4_INPUT_ENABLE		0
#define	 PC4_OUTPUT_ENABLE		1

#define  PF1_INPUT_ENABLE		0
#define	 PF1_OUTPUT_ENABLE		1

#define  PF1_FUNC	            AS_GPIO

#define LED_ON_LEVAL 		1 //gpio output high voltage to turn on led

#else
#define	SW1_GPIO				GPIO_PC5
//#define	SW2_GPIO			GPIO_xxx
#endif


//USB DM DP input enable
#define PB5_INPUT_ENABLE	1
#define PB6_INPUT_ENABLE	1


#include "../common/default_config.h"

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
