#if (__PROJECT_8266_BLE_REMOTE__)

#include "../../proj/tl_common.h"
#include "../../proj_lib/rf_drv.h"
#include "../../proj_lib/pm.h"
#include "../../proj_lib/blt_ll/blt_ll.h"
#include "../../proj/drivers/keyboard.h"
#include "../common/tl_audio.h"
#include "../common/blt_led.h"
#include "../../proj_lib/blt_ll/trace.h"
#include "../../proj/mcu/pwm.h"
#include "../../proj_lib/ble_l2cap/ble_ll_ota.h"
#include "../../proj/drivers/audio.h"

#define				CFG_ADR_MAC				0x76000
#define 			CUST_CAP_INFO_ADDR		0x77000
#define 			CUST_TP_INFO_ADDR		0x77040
////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
#define			HID_HANDLE_CONSUME_REPORT			21
#define			HID_HANDLE_KEYBOARD_REPORT			25



//////////////////////////////////////////////////////////////////////////////
//	Initialization: MAC address, Adv Packet, Response Packet
//////////////////////////////////////////////////////////////////////////////
u8  tbl_mac [] = {0xe1, 0xe1, 0xe2, 0xe3, 0xe4, 0xc7};

u8	tbl_adv [] =
		{0x00, 25,								//random address
		 0xef, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5,		//mac address
		 0x05, 0x09, 'w', 'b', 's', 'h',
		 0x02, 0x01, 0x05, 							// BLE limited discoverable mode and BR/EDR not supported
		 0x03, 0x19, 0x80, 0x01, 					// 384, Generic Remote Control, Generic category
		 0x05, 0x02, 0x12, 0x18, 0x0F, 0x18,		// incomplete list of service class UUIDs (0x1812, 0x180F)
		};


u8	tbl_rsp [] =
		{0x04, 14,									//type len
		 0xef, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5,		//mac address
		 0x07, 0x09, 'w', 'b', 's', 'h', 'b', 'd'	//scan name " tSelfi"
		};

/////////////////////////// led management /////////////////////
enum{
	LED_POWER_ON = 0,
	LED_LONG_ON,	//1
	LED_LONG_OFF,	//2
	LED_SHINE_SLOW,
	LED_SHINE_FAST,
};


device_led_t device_led;
const led_cfg_t led_cfg[] = {
	    {1000,    0,      1,      0x00,	 },    //power-on, 1s on
	    {100,	  0 ,	  0xff,	  0x02,  },    //long on
	    {0,	      100 ,   0xff,	  0x02,  },    //long off
	    {500,	  500 ,   3,	  0x04,	 },    //1Hz for 3 seconds
	    {250,	  250 ,   6,	  0x04,  },    //2Hz for 3 seconds
};



extern kb_data_t	kb_event;


u8 key_buf[8] = {0};
u8 key_type;
#define CONSUMER_KEY   0
#define KEYBOARD_KEY   1

int key_not_released;

u32 latest_user_event_tick;
u8  user_task_ongoing_flg;
static u8 ota_is_working = 0;


int conn_interval_us;
u32 my_ota_timeout_us;


void entry_ota_mode(void)
{
	ota_is_working = 1;

	ble_setOtaTimeout(15000000); //set OTA timeout  15 S

	//gpio_write(GPIO_LED, 1);  //LED on for indicate OTA mode
}

void show_ota_result(int result)
{
#if 0
	if(result == OTA_SUCCESS){
		for(int i=0; i< 8;i++){  //4Hz shine for  4 second
			gpio_write(GPIO_LED, 0);
			sleep_us(125000);
			gpio_write(GPIO_LED, 1);
			sleep_us(125000);
		}
	}
	else{
		for(int i=0; i< 8;i++){  //1Hz shine for  4 second
			gpio_write(GPIO_LED, 0);
			sleep_us(500000);
			gpio_write(GPIO_LED, 1);
			sleep_us(500000);
		}
	}


	gpio_write(GPIO_LED, 0);
#endif
}


void 	ble_remote_terminate(u8 e,u8 *p)
{

}

void	task_connect (u8 e, u8 *p)
{
	//blt_conn_para_request (8, 8, 99, 400);  //interval=10ms latency=99 timeout=4s
	latest_user_event_tick = clock_time();
}


void proc_keyboard (u8 e, u8 *p)
{
	kb_event.keycode[0] = 0;
	int det_key = kb_scan_key (0, 1);

	if (det_key){
		latest_user_event_tick = clock_time();  //record latest key change time
		key_not_released = 1;

		u8 key = kb_event.keycode[0];
		if ( (key & 0xf0) == 0xf0)			//key in consumer report
		{
			key_type = CONSUMER_KEY;
			blt_push_notify (HID_HANDLE_CONSUME_REPORT, 1 << (key & 0xf), 2);
		}
		else if (key)			// key in standard reprot
		{
			key_type = KEYBOARD_KEY;
			key_buf[2] = key;
			blt_push_notify_data (HID_HANDLE_KEYBOARD_REPORT, key_buf, 8);
		}
		else {
			key_not_released = 0;
			if(key_type == CONSUMER_KEY){
				blt_push_notify (HID_HANDLE_CONSUME_REPORT, 0, 2);  //release
			}
			else{
				key_buf[2] = 0;
				blt_push_notify_data (HID_HANDLE_KEYBOARD_REPORT, key_buf, 8); //release
			}
		}
	}
}

