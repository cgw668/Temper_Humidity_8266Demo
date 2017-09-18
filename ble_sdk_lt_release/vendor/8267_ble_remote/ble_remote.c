#if (__PROJECT_BLE_REMOTE__)

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
#include "../../proj/drivers/adc.h"



#define				CFG_ADR_MAC				0x76000
#define 			CUST_CAP_INFO_ADDR		0x77000
#define 			CUST_TP_INFO_ADDR		0x77040
////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
//		handle 0x0e: consumper report
#if HID_MOUSE_ATT_ENABLE	//initial the define is closed

#define 		HID_HANDLE_MOUSE_REPORT				24
#define			HID_HANDLE_CONSUME_REPORT			28
#define			HID_HANDLE_KEYBOARD_REPORT			32
#define			AUDIO_HANDLE_MIC					50


#else 
#define			HID_HANDLE_CONSUME_REPORT			21
#define			HID_HANDLE_KEYBOARD_REPORT			25
#define			AUDIO_HANDLE_MIC					43
#endif 

u16 BattValue[10] = {0};

//////////////////////////////////////////////////////////////////////////////
//	Initialization: MAC address, Adv Packet, Response Packet
//////////////////////////////////////////////////////////////////////////////
u8  tbl_mac [] = {0xe1, 0xe1, 0xe2, 0xe3, 0xe4, 0xc7};

u8	tbl_adv [] =
		{0x00, 25,								//random address
		 0xef, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5,		//mac address
		 0x05, 0x09, 't', 'H', 'I', 'D',
		 0x02, 0x01, 0x05, 							// BLE limited discoverable mode and BR/EDR not supported
		 0x03, 0x19, 0x80, 0x01, 					// 384, Generic Remote Control, Generic category
		 0x05, 0x02, 0x12, 0x18, 0x0F, 0x18,		// incomplete list of service class UUIDs (0x1812, 0x180F)
		};


u8	tbl_rsp [] =
		{0x04, 14,									//type len
		 0xef, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5,		//mac address
		 0x07, 0x09, 't', 'S', 'e', 'l', 'f', 'i'	//scan name " tSelfi"
		};

/////////////////////////// led management /////////////////////
enum{
	LED_POWER_ON = 0,
	LED_AUDIO_ON,	//1
	LED_AUDIO_OFF,	//2
	LED_SHINE_SLOW, //3
	LED_SHINE_FAST, //4
	LED_SHINE_OTA, //5
};

const led_cfg_t led_cfg[] = {
	    {1000,    0,      1,      0x00,	 },    //power-on, 1s on
	    {100,	  0 ,	  0xff,	  0x02,  },    //audio on, long on
	    {0,	      100 ,   0xff,	  0x02,  },    //audio off, long off
	    {500,	  500 ,   3,	  0x04,	 },    //1Hz for 3 seconds
	    {250,	  250 ,   6,	  0x04,  },    //2Hz for 3 seconds
	    {125,	  125 ,   200,	  0x08,  },    //4Hz for 50 seconds
};



u8				ui_mic_enable = 0;

u32 mic_delay_cnt = 0;


int lowBattDet_enable = 0;
void		ui_enable_mic (u8 en)
{
	ui_mic_enable = en;

	mic_delay_cnt = 0;

	gpio_set_output_en (GPIO_PC3, en);		//AMIC Bias output
	gpio_write (GPIO_PC3, en);

	device_led_setup(led_cfg[en ? LED_AUDIO_ON : LED_AUDIO_OFF]);


	if(en){  //audio on
		lowBattDet_enable = 1;
		battery2audio();////switch auto mode
	}
	else{  //audio off
		audio2battery();////switch manual mode
		lowBattDet_enable = 0;
	}
}


extern kb_data_t	kb_event;


u8 key_buf[8] = {0};
u8 key_type;
u8 pm_mask_temp;
#define CONSUMER_KEY   0
#define KEYBOARD_KEY   1


u8 sendTerminate_before_enterDeep = 0;

int key_not_released;
int ir_not_released;

u32 latest_user_event_tick;
u8  user_task_ongoing_flg;

#if (STUCK_KEY_PROCESS_ENABLE)
u32 stuckKey_keyPressTime;
#endif