void blt_pm_proc(void)
{
#if(BLE_REMOTE_PM_ENABLE)
	if(ota_is_working){  //OTA
		blt_enable_suspend(SUSPEND_DISABLE);
	}
	else{
		blt_enable_suspend(SUSPEND_ADV | SUSPEND_CONN);

		user_task_ongoing_flg = key_not_released || DEVICE_LED_BUSY;

		if(key_not_released || DEVICE_LED_BUSY){
			blt_set_latency_off();
		}

#if 0
		if(sendTerminate_before_enterDeep == 1){ //sending Terminate and wait for ack before enter deepsleep
			if(user_task_ongoing_flg){  //detect key Press again,  can not enter deep now
				sendTerminate_before_enterDeep = 0;
			}
		}
		else if(sendTerminate_before_enterDeep == 2){  //send Terminate OK/timeout
			blt_enable_suspend (DEEPSLEEP_CONN); //set deepsleep
			blt_set_wakeup_source(PM_WAKEUP_PAD);  //gpio PAD wakeup deesleep
			analog_write(DEEP_ANA_REG0,CONN_DEEP_FLG);
		}
#endif

		//adv 60s, deepsleep
		if( blt_state == BLT_LINK_STATE_ADV && clock_time_exceed(blt_get_adv_begin_tick() ,60 * 1000 * 1000)){
			blt_enable_suspend (DEEPSLEEP_ADV); //set deepsleep
			blt_set_wakeup_source(PM_WAKEUP_PAD);  //gpio PAD wakeup deesleep
			analog_write(DEEP_ANA_REG0,ADV_DEEP_FLG);
		}
		//conn 60s no event(key/voice/led), enter deepsleep
		else if( blt_state == BLT_LINK_STATE_CONN && clock_time_exceed(latest_user_event_tick, 60 * 1000 * 1000) \
			&& !user_task_ongoing_flg){

			//blt_terminate();  //push terminate cmd into ble TX buffer
			//sendTerminate_before_enterDeep = 1;

			blt_enable_suspend (DEEPSLEEP_CONN); //set deepsleep
			blt_set_wakeup_source(PM_WAKEUP_PAD);  //gpio PAD wakeup deesleep
			analog_write(DEEP_ANA_REG0,CONN_DEEP_FLG);
		}
	}

#endif  //END of  BLE_REMOTE_PM_ENABLE
}



_attribute_ram_code_ void  ble_remote_set_sleep_wakeup (u8 e, u8 *p)
{
	if( blt_state == BLT_LINK_STATE_CONN && ((u32)(blt_get_wakeup_time() - clock_time())) > 80 * CLOCK_SYS_CLOCK_1MS){  //suspend time > 30ms.add gpio wakeup
		blt_set_wakeup_source(PM_WAKEUP_CORE);  //gpio CORE wakeup suspend
	}
}

void 	adv_type_switch(u8 e,u8 *p)
{
	blt_set_adv_channel (BLT_ENABLE_ADV_37);  //only one channel to save power, you can change it
	blt_set_adv_type(ADV_TYPE_CONNECTABLE_UNDIRECTED);  //undirected adv
	blt_set_adv_interval (30000);  //adv interval 30ms
	blt_set_adv_duration (0, 0);  //duration disable
	blt_set_adv_enable(1);  //enable adv again
}


void rf_customized_param_load(void)
{
	  //flash 0x77000 customize freq_offset adjust cap value, if not customized, default ana_81 is 0xd0
	 if( (*(unsigned char*) CUST_CAP_INFO_ADDR) != 0xff ){
		 //ana_81<4:0> is cap value(0x00 - 0x1f)
		 analog_write(0x81, (analog_read(0x81)&0xe0) | ((*(unsigned char*) CUST_CAP_INFO_ADDR)&0x1f) );
	 }

	 //flash 0x77040 customize TP0, flash 0x77041 customize TP1
	 //if not customized,  default TP0 is 0x1f, default TP1 is 0x18
	 if( ((*(unsigned char*) (CUST_TP_INFO_ADDR)) != 0xff) && ((*(unsigned char*) (CUST_TP_INFO_ADDR+1)) != 0xff) ){
		 rf_update_tp_value(*(unsigned char*) (CUST_TP_INFO_ADDR), *(unsigned char*) (CUST_TP_INFO_ADDR+1));
	 }
}

void user_init()
{
	rf_customized_param_load();  //load customized freq_offset cap value and tp value

	//usb_log_init ();
	//usb_dp_pullup_en (1);  //open USB enum

	/////////////////// keyboard drive/scan line configuration /////////
/*	u32 pin[] = KB_DRIVE_PINS;
	for (int i=0; i<(sizeof (pin)/sizeof(*pin)); i++)
	{
		gpio_set_wakeup(pin[i],1,1);  	   //drive pin core(gpio) high wakeup suspend
		cpu_set_gpio_wakeup (pin[i],1,1);  //drive pin pad high wakeup deepsleep
	}*/
	gpio_core_wakeup_enable_all(1);


#if (KEYSCAN_IRQ_TRIGGER_MODE)
	gpio_core_irq_enable_all(1);
	reg_irq_src = FLD_IRQ_GPIO_EN;
#endif

	blt_register_event_callback (BLT_EV_FLAG_CONNECT, &task_connect);
	blt_register_event_callback (BLT_EV_FLAG_TERMINATE, &ble_remote_terminate);
	blt_register_event_callback (BLT_EV_FLAG_EARLY_WAKEUP, &proc_keyboard);

	////////////////// BLE slave initialization ////////////////////////////////////
	u32 *pmac = (u32 *) CFG_ADR_MAC;
	if (*pmac != 0xffffffff)
	{
	    memcpy (tbl_mac, pmac, 6);
	}
    else
    {
        //TODO : should write mac to flash after pair OK
        tbl_mac[0] = (u8)rand();
        tbl_mac[0] = tbl_mac[1] = tbl_mac[2] = tbl_mac[3] = tbl_mac[4]  = tbl_mac[5] = 0x55;  //debug
        flash_write_page (CFG_ADR_MAC, 6, tbl_mac);
    }

	rf_set_power_level_index (RF_POWER_8dBm);
	blt_init (tbl_mac, tbl_adv, tbl_rsp);

	extern void my_att_init ();
	my_att_init ();

	blt_set_adv_interval (700*1000);
	blt_set_adv_channel (BLT_ENABLE_ADV_37); //only one channel to save power, you can change it

#if(BLE_REMOTE_PM_ENABLE)
	blt_enable_suspend (SUSPEND_ADV | SUSPEND_CONN);
	blt_register_event_callback (BLT_EV_FLAG_SET_WAKEUP_SOURCE, &ble_remote_set_sleep_wakeup);
#else
	blt_enable_suspend (SUSPEND_DISABLE);
#endif


	//led gpio init
	//gpio_set_output_en(GPIO_LED, 1);
	//gpio_write(GPIO_LED, 0);

	//////////////////////////////////////
		gpio_set_func(GPIO_PD5, AS_GPIO);
		gpio_set_output_en(GPIO_PD5, 0);
		gpio_set_input_en(GPIO_PD5, 1);


		gpio_setup_up_down_resistor(GPIO_PD5, PM_PIN_PULLUP_1M);

		blt_set_wakeup_source(PM_WAKEUP_CORE);
			gpio_set_wakeup(GPIO_PD5,0,1);


		gpio_set_func(GPIO_PA1, AS_GPIO);
		gpio_set_output_en(GPIO_PA1, 1);
		gpio_set_input_en(GPIO_PA1, 0);
		//	gpio_set_interrupt(GPIO_PD5, 1);
			/* Enable GPIO irq */
		//	reg_irq_mask |= FLD_IRQ_GPIO_RISC2_EN;

		//	gpio_core_irq_enable_all(1);

	//	blt_set_wakeup_source(PM_WAKEUP_PAD);
	//	cpu_set_gpio_wakeup(GPIO_PD5,0,1);

		gpio_core_wakeup_enable_all(1);
	////////////////// OTA relative ////////////////////////
	ble_setOtaStartCb(entry_ota_mode);
	ble_setOtaResIndicateCb(show_ota_result);
}

/////////////////////////////////////////////////////////////////////
// main loop flow
/////////////////////////////////////////////////////////////////////
u32 tick_loop;

u8   tick_loop11;
void main_loop (void)
{
	tick_loop ++;

	////////////////////////////////////// BLE entry /////////////////////////////////
	blt_brx_proc ();

	////////////////////////////////////// UI entry /////////////////////////////////
	tick_loop11=tick_loop%2;
	gpio_write(GPIO_PA6,tick_loop11);

if(0)
{
	tick_loop=0;
	blt_set_wakeup_source(PM_WAKEUP_PAD);
	blt_enable_suspend (DEEPSLEEP_ADV);
	}
//	proc_keyboard (0,0);

//	device_led_process();
//	blt_pm_proc();
    blt_set_wakeup_source(PM_WAKEUP_CORE);
	////////////////////////////////////// Suspend entry /////////////////////////////
	blt_brx_sleep ();
}

#endif


#if 0  //pm_test
if(clock_time_exceed(0, 3000000)){
	shutdown_gpio();
	cpu_sleep_wakeup(0, 0, 0);
}
#endif