void	task_audio (u8 e, u8 *p)
{
	if (!ui_mic_enable)
	{
#if(BLE_REMOTE_PM_ENABLE && !BLE_IR_ENABLE) //if IR open, can not stall mcu
		blt_enable_muc_stall(1);
#endif
		return;
	}


	mic_delay_cnt ++;
	if(mic_delay_cnt < 11){  //delay 7.5*10 ms to avoid noise data
		return;
	}

	///////////////////////////////////////////////////////////////
	//log_task_begin (TR_T_adpcm);
	proc_mic_encoder ();		//about 1.2 ms @16Mhz clock
	//log_task_end (TR_T_adpcm);

	//////////////////////////////////////////////////////////////////
	if (blt_fifo_num() < 8)
	{
		int *p = mic_encoder_data_buffer ();
		if (p)					//around 3.2 ms @16MHz clock
		{
			log_task_begin (TR_T_user_task);
			blt_push_notify_data (AUDIO_HANDLE_MIC, (u8*)p, ADPCM_PACKET_LEN);
			log_task_end (TR_T_user_task);
		}
	}
}


void	conn_para_update_req_proc (u8 e, u8 *p)
{
	//p[1]p[0]:offset; p[3]p[2]:interval; p[5]p[4]:latency; p[7]p[6]:timeout; p[9]p[8]:inst;

	u16 *ps = (u16 *)p;
	int interval = ps[1];

}


/****************************************************************************************
there are 4 case could lead to BLT_EV_FLAG_TERMINATE callback in telink 8267 ble stack
1. connection supervision timeout,  				reason: HCI_ERR_CONN_TIMEOUT (0x08)
2. master send Terminate, slave acked   			resson: HCI_ERR_REMOTE_USER_TERM_CONN (0x13)
3. slave call <void blt_terminate (void)> to send terminate
  (1). master acked in 2 second						reason: SLAVE_TERMINATE_CONN_ACKED
  (2). master not acked in 2 second, timeout		reason: SLAVE_TERMINATE_CONN_ACKED
****************************************************************************************/
void 	ble_remote_terminate(u8 e,u8 *p) //*p is terminate reason
{
	if(*p == HCI_ERR_CONN_TIMEOUT){

	}
	else if(*p == HCI_ERR_REMOTE_USER_TERM_CONN){  //0x13

	}
	else if(*p == SLAVE_TERMINATE_CONN_ACKED || *p == SLAVE_TERMINATE_CONN_TIMEOUT){

	}

	 //user has push terminate pkt to ble TX buffer before deepsleep
	if(sendTerminate_before_enterDeep == 1){
		sendTerminate_before_enterDeep = 2;
	}

	if(ui_mic_enable){
		ui_enable_mic (0);
	}
}

void	task_connect (u8 e, u8 *p)
{
	blt_conn_para_request (6, 6, 99, 400);  //interval=10ms latency=99 timeout=4s
	latest_user_event_tick = clock_time();
}


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////


//This function process ...
void deep_wakeup_proc(void)
{
#if(DEEPBACK_FAST_KEYSCAN_ENABLE)
	//if deepsleep wakeup is wakeup by GPIO(key press), we must quickly scan this
	//press, hold this data to the cache, when connection established OK, send to master
	//deepsleep_wakeup_fast_keyscan
	if(analog_read(DEEP_ANA_REG0) == CONN_DEEP_FLG){
		if(kb_scan_key (KB_NUMLOCK_STATUS_POWERON,1) && kb_event.cnt){
			deepback_key_state = DEEPBACK_KEY_CACHE;
			key_not_released = 1;
			memcpy(&kb_event_cache,&kb_event,sizeof(kb_event));
		}
	}
#endif
}


u8 bRspMtuFlag =1;
signed char mouse_data_add[4]={0,20,20,0};
signed char mouse_data_minus[4]={0,-20,-20,0};


static u8 key_voice_press = 0;
static u8 ota_is_working = 0;
static u32 key_voice_pressTick = 0;
static u32 interval_update_tick =0;

void deepback_pre_proc(int *det_key)
{
	// to handle deepback key cache
	extern u32 blt_conn_start_tick; //ble connect establish time
	if(!(*det_key) && deepback_key_state == DEEPBACK_KEY_CACHE && blt_state == BLT_LINK_STATE_CONN \
			&& clock_time_exceed(blt_conn_start_tick,25000)){

		memcpy(&kb_event,&kb_event_cache,sizeof(kb_event));
		*det_key = 1;

		if(key_not_released || kb_event_cache.keycode[0] == VK_M){  //no need manual release
			deepback_key_state = DEEPBACK_KEY_IDLE;
		}
		else{  //need manual release
			deepback_key_tick = clock_time();
			deepback_key_state = DEEPBACK_KEY_WAIT_RELEASE;
		}
	}
}

void deepback_post_proc(void)
{
	//manual key release
	if(deepback_key_state == DEEPBACK_KEY_WAIT_RELEASE && clock_time_exceed(deepback_key_tick,150000)){
		key_not_released = 0;

		key_buf[2] = 0;
		blt_push_notify_data (HID_HANDLE_KEYBOARD_REPORT, key_buf, 8); //release
		deepback_key_state = DEEPBACK_KEY_IDLE;
	}
}



void key_change_proc(void)
{
#if (STUCK_KEY_PROCESS_ENABLE)
	if(kb_event.cnt
	#if (KB_REPEAT_KEY_ENABLE)
		 && ( !repeat_key.key_repeat_flg || repeat_key.key_change_flg == KEY_CHANGE )
	#endif
			){  //key press
		stuckKey_keyPressTime = clock_time();  //any key press change will update this time
	}
#endif

	latest_user_event_tick = clock_time();  //record latest key change time

	if(key_voice_press){  //clear voice key press flg
		key_voice_press = 0;
	}

	u8 key = kb_event.keycode[0];

	if ( (key & 0xf0) == 0xf0)			//key in consumer report
	{
		key_not_released = 1;
		key_type = CONSUMER_KEY;
		blt_push_notify (HID_HANDLE_CONSUME_REPORT, 1 << (key & 0xf), 2);
	}
	else if (key)			// key in standard reprot
	{
		key_not_released = 1;
		if (key == VK_M)
		{
			if(ui_mic_enable){
				//adc_clk_powerdown();
				ui_enable_mic (0);
			}
			else{ //if voice not on, mark voice key press tick
				key_voice_press = 1;
				key_voice_pressTick = clock_time();
			}
		}
#if HID_MOUSE_ATT_ENABLE
		else if (key == VK_1)	//change the key by actually use
		{
			key_type = KEYBOARD_KEY;
			blt_push_notify_data (HID_HANDLE_MOUSE_REPORT, mouse_data_add,4);

		}
		else if (key == VK_2)	//change the key by actually use
		{
			key_type = KEYBOARD_KEY;
			blt_push_notify_data (HID_HANDLE_MOUSE_REPORT, mouse_data_minus,4);

		}
#endif
		else{
			key_type = KEYBOARD_KEY;
			key_buf[2] = key;
			blt_push_notify_data (HID_HANDLE_KEYBOARD_REPORT, key_buf, 8);
		}
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

void voice_press_proc(void)
{
	key_voice_press = 0;
	//adc_clk_poweron();
	//config_adc (FLD_ADC_PGA_C45, FLD_ADC_CHN_D0, SYS_16M_AMIC_16K);
	ui_enable_mic (1);

	if(bRspMtuFlag && blt_state==BLT_LINK_STATE_CONN){
		extern u8 pkt_notify_short[36];
		extern u8 blt_push_fifo (u8 *p);
		bRspMtuFlag = 0;

		pkt_notify_short[0] = 9;
		pkt_notify_short[4] = 2;			//first data packet
		pkt_notify_short[5] = 7;
		*(u16*)(pkt_notify_short + 6) =  3; //l2cap
		*(u16*)(pkt_notify_short + 8) = 0x04;	//chanid
		pkt_notify_short[10]=0x02;
		pkt_notify_short[11]=0x9e;
		//pkt_notify_short[11]=24;
		pkt_notify_short[12]=0x00;

		blt_push_fifo (pkt_notify_short);
	}
}

void audio_driver_proc(void)
{
	if(		(clock_time_exceed(interval_update_tick,20*1000*1000))&&
			(blt_conn_interval != 6*1250*CLOCK_SYS_CLOCK_1US)&&
			(blt_state== BLT_LINK_STATE_CONN)	){
		interval_update_tick = clock_time();
		blt_conn_para_request (6, 6, 99, 400);  //interval=10ms latency=99 timeout=4s

	}
}

void proc_keyboard (u8 e, u8 *p)
{
	// sometimes the connect update request may lost ,and make sure the command is done
#if CONNECT_AUDIO_DRIVER_ENABLE
	audio_driver_proc();
#endif


	kb_event.keycode[0] = 0;
	int det_key = kb_scan_key (0, 1);


#if(DEEPBACK_FAST_KEYSCAN_ENABLE)
	if(deepback_key_state != DEEPBACK_KEY_IDLE){
		deepback_pre_proc(&det_key);
	}
#endif
	
	if (det_key){
		key_change_proc();
	}
	

	if(blt_state!=BLT_LINK_STATE_CONN){ //when the connect disconnect
		bRspMtuFlag =1;
	}

	 //long press voice 1 second
	if(key_voice_press && !ui_mic_enable && clock_time_exceed(key_voice_pressTick,1000000)){
		voice_press_proc();
	}

#if(DEEPBACK_FAST_KEYSCAN_ENABLE)
	if(deepback_key_state != DEEPBACK_KEY_IDLE){
		deepback_post_proc();
	}
#endif
}

void blt_pm_proc(void)
{
#if(BLE_REMOTE_PM_ENABLE)
	if( ui_mic_enable || ota_is_working	){
		blt_enable_suspend(SUSPEND_DISABLE);
	}
	else{
		blt_enable_suspend(SUSPEND_ADV | SUSPEND_CONN);

		user_task_ongoing_flg = key_not_released || DEVICE_LED_BUSY;

		if(key_not_released || DEVICE_LED_BUSY){
	#if (LONG_PRESS_KEY_POWER_OPTIMIZE)
			extern int key_matrix_same_as_last_cnt;
			if(key_matrix_same_as_last_cnt > 5){  //key matrix stable can optize
				blt_user_change_latency(4);  //7.5*(4+1) = 37.5 ms
			}
			else{
				blt_set_latency_off();  //latency off: 0
			}
	#else
			blt_set_latency_off();
	#endif
		}

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

		//adv 60s, deepsleep
		if( blt_state == BLT_LINK_STATE_ADV && clock_time_exceed(blt_get_adv_begin_tick() ,60 * 1000 * 1000)){
			blt_enable_suspend (DEEPSLEEP_ADV); //set deepsleep
			blt_set_wakeup_source(PM_WAKEUP_PAD);  //gpio PAD wakeup deesleep
			analog_write(DEEP_ANA_REG0,ADV_DEEP_FLG);
		}
		//conn 60s no event(key/voice/led), enter deepsleep
		else if( blt_state == BLT_LINK_STATE_CONN && clock_time_exceed(latest_user_event_tick,60 * 1000 * 1000) \
			&& !user_task_ongoing_flg){

			blt_terminate();  //push terminate cmd into ble TX buffer
			sendTerminate_before_enterDeep = 1;
		}


		#if (STUCK_KEY_PROCESS_ENABLE)
		if(key_not_released && clock_time_exceed(stuckKey_keyPressTime, STUCK_KEY_ENTERDEEP_TIME*1000000)){
			u32 pin[] = KB_DRIVE_PINS;
			for (int i=0; i<(sizeof (pin)/sizeof(*pin)); i++)
			{
				extern u8 stuckKeyPress[];
				if(stuckKeyPress[i]){
					cpu_set_gpio_wakeup (pin[i],0,1);  //reverse stuck key pad wakeup level
				}
			}
			blt_enable_suspend (DEEPSLEEP_CONN); //set deepsleep
			blt_set_wakeup_source(PM_WAKEUP_PAD);  //gpio PAD wakeup deesleep
		}
		#endif
	}

#endif  //END of  BLE_REMOTE_PM_ENABLE
}

void blt_system_power_optimize(void)  //to lower system power
{
	//disable_unnecessary_module_clock
	reg_rst_clk0 &= ~FLD_RST_SPI;  //spi not use
	reg_rst_clk0 &= ~FLD_RST_I2C;  //iic not use
#if(!MODULE_USB_ENABLE) //if usb not use
	reg_rst_clk0 &= ~(FLD_RST_USB | FLD_RST_USB_PHY);
#endif

#if(!BLE_REMOTE_UART_ENABLE) //if uart not use
	reg_clk_en1 &= ~(FLD_CLK_UART_EN);
#endif

}


//shut down some io before entry suspend,in case of io leakage
//restore them after suspend wakeup
//PC3:bias,if not shut down,150 uA leakage at suspend state

void io_isolate_before_suspend(void)
{

}

void restore_io_after_suspend(void)
{

}

void entry_ota_mode(void)
{
	ota_is_working = 1;
	device_led_setup(led_cfg[LED_SHINE_OTA]);
	ble_setOtaTimeout(15 * 1000 * 1000); //set OTA timeout  15 seconds
}



void LED_show_ota_result(int result)
{
#if 0
	irq_disable();
	WATCHDOG_DISABLE;

	gpio_set_output_en(GPIO_LED, 1);

	if(result == OTA_SUCCESS){  //OTA success
		gpio_write(GPIO_LED, 1);
		sleep_us(2000000);  //led on for 2 second
		gpio_write(GPIO_LED, 0);
	}
	else{  //OTA fail

	}

	gpio_set_output_en(GPIO_LED, 0);
#endif
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
	u32 pin[] = KB_DRIVE_PINS;
	for (int i=0; i<(sizeof (pin)/sizeof(*pin)); i++)
	{
		gpio_set_wakeup(pin[i],1,1);  	   //drive pin core(gpio) high wakeup suspend
		cpu_set_gpio_wakeup (pin[i],1,1);  //drive pin pad high wakeup deepsleep
	}
	gpio_core_wakeup_enable_all(1);


#if(KEYSCAN_IRQ_TRIGGER_MODE)
	gpio_core_irq_enable_all(1);
	reg_irq_src = FLD_IRQ_GPIO_EN;
#endif


#if (BLE_DMIC_ENABLE)  //Dmic config
	/////////////// DMIC: PA0-data, PA1-clk, PA3-power ctl
	gpio_set_func(GPIO_PA0, AS_DMIC);
	*(volatile unsigned char  *)0x8005b0 |= 0x01;  //PA0 as DMIC_DI
	gpio_set_func(GPIO_PA1, AS_DMIC);

	gpio_set_func(GPIO_PA3, AS_GPIO);
	gpio_set_input_en(GPIO_PA3, 1);
	gpio_set_output_en(GPIO_PA3, 1);
	gpio_write(GPIO_PA3, 0);

	Audio_Init(1, 0, DMIC, 26, 4, R64|0x10);  //16K
	Audio_InputSet(1);
#else  //Amic config
	//////////////// AMIC: PC3 - bias; PC4/PC5 - input
	#if TL_MIC_32K_FIR_16K
		#if (CLOCK_SYS_CLOCK_HZ == 16000000)
			Audio_Init( 1, 0, AMIC, 47, 4, R2|0x10);
		#elif (CLOCK_SYS_CLOCK_HZ == 48000000)
			Audio_Init( 1, 0, AMIC, 65, 15, R3|0x10);
		#endif
	#else
		#if (CLOCK_SYS_CLOCK_HZ == 16000000)
			Audio_Init( 1,0, AMIC,18,8,R5|0x10);
		#elif (CLOCK_SYS_CLOCK_HZ == 48000000)
			Audio_Init( 1,0, AMIC,65,15,R6|0x10);
		#endif
	#endif

	Audio_FineTuneSampleRate(3);//reg0x30[1:0] 2 bits for fine tuning, divider for slow down sample rate
	Audio_InputSet(1);//audio input set, ignore the input parameter
#endif
    adc_BatteryCheckInit(2);///add by Q.W

	config_mic_buffer ((u32)buffer_mic, TL_MIC_BUFFER_SIZE);
	blt_register_event_callback (BLT_EV_FLAG_BRX, &task_audio);

	blt_register_event_callback (BLT_EV_FLAG_CONNECT, &task_connect);
	blt_register_event_callback (BLT_EV_FLAG_TERMINATE, &ble_remote_terminate);
	blt_register_event_callback (BLT_EV_FLAG_EARLY_WAKEUP, &proc_keyboard);
	//blt_register_event_callback (BLT_EV_FLAG_CONN_PARA_REQ, &conn_para_update_req_proc);

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
        flash_write_page (CFG_ADR_MAC, 6, tbl_mac);
    }

	rf_set_power_level_index (RF_POWER_8dBm);
	blt_init (tbl_mac, tbl_adv, tbl_rsp);


	extern void my_att_init ();
	my_att_init ();


	if(analog_read(DEEP_ANA_REG0) == CONN_DEEP_FLG){  //conn deepback: directed low duty
		blt_set_adv_channel (BLT_ENABLE_ADV_ALL);
		blt_set_adv_type(ADV_TYPE_CONNECTABLE_DIRECTED_LOW_DUTY);  //send direct adv
		blt_set_adv_interval (10000);  //10 ms adv_interval

		//note: when duration timeout, adv will disabled by ble stack
		//you can enable it again with : void blt_set_adv_enable(void) in BLT_EV_FLAG_ADV_DURATION_TIMEOUT callback
		blt_set_adv_duration (2000000, 1);  //direct adv last for 2s
		blt_register_event_callback (BLT_EV_FLAG_ADV_DURATION_TIMEOUT, &adv_type_switch);
	}
	else{  //never bonded: undirected adv
		blt_set_adv_channel (BLT_ENABLE_ADV_37); //only one channel to save power, you can change it
		blt_set_adv_type(ADV_TYPE_CONNECTABLE_UNDIRECTED);  //undirected adv, default type
		blt_set_adv_interval (30000);  //adv interval 30ms
		blt_set_adv_duration (0, 0);  //duration disable
	}

	blt_set_adv_enable(1);  //adv enable


#if(BLE_REMOTE_PM_ENABLE)
	blt_enable_suspend (SUSPEND_ADV | SUSPEND_CONN);
	blt_register_event_callback (BLT_EV_FLAG_SET_WAKEUP_SOURCE, &ble_remote_set_sleep_wakeup);
	//blt_system_power_optimize();
#else
	blt_enable_suspend (SUSPEND_DISABLE);
#endif


	/////////////////////////////////////////////////////////////////
	ll_whiteList_reset();


	////////////////// OTA relative ////////////////////////
	ble_setOtaStartCb(entry_ota_mode);
	ble_setOtaResIndicateCb(LED_show_ota_result);

	device_led_init(GPIO_LED, 1);
}

void Battery_get_and_filter(u8 Chn_sel, u8 len)
{
	adc_AnaChSet(Chn_sel);
	adc_clk_poweron();
	adc_BatteryCheckInit(0);
	int j;
	u16 temp;
	u8 m_num = len >> 1;
	for(int i=0; i<len; i++){
		sleep_us(3);
		BattValue[i] = adc_BatteryValueGet();
		if(i){
			if(BattValue[i] < BattValue[i-1]){
				temp = BattValue[i];
				//BattValue[i] = BattValue[i-1];
				for(j = i-1; j>=0 && BattValue[j] > temp; j--){
					BattValue[j+1] = BattValue[j];
				}
				BattValue[j+1] = BattValue[j];
			}
		}
	}
	BattValue[0] = (BattValue[m_num] +  BattValue[m_num -1] +  BattValue[m_num + 1]) / 3;
}



/////////////////////////////////////////////////////////////////////
// main loop flow
/////////////////////////////////////////////////////////////////////
u32 tick_loop;
unsigned short battValue[20];
void main_loop ()
{
	tick_loop ++;

	////////////////////////////////////// BLE entry /////////////////////////////////
	blt_brx_proc ();

	////////////////////////////////////// UI entry /////////////////////////////////
	proc_keyboard (0,0);

#if 0
	if(!lowBattDet_enable){//////manual i.e battery dectect
		unsigned char i=0;
		for(i=0;i<20;i++)
		battValue[i] = adc_BatteryValueGet();
	}
#endif

	device_led_process();
	blt_pm_proc();

	////////////////////////////////////// Suspend entry /////////////////////////////
	blt_brx_sleep ();
}

#endif
