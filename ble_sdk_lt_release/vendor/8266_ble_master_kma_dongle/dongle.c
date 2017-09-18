#include "../../proj/tl_common.h"
#include "../../proj/mcu/watchdog_i.h"
#include "../../proj_lib/rf_drv.h"
#include "../../proj_lib/pm.h"
#include "../../proj_lib/ble_ll/ble_ll.h"
#include "../../proj_lib/ble_ll/ble_frame.h"

#include "../../proj/drivers/keyboard.h"
#include "../common/rf_frame.h"
#include "../common/tl_audio.h"
#include "../common/att_handle_index.h"
//#include "../../proj_lib/ble_l2cap/blueLight.h"

#include "trace.h"


#define  WEIGHT_SCALE_OTA			0


#if (DONGLE_8266_24PIN_F64)
	#define flash_adr_pairing    	0xd000
#else
	#define flash_adr_pairing    	0x11000
#endif

#define				DEBUG_DISABLE_SUSPEND			1
#define				ACTIVE_INTERVAL					24000			//24ms

/*Define OTA MODE for BLE light stack & BLE Full stack*/
#define OTA_BLE_LIGHTSTACK   1
#define OTA_BLE_FULLSTACK    0

#if OTA_BLE_LIGHTSTACK
#define TELINK_SPP_DATA_OTA 				{0x12,0x2B,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00} 		//!< TELINK_SPP data for ota
#define OTA_UUID_LEN   16

#elif  OTA_BLE_FULLSTACK
#define TELINK_SPP_DATA_OTA 				{0x12,0x2B} 		//!< TELINK_SPP data for ota
#define OTA_UUID_LEN   2

#endif
#if VIN_TEST_MODE
extern u8 	voice_timeout;
extern u8 change_voice[] ;
#endif 

#define UART_ENABLE             0

#if UART_ENABLE
#include "../../proj/drivers/uart.h"

#define UART_DATA_LEN    44      // data max 252
typedef struct{
    u32 len;        // data max 252
    u8 data[UART_DATA_LEN];
}uart_data_t;

uart_data_t T_txdata_user;
uart_data_t T_txdata_buf;      // not for user

uart_data_t T_rxdata_user;
uart_data_t T_rxdata_buf;   // data max 252, user must copy rxdata to other Ram,but not use directly
unsigned char uart_rx_true;
#endif

/*Store the last UUID called by read by type req command*/
//u16 read_by_type_req_uuid = 0;
u8 read_by_type_req_uuid[16] = {};
u16 remote_ota_handle = 0;

/*TELINK_SPP_DATA_OTA Should be the same for all Telink products to make OTA Tool compatible*/
const u8 my_OtaUUID[16]		= TELINK_SPP_DATA_OTA;


/*OTA State machine*/
u8 host_ota_start = 0;


u16 blt_master_interval = 8;   //init interval  = 8*1.25 = 10ms
u32 blt_master_timeout = 1500; //init timeout = 1500 ms = 1.5 S


extern rf_packet_ll_init_t	pkt_master_init;
/*****************************************************
pkt_master_init is defined as below(in lib file)
rf_packet_ll_init_t	pkt_master_init = {
		sizeof (rf_packet_ll_init_t) - 4,		// dma_len
		EVENT_PKT_CONNECT,				// type
		sizeof (rf_packet_ll_init_t) - 6,		// rf_len
		{0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5},	// scanA
		{0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5},	// advA
		{0xaa, 0x55, 0x55, 0xaa},				// access code
		{0x55, 0x55, 0x55},						// crcinit[3]
		0x02,									// wsize = 2*1.25 ms
		0x0005,									// woffset = 5* 1.25 ms
		8,										// interval: 32 * 1.25 ms = 40 ms
		0	,									// latency
		150,									// timeout: 1.5 second
		{0xff, 0xff, 0xff, 0xff, 0x1f},							// chm[5]
		0xac,									// hop & sca
};
 *******************************************************/

//winsize, winOff, interval, latency, timeout, hop, sca, chnMap
void blt_manual_set_conn_param(u16 winsize, u16 winOff, u16 interval, u16 latency, u32 timeout, u8 hop, u8 sca, u8 *chnMap)
{

	blt_master_interval = interval;
	blt_master_timeout = timeout*10;

	pkt_master_init.wsize = winsize;
	pkt_master_init.woffset = winOff;
	pkt_master_init.latency = latency;
	pkt_master_init.hop = hop | (sca<<5);

	//memcpy(pkt_master_init.chm, chnMap, 5);
}


extern rf_packet_ll_init_t	pkt_master_init;

extern				void abuf_init ();
extern				void abuf_mic_add (u8 *p);
extern				void abuf_mic_dec (void);
extern				void abuf_dec_usb (void);
extern void usbkb_hid_report(kb_data_t *data);
typedef	void (*att_func_t) (rf_packet_att_write_data_t *p);
/*Read by type request, to get the handle of assigned 2-byte UUID*/
extern u8 pkt_read_by_type_req[];
void register_read_by_type_rsp_cb(att_func_t cb_func);
void  att_read_by_type_rsp_handler (rf_packet_att_write_data_t *p);

u16 current_red_req_handle;

void set_red_req_handle(u16 handle)
{
	current_red_req_handle = handle;
}

///////////////////////////////////////////////////////////////////////////
rf_custom_dat_t custom_cmd = {
		0xcccc,						//network id, reserved
		0xffff,						//group
		0xffffffff,					//destination id
		{0x12, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a},
	};

rf_packet_mouse_t	pkt_mouse = {
		sizeof (rf_packet_mouse_t) - 4,	// dma_len

		sizeof (rf_packet_mouse_t) - 5,	// rf_len
		RF_PROTO_BYTE,		// proto
		PKT_FLOW_DIR,		// flow
		FRAME_TYPE_MOUSE,					// type

//		U32_MAX,			// gid0

		0,					// rssi
		0,					// per
		0,					// seq_no
		1,					// number of frame
};

#if(KMA_DONGLE_OTA_ENABLE)
void proc_ota(void);
#endif


kb_data_t		kb_dat_debug = {1, 0, 0x04};
/////////////////// rf interrupt handler ///////////////////////////
int send_packet_usb (u8 *p, int n)
{
	for (int i=0; i<n; i++)
	{
		reg_usb_ep8_dat = *p ++;
	}
	reg_usb_ep8_ctrl = BIT(7);
	return n;
}

void  usb_iso_in_1k_square ()
{
	reg_usb_ep7_ptr = 0;

	/////// get MIC input data ///////////////////////////////
	for (int i=0; i<16; i++) {
		if (i & 8) {
			reg_usb_ep7_dat = 0;
			reg_usb_ep7_dat = 0x40;
		}
		else {
			reg_usb_ep7_dat = 0;
			reg_usb_ep7_dat = 0xc0;
		}
	}
	reg_usb_ep7_ctrl = BIT(0);
}

void  usb_iso_in_from_mic ()
{
#if 0
	mic_wptr = reg_audio_wr_ptr;
	reg_usb_ep7_ptr = 0;
	/////// get MIC input data ///////////////////////////////
	for (int i=0; i<16 && mic_rptr != mic_wptr; i++) {
		s16 md = buffer_mic[mic_rptr];

		if (ui_filter_on) {
			md = noise_supression (md);
		}
		//md = (md + 1) >> 1;
		reg_usb_ep7_dat = md;
		reg_usb_ep7_dat = md >>8;
		mic_rptr = (mic_rptr + 1) & ((MIC_BUFFER_SIZE>>2) - 1);
	}
	reg_usb_ep7_ctrl = BIT(0);
#endif
}

u32			tick_iso_in;
int			mode_iso_in;
_attribute_ram_code_ void  usb_endpoints_irq_handler (void) {

	u32 t = clock_time ();
	/////////////////////////////////////
	// ISO IN
	/////////////////////////////////////
	if (reg_usb_irq & BIT(7)) {
		mode_iso_in = 1;
		tick_iso_in = t;
		reg_usb_irq = BIT(7);	//clear interrupt flag of endpoint 7

		/////// get MIC input data ///////////////////////////////

		//usb_iso_in_1k_square ();
		//usb_iso_in_from_mic ();
		abuf_dec_usb ();

	}

}


_attribute_ram_code_ void irq_handler(void)
{
	irq_ble_master_handler ();

	// usb end-point packet transfer interrupt
	if(reg_irq_src & FLD_IRQ_IRQ4_EN){
		usb_endpoints_irq_handler();
	}

#if UART_ENABLE
    static unsigned char enterRXIrq,enterTXIrq;
    unsigned char irqS = uart_IRQSourceGet_kma();
    if(irqS & UARTRXIRQ_MASK){
        uart_rx_true = 1;
        enterRXIrq++;
    }

    if(irqS & UARTTXIRQ_MASK){
        uart_clr_tx_busy_flag();
        enterTXIrq++;
    }
#endif
}



extern u8	ble_rssi_advPkt;
extern u8	ble_rssi_current;

/*********************************************************************************
ble_rssi_current : the new pkt rssi read from hardware
ble_rssi_advPkt  : the result you process ble_rssi_current,
                    in first paring,if  ble_rssi_advPkt > paring_rssi_thres,  paring enable.
  				  paring_rssi_thres is set by : void ble_master_set_paring_thres(void);

void	ble_master_set_paring_thres (u8 thres)
{
	paring_rssi_thres = thres;
}
**********************************************************************************/
#if 0
_attribute_ram_code_ void adv_pkt_rssi_filter(void)  //this func must on ram to save run time
{
    static u8 rssi_last;
    if ( abs(rssi_last - ble_rssi_current) < 12 ){
    	ble_rssi_advPkt = ( ( (ble_rssi_advPkt<<1) + ble_rssi_advPkt + ble_rssi_current ) >> 2 );
    }
    rssi_last = ble_rssi_current;
}
#endif




att_func_t		att_func[MAX_ATT_FUNC] = {0};

void	register_att_function (u8 h, att_func_t f)
{
	if (h < MAX_ATT_FUNC)
	{
		att_func[h] = f;
	}
}

/////////////////////// att_read_rep_handler  ///////////////////////////////////////
u8 att_read_value[8] ={0}; //read this var in tdebug
int cmd_no_accept = 0;
#define reg_cmd 0x8000
#define reg_datalen 0x8002
#define reg_att_read 0x8008     // ~ 0x800f 8 bytes
#define reg_cmd_result 0x8008   // cmd exe result

#define SET_CMD_DONE   write_reg8(0x8000,0);   \
					   cmd_no_accept = 0

int pc_tool_send_ota_cmd = 0;
void  att_read_rep_handler (rf_packet_att_write_data_t *p)
{
    if (current_red_req_handle > MAX_ATT_FUNC) return;

    int i;
    //clear reg
    for(i=0; i<8 ;i++)
    {
        att_read_value[i] =0;
        write_reg8( (reg_att_read+i), 0);
    }

    // if a read response
    if (p->opcode == ATT_OP_READ_RSP)
    {
        rf_packet_att_read_rsp_t *rsp  = p;
        memcpy (att_read_value, rsp->dat, (int)rsp->l2capLen-1);
        //write to memory
        write_reg16( reg_datalen, rsp->l2capLen-1);
        if ((rsp->l2capLen-1) <= 8) // data register space 0x8008~0x800f
            {for(i=0; i<(rsp->l2capLen -1) ;i++)  write_reg8( (reg_att_read+i), *(rsp->dat+i));}
        else
            {for(i=0; i<8 ;i++)  write_reg8( (reg_att_read+i), *(rsp->dat+i));}
        set_red_req_handle(NULL);
        SET_CMD_DONE;// done
    }
    // a notify
    // db dd 56 1b
    // 0a 0b
    // 07 00    //l2caplen
    // 04 00    //chanid
    // 1b       //opcode
    // 28 00    //handle handle1
    // 81       //data[0]
    else if (p->opcode == ATT_OP_HANDLE_VALUE_NOTI)
    {
        memcpy (att_read_value, p->dat, (int)p->l2capLen-5);
        //write to memory
        write_reg16( reg_datalen, p->l2capLen-5);
        if ((p->l2capLen-5) <= 8) // data register space 0x8008~0x800f
            {for(i=0; i<(p->l2capLen -5) ;i++)  write_reg8( (reg_att_read+i), *(p->dat+i));}
        else
            {for(i=0; i<8 ;i++)  write_reg8( (reg_att_read+i), *(p->dat+i));}
        set_red_req_handle(NULL); // done
    }

}

/////////////////////// mic handle ///////////////////////////////////////
u8		att_mic_rcvd = 0;
u32		tick_adpcm;
u8		buff_mic_adpcm[MIC_ADPCM_FRAME_SIZE];
#if TEST_END_SEND_SLEEP
u8 		test_OK_flag =0;//BIT(0) means key test ok ,and BIT(1)means audio test ok 
u8		test_send_sleep_flag =0;
u32 	test_send_tick ;
u8 		send_clr_ui_flag =0;
#endif 

#if DONGLE_AUDIO_CONTROL
extern u8  vin_sample_OK_cnt ;
extern u8  vin_sample_Fail_cnt ;

u8 audio_play_flag =0;
u32 audio_play_tick =0;
u8 audio_control_dat =0;
u8 audio_run_flag =0;

void	audio_control (rf_packet_att_write_data_t *p)
{
	//clean the cnt of the audio play
	audio_control_dat = p->dat[0];
	vin_sample_OK_cnt =0;
	vin_sample_Fail_cnt =0;
	//remeber the tick of the audio
	audio_play_flag =1;
	audio_play_tick =clock_time();
	return ;
}
void    audio_play_dis()
{
	u8 *store_info;
	store_info = (volatile u8*) (0x808004);
	if(audio_control_dat){
		store_info[2]=0xee;//when the 8006 is 0xee ,means only detect the key of the audio control 
		store_info[10]|=0x80;
		store_info[0]++;
		audio_run_flag=1;

		
	}else{
		store_info[2]=0xee;
		store_info[10]&=~(0x80);
		store_info[0]++;
		audio_run_flag=0;
		//if audio sending is end ,add dispatch 
		
	}
}

#endif 
void	att_mic (rf_packet_att_write_data_t *p)
{
	if (p->l2capLen >= 74 && p->opcode == 0x1b)			//notify data
	{
		att_mic_rcvd = 1;
		memcpy (buff_mic_adpcm, p->dat, MIC_ADPCM_FRAME_SIZE);
		abuf_mic_add ((u32 *)buff_mic_adpcm);
		// loop back receiving data
		//master_write_att_data (p->handle + 3, buff_mic_adpcm, 152);
	}
}

//////////////////////// mouse handle ////////////////////////////////////////
void	att_mouse (rf_packet_att_write_data_t *p)
{
	if (p->l2capLen >= 4 && p->opcode == 0x1b)			//notify data
	{
		memcpy (pkt_mouse.data, p->dat, 4);
        pkt_mouse.seq_no++;
        usbmouse_add_frame(&pkt_mouse);
	}
}


#define	SIM_TOUCH_HANDLE				46

#define COORDS_DATA		0xff
#define TOUCH_EV		0xfe
#define SLIDE_EV		0xfd

#define DIR_INC						1
#define DIR_DEC						2

u8 aaa_touch_cnt;
u8 aaa_slide_cnt;

u8 aaa_index = 0;
//u8 aaa_data[32];

#define  ADD_DATA   do{if(aaa_index < 16){memcpy(aaa_data+((aaa_index++)<<1),p->dat,2);}}while(0)
void att_touch_sim(rf_packet_att_write_data_t *p)
{

	static u32 report_tick;
	if (p->opcode == 0x1b)			//notify data
	{
		if(p->dat[0] == TOUCH_EV){
			//ADD_DATA;
			//aaa_touch_cnt ++;
			printf("---------touch  %02x---------\n",p->dat[1]);
		}
		else if(p->dat[0] == SLIDE_EV){
			//ADD_DATA;
			//aaa_slide_cnt++;
			if(p->dat[1] == DIR_INC){
				printf("---------slide inc---------\n");
			}
			else if(p->dat[1] == DIR_DEC){
				printf("---------slide dec---------\n");
			}
		}
		else if(p->dat[0] == COORDS_DATA){
			if(clock_time_exceed(report_tick,500000)){
				printf("\n\n%02x,%02x,%04d  %01d,%02x  %02x,%02x,%02x,%02x  %02x,%02x\n",\
										    		p->dat[1],p->dat[2],(p->dat[4]<<8) | p->dat[3],\
										    		p->dat[5],p->dat[6],\
										    		p->dat[7],p->dat[8],p->dat[9],p->dat[10],\
										    		p->dat[11],p->dat[12]);
			}
			else{
				printf("%02x,%02x,%04d  %01d,%02x  %02x,%02x,%02x,%02x  %02x,%02x\n",\
						    		p->dat[1],p->dat[2],(p->dat[4]<<8) | p->dat[3],\
						    		p->dat[5],p->dat[6],\
						    		p->dat[7],p->dat[8],p->dat[9],p->dat[10],\
						    		p->dat[11],p->dat[12]);
			}

		    report_tick = clock_time();
		}
	}
}



///////////////////////////////////////////////////////////////////////////////
//		call back function
///////////////////////////////////////////////////////////////////////////////
//u8 cb_status[64]= {0x0};
u8 red_light_status = 0;
_attribute_ram_code_ void ble_event_callback (u8 status, u8 *p)
{

	//cb_status[status] = 1;
    switch(status)
    {
        case FLG_SYS_LINK_CONNECTED:
            gpio_write(GPIO_LED_RED, LED_ON_LEVAL);     //red on
            red_light_status = 1;
            gpio_write(GPIO_LED_WHITE, !LED_ON_LEVAL);
	        break;
        case FLG_SYS_LINK_LOST:
        	if(red_light_status){
        		gpio_write(GPIO_LED_WHITE, LED_ON_LEVAL);   //white on
        	}
            reg_dma_tx_rptr = FLD_DMA_RPTR_CLR;         //clear fifo
            SET_CMD_DONE;
	        break;
        case FLG_SYS_LINK_STOP:
            gpio_write(GPIO_LED_RED, !LED_ON_LEVAL);    //red off
            red_light_status = 0;
	      break;
    }

}


////////////////////// keyboard: multimedia from consumer report ////////////////
#define			CR_VOL_UP		0x0  //0000 01
#define			CR_VOL_DN		0x1  //0000 10
#define			CR_VOL_MUTE		0x2  //0001 00
#define			CR_POWER		0x3  //0010 00
#define			CR_SEL			0x4  //0011 00
#define			CR_UP			0x5  //0100 00
#define			CR_DN			0x6  //0101 00
#define			CR_LEFT			0x7  //0110 00
#define			CR_RIGHT		0x8  //0111 00
#define			CR_HOME			0x9  //1000 00
#define			CR_REWIND		0xa  //1001 00
#define			CR_NEXT			0xb  //1010 00
#define			CR_PREV			0xc  //1011 00
#define			CR_STOP			0xd  //1100 00


u8		cr_map_key[16] = {
		VK_VOL_UP, 	VK_VOL_DN,	VK_W_MUTE,	0,
		VK_ENTER,	VK_UP,		VK_DOWN,	VK_LEFT,
		VK_RIGHT,	VK_HOME,	0,			VK_NEXT_TRK,
		VK_PREV_TRK,VK_STOP,	0,			0
};

#define KEY_MASK_PRESS		0x10
#define KEY_MASK_REPEAT		0x20
#define KEY_MASK_RELEASE	0x30


u8 release_key_pending;
u32 release_key_tick;

void    report_to_pc_tool(u8 len,u8 * keycode)
{

#if 1  //pc tool verison_1.9 or later
		static u8 last_len = 0;
		static u8 last_key = 0;
		static u32 last_key_tick = 0;

		u8 mask = 0;

		if(!(read_reg8(0x8004)&0xf0)){ //pc tool cleared 0x8004
			if(!len){  //release
				write_reg8(0x8004,KEY_MASK_RELEASE);
				write_reg8(0x8005,0);
			}
			else{//press or repeat
				if(last_len==len && last_key==keycode[0]){//repeat
					mask = KEY_MASK_REPEAT;
				}
				else{ //press
					mask = KEY_MASK_PRESS;
				}
				write_reg8(0x8004,mask | len);
				write_reg8(0x8005,keycode[0]);
			}
		}
		else{  //pc tool not clear t0x8004, drop the key
			if(!len){  //release can not drop
				release_key_pending = 1;
				release_key_tick = clock_time();
			}
		}

		last_len = len;
		last_key = keycode[0];
#else //old pc tool
		write_reg8(0x8004,len);
		write_reg8(0x8005,keycode[0]);
#endif
}
#if VIN_TEST_MODE
/*
const u8 key_tbl[16]={
	UEI_VOL_UP,UEI_VOL_DN,UEI_POWER,UEI_VOICE_SEARCH,
	UEI_UP,UEI_LEFT,UEI_SELECT,UEI_RIGHT,
	UEI_DOWN,UEI_BACK,UEI_HOME,UEI_MENU,
	UEI_LANUCH_TV,UEI_MEDIA,UEI_BROWER,UEI_APPS
};*/
//make the key follow as sequence 
const u8 key_tbl[16]={
	UEI_POWER,UEI_VOICE_SEARCH,UEI_UP,UEI_LEFT,
	UEI_SELECT,UEI_RIGHT,UEI_DOWN,UEI_BACK,
	UEI_HOME,UEI_MENU,UEI_LANUCH_TV,UEI_VOL_UP,
	UEI_MEDIA,UEI_BROWER,UEI_VOL_DN,UEI_APPS
};

u16 key_tbl_mask =0;
#if DIS_KEY_PRESS_SEQ_ENABLE
u8  key_no_now =0;
u8  key_no_last = 0;
u8  key_test_flag =0;//set BIT(7) means test OK,BIT(6)means test fail 
#endif 
u8 dispatch_key_tbl(u8 data)
{
	u8 i;
	for(i=0;i<sizeof(key_tbl);i++){
		if(data == key_tbl[i]){ 
			key_tbl_mask|=BIT(i);
			#if DIS_KEY_PRESS_SEQ_ENABLE
			key_no_now = i;
			#endif 
			break;
		}
	}
#if DIS_KEY_PRESS_SEQ_ENABLE
	// if you press the first key ,means you start the key test 
	if(data == key_tbl[0]){
		key_tbl_mask =0;
		key_tbl_mask |=BIT(0);
		key_no_now =0;
		key_no_last =0;
		key_test_flag &=~(BIT(7)|BIT(6));//clr the status 
		
	}else{
	//as the key sequence 
		if(key_no_now == key_no_last+1){//key sequence is OK
			key_no_last = key_no_now ;//give the last 
		}else{
			if(data!=key_tbl[1]){
				key_test_flag |=BIT(6);//send the key test fail flag
			}
			key_no_last = key_no_now ;//update the key_no_last  
		}
	}
#endif 
	if(key_tbl_mask == 0xffff){
	#if DIS_KEY_PRESS_SEQ_ENABLE
		if(!(key_test_flag&BIT(6))){
				key_test_flag |= BIT(7);//test OK flag 
			}
	#endif 
		key_tbl_mask =0;
		return 1;
	}
	return 0;
}
u8 clr_ui_flag =0;
u32 clr_ui_tick =0;
void  set_key_simulator(rf_packet_att_write_data_t *p)
{
	u8 *store_info;
	store_info = (volatile u8*) (0x808004);
	store_info[1]=1;
	store_info[2]=0;//clear the 0x1e and 0xff ,to update the key data 
	if(p->dat){
		store_info[4]=1;
	}else{
		//store_info[4]=0;
	}
	store_info[5]=p->dat[0]; 
	if(p->dat[0]){
		//when all the keys are pressed ,and reset the UI
		if(dispatch_key_tbl(p->dat[0])&&(key_test_flag&BIT(7))){
			//store_info[2]=0xfe;//clear the button ui ,and the 0xff is clear all the ui 
			clr_ui_flag =1;
			clr_ui_tick =clock_time();
		#if TEST_END_SEND_SLEEP
			test_OK_flag|=BIT(0);
			if(test_OK_flag==(BIT(0)|BIT(1))){
					test_send_sleep_flag =1;
					test_send_tick =clock_time();
			}
		#endif 
		}
	}
	#if DIS_KEY_PRESS_SEQ_ENABLE
	store_info[11]&=~(3<<KEY_TEST_BIT_POS);//clr two high bits 
	if(key_test_flag&BIT(7)){
		store_info[11]|=TEST_OK<<KEY_TEST_BIT_POS;
	}else if(key_test_flag&BIT(6)){
		store_info[11]|=TEST_FAIL<<KEY_TEST_BIT_POS;
	}else{
		store_info[11]|=TEST_UNKOWN<<KEY_TEST_BIT_POS;
	}
	#endif 
	store_info[6]=0x00;
	store_info[7]=0x00;
	store_info[8]=0x01;
	store_info[9]=0x01;
	store_info[0]++;
}
#endif 

void	att_keyboard_media (rf_packet_att_write_data_t *p)
{
	if (p->l2capLen >= 2 && p->opcode == 0x1b)			//notify data
	{
		//send_packet_usb (p + 6, p[5]);
		#if VIN_TEST_MODE 
		if(p->dat[0]){
			set_key_simulator(p);
			}
		else{
			// if the audio key release ,and we will reset the audio test cnt 
				extern u8  vin_sample_OK_cnt ;
				extern u8  vin_sample_Fail_cnt ;
				vin_sample_OK_cnt =0;
				vin_sample_Fail_cnt =0;
		}
		#endif 
		u16 bitmap = p->dat[0] + p->dat[1] * 256;
		kb_dat_debug.cnt = 0;
		kb_dat_debug.keycode[0] = 0;
		for (int i=0; i<16; i++)
		{
			if (bitmap & BIT(i))
			{
				kb_dat_debug.cnt = 1;
				kb_dat_debug.keycode[0] = cr_map_key[i];// cr_map_key[i];
				break;
			}
		}
#if CUSTOMER_2_MODE
		switch(bitmap){
		case 0x20: kb_dat_debug.keycode[0] = VK_WEB ;break;
		case 0x80: kb_dat_debug.keycode[0] = VK_SLEEP ;break;
		}
#endif

	#if VIN_TEST_MODE
	#else
		if(read_reg8(0) == 0x5a){ //report to pc_tool  mode
			report_to_pc_tool(kb_dat_debug.cnt,kb_dat_debug.keycode);
		}
		else{
			usbkb_hid_report((kb_data_t *) &kb_dat_debug);
		}
	#endif
	}
}


int slave_send_unpair_req;
int slave_send_unpair_tick;
//////////////// keyboard ///////////////////////////////////////////////////
void	att_keyboard (rf_packet_att_write_data_t *p)
{
	if (p->l2capLen >= 3 && p->opcode == 0x1b)			//notify data
	{
		//send_packet_usb (p + 6, p[5]);
		if(p->dat[1] == 0xff){  //for kt_remote use only:slave unpair cmd
			slave_send_unpair_req = 1;
			slave_send_unpair_tick = clock_time();
			return;
		}

		kb_dat_debug.cnt = 0;
		kb_dat_debug.keycode[0] = 0;
		if (p->dat[2])  //keycode[0]
		{
			kb_dat_debug.cnt = 1;
			kb_dat_debug.keycode[0] = p->dat[2];
		}

		if(read_reg8(0) == 0x5a){ //report to pc_tool  mode
			report_to_pc_tool(kb_dat_debug.cnt,kb_dat_debug.keycode);
		}
		else{
			usbkb_hid_report((kb_data_t *) &kb_dat_debug);
		}
	}
}


void debug_mouse (void) {
	static u32 tick_m;
	if (reg_usb_host_conn && clock_time_exceed (tick_m, 8000)) {
		tick_m = clock_time ();
        pkt_mouse.data[1] = pkt_mouse.seq_no & BIT(7) ? 6 : -6;
        pkt_mouse.data[2] = pkt_mouse.seq_no & BIT(6) ? -6 : 6;
        pkt_mouse.seq_no++;
        usbmouse_add_frame(&pkt_mouse);
	}
}

void	debug_keyboard ()
{
	static u32 tick_k, dno_keyboard;
	if (reg_usb_host_conn && clock_time_exceed (tick_k, 100000)) {
		tick_k = clock_time ();
		kb_dat_debug.keycode[0] = 0x04 + (dno_keyboard & 0x1f);
		dno_keyboard++;
		usbkb_hid_report((kb_data_t *) &kb_dat_debug);
	}
}

u8 read_att_req[] = {
	0x09,0,0,0,	//dma_len
	0x02,   	//type
	0x07,		//rf_len

	0x03,0x00,  //l2caplen

	0x04,0x00,  //chanid

	0x0a, //opcode = Read Request

	0x00 , //11 , ex handle HANDLE_ALERT_LEVEL
	0x00, // handle1
};


u8 sihui_debug_cmd;
void proc_read_att ()
{
    // use register reg_cmd for test
    static u16 exe_pair=0;
    static u8  exe_unpair=0;
    static u16 cmd_time_out_counter=0;


    u8 cmd = read_reg8(reg_cmd);
    // if there is command remained , need set up a timeout, or may has a stuck
    if (cmd_no_accept) {
        if(cmd_time_out_counter++ > 0x8ff0){
            cmd_no_accept = 0;
            cmd_time_out_counter = 0;
        }
    }

    if (cmd !=0 && !cmd_no_accept) {
    	sihui_debug_cmd = cmd;

    	cmd_no_accept = 1;  //not accept other cmd before the old cmd is done

        if (cmd < MAX_ATT_FUNC)
        {
            read_att_req[11] = cmd; //  ex :HANDLE_ALERT_LEVEL 0x35
            reg_dma_tx_fifo = (u16) read_att_req;
            set_red_req_handle(cmd);
            if (cmd ==HANDLE_ALERT_LEVEL ){
            	SET_CMD_DONE; // buzzer response have delay
            }

        }
        else if(cmd >= CONTROL_START) //control cmd
        {
            switch(cmd)
            {
                case CONTROL_PAIR:
                    //pair
                    exe_pair = 0x4000; //pair retry
                    //ble_master_pairing_enable (TRUE); // GPIO_PD5 for dongle pcb version3.4, GPIO_PC5 for 3.2
                    write_reg8(reg_cmd_result, 1);// control ok
                    SET_CMD_DONE;
                    break;
                case CONTROL_UNPAIR:
                    //unpair
                    exe_unpair = 0x02;//0x20;
                    //ble_master_terminate_enable (TRUE); //GPIO_PD4 for unpair, dongle pcb version3.4
                    write_reg8(reg_cmd_result, 1);// control ok
                    SET_CMD_DONE;
                    break;
                case CONTROL_OTA:
                	pc_tool_send_ota_cmd = 1;
                    break;
                case CONTROL_REBOOT:  //reboot dongle
                	usb_dp_pullup_en (0);
                	cpu_sleep_wakeup(1, PM_WAKEUP_TIMER, clock_time() + 10 * CLOCK_SYS_CLOCK_1MS);
                	while(1);
                	break;

                default:
                    write_reg8(reg_cmd_result, 0);// control NG/ unknow cmd
                    break;

            }

        }
    }
    if (exe_pair)
    {
        exe_pair--;
        ble_master_pairing_enable (TRUE); // GPIO_PD5 for dongle pcb version3.4, GPIO_PC5 for 3.2
    }
    if (exe_unpair)
    {
        exe_unpair--;
        ble_master_terminate_enable (exe_unpair);
    }




}




u16		buff_adpcm[64];
u32		dbg_abuf_init;
void main_loop(void)
{
	static u32 dbg_st, dbg_m_loop;
	dbg_m_loop ++;

#if UART_ENABLE
    if(uart_rx_true){
        uart_rx_true = 0;
        u32 rx_len = T_rxdata_buf.len + 4 > sizeof(T_rxdata_user) ? sizeof(T_rxdata_user) : T_rxdata_buf.len + 4;
        memcpy(&T_rxdata_user, &T_rxdata_buf, rx_len);
        uart_Send_kma((u8 *)(&T_rxdata_user));
    }
#endif

	usb_handle_irq();  //proc usb cmd from host

#if DONGLE_AUDIO_CONTROL
	if(clock_time_exceed(audio_play_tick,30*1000)&&audio_play_flag){
		audio_play_flag =0;
		audio_play_tick =clock_time();
		audio_play_dis();
	}
	if(clock_time_exceed(clr_ui_tick,50*1000)&&clr_ui_flag){
		clr_ui_flag =0;
		clr_ui_tick =clock_time();
		u8 *store_info;
		store_info = (volatile u8*) (0x808004);
		//store_info[2]=0xfe; 
		//store_info[0]++;
	}
#endif 
#if TEST_END_SEND_SLEEP
	if(test_send_sleep_flag&&clock_time_exceed(test_send_tick,100*1000)){
		static u8 A_dgb_test =0;
		A_dgb_test++;
		change_voice[13]=CMD_SEND_DEEP;
		master_push_fifo(change_voice);
		change_voice[13]=CMD_SEND_TIMEOUT;
		test_send_sleep_flag =0;
		//test_send_tick =clock_time();
		test_OK_flag =0;//clr the flag of the key test ok and key test fail
	// the send sleep cmd is early than the cmd of the ctrl audio stop ,so we should mannul dispatch the audio stop cmd 
		vin_sample_OK_cnt =0;//clr audio part dispatch 
		vin_sample_Fail_cnt =0;
		u8 *store_info;//send stop to the dongle 
		store_info = (volatile u8*) (0x808004);
		store_info[2]=0xee;
		store_info[10]&=~(0x80);
		store_info[0]++;
		audio_run_flag=0;
		send_clr_ui_flag =1;
	}
	if(send_clr_ui_flag&&clock_time_exceed(test_send_tick,150*1000)){
		send_clr_ui_flag =0;
		test_send_tick =clock_time();
		u8 *store_info;
		store_info = (volatile u8*) (0x808004);
		store_info[2]=0xfe; 
		store_info[0]++;
	}
#endif 


	// proc audio
	if (att_mic_rcvd)
	{
		tick_adpcm = clock_time ();
		att_mic_rcvd = 0;
	}
	if (clock_time_exceed (tick_adpcm, 200000))
	{
		tick_adpcm = clock_time ();
		abuf_init ();
		dbg_abuf_init++;
	}
	abuf_mic_dec ();


	ble_master_pairing_enable (!gpio_read (SW1_GPIO)); // GPIO_PD5 for dongle pcb version3.4, GPIO_PC5 for 3.2
	ble_master_terminate_enable (!gpio_read (SW2_GPIO) || slave_send_unpair_req); //GPIO_PD4 for unpair, dongle pcb version3.4
	if(slave_send_unpair_req && clock_time_exceed(slave_send_unpair_tick,100000)){
		slave_send_unpair_req = 0;
	}

    proc_read_att();

    //proc OTA
#if(KMA_DONGLE_OTA_ENABLE)
#if (WEIGHT_SCALE_OTA)
    static u32 btn_release_tick = 0;
    static u8 btn_press_flg = 0;
    static u8 btn_release_flg = 0;

    if(!gpio_read (SW1_GPIO)){  //button press
    	//btn_press_flg = 1;
    	if(clock_time_exceed(btn_release_tick, 3000000)){
    		pc_tool_send_ota_cmd = 1;
    	}
    }
    else{  //not press
    	//btn_release_flg = 1;
    	btn_release_tick = clock_time();
    }
#endif
	proc_ota();
#endif


	//process slave send terminate: master should delay several interval to make sure slave rcvd ack
	extern u8 ble_terminate_pending;
	extern void ble_master_back_to_idle(void);
	if(ble_terminate_pending >= 6){
		ble_terminate_pending = 0;
		ble_master_back_to_idle();
	}


	if (1 && ! ble_master_status ())		// APP(master) emulation
	{
		dbg_st++;
#if VIN_TEST_MODE   // to make the test key to be faster
		blt_master_interval = 6; //7.5 ms
#endif
		ble_master_start (
				blt_master_interval, //interval
				blt_master_timeout,  //supervision time out
				&custom_cmd,
				32				// 32M: 32 tick per us
				);
	}
	else
	{

	}


	//proc pc_tool key release
	if(release_key_pending){
		if(!(read_reg8(0x8004)&0xf0)){ //pc tool cleared 0x8004
			write_reg8(0x8004,KEY_MASK_RELEASE);
			write_reg8(0x8005,0);
			release_key_pending = 0;
		}

		if(clock_time_exceed(release_key_tick,100000)){
			release_key_pending = 0;
		}
	}
}


/**********************************************************************************
				// proc   PAIR and UNPAIR
**********************************************************************************/
//flash  sector 0x11000: dongle stored rc mac address
//can not change this value

/* define pair slave max num,
   if exceed this max num, two methods to process new slave pairing
   method 1: overwrite the oldest one(telink use this method)
   method 2: not allow paring unness unpair happend  */
#define	PAIR_SLAVE_MAX_NUM       1  //telink use max 1

typedef struct {
	u8 address[6];
} macAddr_t;
typedef struct {
	u32 bond_flash_idx[PAIR_SLAVE_MAX_NUM];  //mark paired slave mac address in flash
	u8 curNum;
	macAddr_t bond_device[PAIR_SLAVE_MAX_NUM];
} salveMac_t;



//current connect slave mac adr: when dongle establish conn with slave, it will record the device mac adr
//when unpair happens, you can select this addr to delete from  slave mac adr table.
u8 slaveMac_curConnect[6];

/* flash erase strategy:
   never erase flash when dongle is working, for flash sector erase takes too much time(20-100 ms)
   this will lead to timing err
   so we only erase flash at initiation,  and with mark 0x00 for no use symbol
 */

#define ADR_BOND_MARK 		0x01
#define ADR_ERASE_MARK		0x00
/* flash stored mac address struct:
   every 8 bytes is a address area: first one is mark, second no use, third - eighth is 6 byte address
   	   0     1           2 - 7
   | mark |     |    mac_address     |
   mark = 0xff, current area is invalid, pair info end
   mark = 0x01, current area is valid, load the following mac_address,
   mark = 0x00, current area is invalid (previous valid address is erased)
 */

int		bond_slave_flash_cfg_idx;  //new mac address stored flash idx

salveMac_t tbl_slaveMac;  //slave mac bond table




int tbl_slave_mac_add(u8 *adr)  //add new mac address to table
{
	u8 add_new = 0;
	if(tbl_slaveMac.curNum >= PAIR_SLAVE_MAX_NUM){ //salve mac table is full
		//slave mac max, telink use  method 1: overwrite the oldest one
		tbl_slave_mac_delete_by_index(0);  //delete index 0 (oldest) of table
		add_new = 1;  //add new
	}
	else{//slave mac table not full
		add_new = 1;
	}

	if(add_new){
		memcpy(tbl_slaveMac.bond_device[tbl_slaveMac.curNum].address, adr, 6);

		u8 add_mark = ADR_BOND_MARK;

		bond_slave_flash_cfg_idx += 8;  //inc flash idx to get the new 8 bytes area
		flash_write_page (flash_adr_pairing + bond_slave_flash_cfg_idx + 0, 1, &add_mark);
		flash_write_page (flash_adr_pairing + bond_slave_flash_cfg_idx + 2, 6, adr);

		tbl_slaveMac.bond_flash_idx[tbl_slaveMac.curNum] = bond_slave_flash_cfg_idx;  //mark flash idx
		tbl_slaveMac.curNum++;

		return 1;  //add OK
	}

	return 0;
}

/* search mac address in the bond slave mac table:
   when slave paired with dongle, add this addr to table
   re_poweron slave, dongle will search if this AdvA in slave adv pkt is in this table
   if in, it will connect slave directly
   this function must in ramcode
 */
_attribute_ram_code_ int tbl_slave_mac_search(u8 * adr)
{
	for(int i=0; i< tbl_slaveMac.curNum; i++){
		if(!memcmp(tbl_slaveMac.bond_device[i].address ,adr, 6)){  //match
			return (i+1);  //return index+1( 1 - PAIR_SLAVE_MAX_NUM)
		}
	}

	return 0;
}

//when rc trigger unpair, use this function to delete the slave mac
int tbl_slave_mac_delete_by_adr(u8 *adr)  //remove adr from slave mac table by adr
{
	for(int i=0;i<tbl_slaveMac.curNum;i++){
		if(!memcmp(tbl_slaveMac.bond_device[i].address ,adr, 6)){  //match
			//erase the match adr
			u8 delete_mark = ADR_ERASE_MARK;
			flash_write_page (flash_adr_pairing + tbl_slaveMac.bond_flash_idx[i], 1, &delete_mark);

			for(int j=i; j< tbl_slaveMac.curNum - 1;j++){ //move data
				tbl_slaveMac.bond_flash_idx[j] = tbl_slaveMac.bond_flash_idx[j+1];
				memcpy(tbl_slaveMac.bond_device[j].address, tbl_slaveMac.bond_device[j+1].address, 6);
			}

			tbl_slaveMac.curNum --;
			return 1; //delete OK
		}
	}

	return 0;
}

void tbl_slave_mac_delete_by_index(int index)  //remove the oldest adr in slave mac table
{
	//erase the oldest with ERASE_MARK
	u8 delete_mark = ADR_ERASE_MARK;
	flash_write_page (flash_adr_pairing + tbl_slaveMac.bond_flash_idx[index], 1, &delete_mark);

	for(int i=index; i<tbl_slaveMac.curNum - 1; i++){ 	//move data
		tbl_slaveMac.bond_flash_idx[i] = tbl_slaveMac.bond_flash_idx[i+1];
		memcpy(tbl_slaveMac.bond_device[i].address, tbl_slaveMac.bond_device[i+1].address, 6);
	}

	tbl_slaveMac.curNum --;
}


void tbl_slave_mac_delete_all(void)  //delete all the  adr in slave mac table
{
	u8 delete_mark = ADR_ERASE_MARK;
	for(int i=0; i< tbl_slaveMac.curNum; i++){
		flash_write_page (flash_adr_pairing + tbl_slaveMac.bond_flash_idx[i], 1, &delete_mark);
		memset(tbl_slaveMac.bond_device[i].address, 0, 6);
		//tbl_slaveMac.bond_flash_idx[i] = 0;  //do not  concern
	}

	tbl_slaveMac.curNum = 0;
}



/* unpair cmd proc
   when press unpair button on dongle or send unpair cmd from slave
   dongle will call this function to process current unpair cmd
 */
void tbl_salve_mac_unpair_proc(void)
{
	//telink will delete all adr when unpair happens, you can change to your own strategy
	//slaveMac_curConnect is for you to use
	tbl_slave_mac_delete_all();
}

u8 adbg_flash_clean;
#define DBG_FLASH_CLEAN   0
//when flash stored too many addr, it may exceed a sector max(4096), so we must clean this sector
// and rewrite the valid addr at the beginning of the sector(0x11000)
void	bond_slave_flash_clean (void)
{
#if	DBG_FLASH_CLEAN
	if (bond_slave_flash_cfg_idx < 8*8)  //debug, max 8 area, then clean flash
#else
	if (bond_slave_flash_cfg_idx < 1024)  //max 1024/8 = 128,rest available sector is big enough, no need clean
#endif
	{
		return;
	}

	adbg_flash_clean = 1;

	u8 *pf = (u8 *)flash_adr_pairing;

	flash_erase_sector (flash_adr_pairing);

	bond_slave_flash_cfg_idx = -8;  //init value for no bond slave mac

	//rewrite bond table at the beginning of 0x11000
	for(int i=0; i< tbl_slaveMac.curNum; i++){
		u8 add_mark = ADR_BOND_MARK;

		bond_slave_flash_cfg_idx += 8;  //inc flash idx to get the new 8 bytes area
		flash_write_page (flash_adr_pairing + bond_slave_flash_cfg_idx + 0, 1, &add_mark);
		flash_write_page (flash_adr_pairing + bond_slave_flash_cfg_idx + 2, 6, tbl_slaveMac.bond_device[i].address);

		tbl_slaveMac.bond_flash_idx[i] = bond_slave_flash_cfg_idx;  //update flash idx
	}
}

void	bond_slave_flash_config_init(void)
{
	u8 *pf = (u8 *)flash_adr_pairing;
	for (bond_slave_flash_cfg_idx=0; bond_slave_flash_cfg_idx<4096; bond_slave_flash_cfg_idx+=8)
	{ //traversing 8 bytes area in sector 0x11000 to find all the valid slave mac adr
		if( pf[bond_slave_flash_cfg_idx] == ADR_BOND_MARK ){  //valid adr
			if(tbl_slaveMac.curNum < PAIR_SLAVE_MAX_NUM){
				tbl_slaveMac.bond_flash_idx[tbl_slaveMac.curNum] = bond_slave_flash_cfg_idx;
				flash_read_page (flash_adr_pairing + bond_slave_flash_cfg_idx + 2, 6, tbl_slaveMac.bond_device[tbl_slaveMac.curNum].address);
				tbl_slaveMac.curNum ++;
			}
			else{ //slave mac in flash more than max, we think it's code bug
				write_reg32(0x9000,0x12345678);  //for debug
				while(1);
			}
		}
		else if (pf[bond_slave_flash_cfg_idx] == 0xff)	//end
		{
			break;
		}
	}

	bond_slave_flash_cfg_idx -= 8; //back to the newest addr 8 bytes area flash ixd(if no valid addr, will be -8)

	bond_slave_flash_clean ();
}


/* set what advData trigger paring:
   when you press combination key at RC to pair, change your adv pkt format
   add set that format could directly trigger paring
   example:  8267 kt remote/ble_remote.c
   u8	tbl_pair_adv [] =
		{0x00, 14,  //27
		 0xef, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5,
		 0x07, 0x09, 'K', 'T','P','A','I','R',  //from 0x07 is advData in adv pkt
		};

   blt_set_adv_pkt (tbl_pair_adv);  //change normal adv pkt to this specil adv pkt for paring request
   when you paired OM or timeout, you must change adv pkyt back to normal (blt_set_adv_pkt (tbl_adv); )
   //0x07, 0x09 'K','T','P','A','I','R'  -> 07 09 4B 54 50 41 49 52

   this function must in ramcode
 */

typedef struct{
	u32 dma_len;
	u8	type;
	u8  rf_len;

	u8	advA[6];
	u8	data[31];
}ble_packet_adv_t;
_attribute_ram_code_ int adv_data_trigger_pair(u8 *raw_pkt)  //advData
{
	ble_packet_adv_t * p = (ble_packet_adv_t *) raw_pkt;

	//notice that p->data[0] is 4 bytes aigned in ram, so change it to u32 * pointer is OK
	//if not 4 bytes aligned, must use memcmp
	if( (*(u32 *)(&p->data[0]) == 0x544B0907) && (*(u32 *)(&p->data[4]) == 0x52494150) ){
		ble_master_pairing_enable(1);
	}

	return 0;
}

void master_bond_slave_mac_init(void)
{
	bond_slave_flash_config_init();
}





////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
#if VIN_TEST_MODE
extern u32				userdefine_timeout;
void userdefine_conn_timeout_disaptch()
{
	int i;
	u16 timeout_tmp=0;
	for(i=0;i<500;i++){
		flash_read_page(USER_DEFINE_TIMEOUT_ADR+2*i,2,(u8 *)(&timeout_tmp));
		if(timeout_tmp ==0xffff){
			break;
		}else{
			userdefine_timeout = timeout_tmp&0x0000ffff;
		}
	}
	if(i==500){
		flash_erase_sector(USER_DEFINE_TIMEOUT_ADR);
		flash_write_page(USER_DEFINE_TIMEOUT_ADR,2,(u8 *)(&userdefine_timeout));//save the last data 
	}
	if(userdefine_timeout<100||userdefine_timeout>400){
		userdefine_timeout = 400;
	}
	userdefine_timeout = userdefine_timeout*10*1000;
}


void auto_voice_timeout_dispatch()
{
	int i;
	u8 timeout_tmp;
	for(i=0;i<1000;i++){
		flash_read_page(AUTO_VOICE_TIMEOUT_ADR+i,1,&timeout_tmp);
		if(timeout_tmp == 0xff){
			break;
		}else{
			voice_timeout =timeout_tmp;
		}
	}
	if(i==1000){
		flash_erase_sector(AUTO_VOICE_TIMEOUT_ADR);
		flash_write_page(AUTO_VOICE_TIMEOUT_ADR,1,&voice_timeout);//save the last data 
	}
	if(voice_timeout<2){
		voice_timeout =30;
	}
	change_voice[14]=voice_timeout;
}
#endif 


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

void  user_init(void)
{
#if VIN_TEST_MODE
	userdefine_conn_timeout_disaptch();
	auto_voice_timeout_dispatch();
	extern u8 change_voice[18];
	change_voice[11]= VALUE_REPORT_DONGLE_TIMEOUT;
	change_voice[13]= CMD_SEND_TIMEOUT;
	
#endif 

	rf_customized_param_load();

	usb_log_init ();
	/////////// ID initialization for light control software //////////
	//set UAB ID
	REG_ADDR8(0x74) = 0x53;
	REG_ADDR16(0x7e) = 0x08ee;
	REG_ADDR8(0x74) = 0x00;

	//set kma dongle MAC address:  rnadom value in first poweron
	u8 master_mac_adr[6];
	if (*(u32 *) CFG_ADR_MAC == 0xffffffff){
		u16 * ps = (u16 *) master_mac_adr;
		ps[0] = REG_ADDR16(0x448);  //random
		ps[1] = REG_ADDR16(0x448);
		ps[2] = REG_ADDR16(0x448);
		flash_write_page (CFG_ADR_MAC, 6, master_mac_adr);  //store master address
	}
	else{
		memcpy (master_mac_adr, (u8 *) CFG_ADR_MAC, 6);  //copy from flash
	}
	ble_master_init (master_mac_adr);

	master_bond_slave_mac_init();  //load and mark  paired slave mac address


	/////////// enable USB device /////////////////////////////////////
	//////////////// config USB ISO IN/OUT interrupt /////////////////
	reg_usb_mask = BIT(7);			//audio in interrupt enable
	reg_irq_mask |= FLD_IRQ_IRQ4_EN;
	reg_usb_ep6_buf_addr = 0x80;
	reg_usb_ep7_buf_addr = 0x60;
	reg_usb_ep_max_size = (256 >> 3);

	usb_dp_pullup_en (1);  //DM pullup to tell host USB device is on, host start USB enum


	////////// set up wakeup source: driver pin of keyboard  //////////
#if DONGLE_TEST_ENABLE
	rf_set_power_level_index (RF_POWER_m28dBm); 	//testing dongle, reduce power
	ble_master_pairing_enable(1);//enable paring 
#else
	rf_set_power_level_index (RF_POWER_8dBm);
#endif 

#if VIN_TEST_MODE
    register_att_function (HID_HANDLE_CONSUME_REPORT_DONGLE, &att_keyboard_media);
    register_att_function (HID_HANDLE_KEYBOARD_REPORT_DONGLE, &att_keyboard);
    register_att_function (AUDIO_HANDLE_MIC_REPORT_DONGLE, &att_mic);
	register_att_function (VALUE_REPORT_DONGLE_TIMEOUT, &audio_control);
	
#else
    register_att_function (10, &att_mouse);
    register_att_function (21, &att_keyboard_media);
    register_att_function (25, &att_keyboard);
#endif 

#if VIN_TEST_MODE
#else
    register_att_function (43, &att_mic);
#endif

    //register_att_function(SIM_TOUCH_HANDLE,&att_touch_sim);

    // 1 init read rep register space
    for(int i=0; i<8 ;i++)  write_reg8(0x8000+i, 0);
    for(int i=0; i<8 ;i++)  write_reg8(0x8008+i, 0);
#if VIN_TEST_MODE
#else
    //battery
    register_att_function (HANDLE_BATTERY_VALUE, &att_read_rep_handler); // or use notify

    //Immediate Alert
    register_att_function (HANDLE_ALERT_LEVEL, &att_read_rep_handler);

    //Device Information Service
    register_att_function (HANDLE_SYS_ID, &att_read_rep_handler);
    register_att_function (HANDLE_MODEL_NO_STR, &att_read_rep_handler);
    register_att_function (HANDLE_SERIAL_NO_STR, &att_read_rep_handler);
    register_att_function (HANDLE_FW_VER_STR, &att_read_rep_handler);
    register_att_function (HANDLE_HW_VER_STR, &att_read_rep_handler);
    register_att_function (HANDLE_SW_VER_STR, &att_read_rep_handler);
    register_att_function (HANDLE_MANU_NAME_STR, &att_read_rep_handler);
    register_att_function (HANDLE_CERT_DATA_STR, &att_read_rep_handler);
    register_att_function (HANDLE_PNP_ID, &att_read_rep_handler);
#endif 
    /*Register read by type rsp handler function*/
    register_read_by_type_rsp_cb(att_read_by_type_rsp_handler);

#if(KMA_DONGLE_SECURITY_ENABLE)
    master_smp_func_init();
#endif

#if UART_ENABLE
    //Initial IO
    gpio_set_func(GPIO_UTX, AS_UART);
    gpio_set_func(GPIO_URX, AS_UART);

    CLK32M_UART115200;
    uart_BuffInit((u8 *)(&T_rxdata_buf), sizeof(T_rxdata_buf), (u8 *)(&T_txdata_buf));
#endif
}


void  att_read_by_type_rsp_handler (rf_packet_att_write_data_t *p){
    rf_packet_att_read_by_type_rsp_t*p_tmp = (rf_packet_att_read_by_type_rsp_t*)p;
    
    if(!(memcmp(read_by_type_req_uuid, my_OtaUUID,OTA_UUID_LEN))){
        remote_ota_handle =    p_tmp->attrHandle;
        memset(read_by_type_req_uuid, 0, 16);
        if(host_ota_start == 1){
            host_ota_start = 2;
        }
    }

}

#if(KMA_DONGLE_OTA_ENABLE)

#define  OTA_GPIO_DBG	1

typedef struct{
	u32 dma_len;            //won't be a fixed number as previous, should adjust with the mouse package number

	u8	type;				//RFU(3)_MD(1)_SN(1)_NESN(1)-LLID(2)
	u8  rf_len;				//LEN(5)_RFU(3)
	u16	l2cap;
	u16	chanid;

	u8	att;
	u8	hl;					// assigned by master
	u8	hh;					//

	u8	dat[23];

}rf_packet_att_data_t;


u8 write_ota_cmd[] = {
	0x0b,0,0,0,	//dma_len
	0x02,   	//type
	0x09,		//rf_len

	0x05,0x00,  //l2caplen

	0x04,0x00,  //chanid

	0x52, //opcode = ATT_OP_WRITE_CMD

	REMOTE_OTA_DATA_HANDLE, 0x00, //handle

	0x00,0x00,  //13  14
};

#define CMD_OTA_FW_VERSION					0xff00
#define CMD_OTA_START						0xff01
#define CMD_OTA_END							0xff02
#define CMD_OTA_START_REQ                                    0xff03 //Request OTA start, reserved for BLE Fullstack
#define CMD_OTA_START_RSP                                    0xff04 //Slave allow OTA start, reserved for BLE Fullstack


rf_packet_att_data_t	ota_buffer[8];

u32 flash_adr_ota_master = 0x20000;
u8 *p_firmware;
u32 n_firmware = 0;
u32 ota_adr = 0;


void ota_set_result(int status)
{
	host_ota_start = 0;
	pc_tool_send_ota_cmd = 0;
	gpio_write(GPIO_LED_WHITE,status);  //ota fail

#if (WEIGHT_SCALE_OTA)
	gpio_write(GPIO_LED_BLUE,!LED_ON_LEVAL);
#endif

	write_reg8(reg_cmd_result,status);  //1 OK  0 fail
	SET_CMD_DONE;
}

/*Set OTA process timeout*/
u8 ota_timeout_s = 0;
u8 adr_index_max_l;
u8 adr_index_max_h;

void proc_ota ()
{
	static u32	tick_page;

	//add data when master is not in BTX state
	extern u32	ble_master_link_tick;
	extern int  push_tx_fifo_enable;
	extern void blt_send_read_by_type_req(u8* uuid, u8 len);
	if(!clock_time_exceed(ble_master_link_tick,800)  || !push_tx_fifo_enable){
		return;
	}

	if(host_ota_start == 0)
	{
		gpio_write(GPIO_LED_BLUE,!LED_ON_LEVAL);

		if(pc_tool_send_ota_cmd){  //UI: pc tool trig OTA mode
		    gpio_write(GPIO_LED_BLUE,LED_ON_LEVAL);  //ota begin
		    host_ota_start = 1;

		    /*Send read_by_type_req to get remote's OTA handle
		     * once receive remote's response, call back function att_read_by_type_rsp_handler
		     * will be called to get the handle and set host_ota_start to 2*/
		    memset(read_by_type_req_uuid, 0, 16);
		    memcpy(read_by_type_req_uuid,my_OtaUUID,OTA_UUID_LEN);
		    blt_send_read_by_type_req((u8*)read_by_type_req_uuid,OTA_UUID_LEN); 
		}
	}
	else if (host_ota_start == 2)
	{
		n_firmware = *(u32 *)(flash_adr_ota_master+0x18);
		if(n_firmware > (124<<10)){  //bigger then 124K or 0xffffffff  is ERR
			host_ota_start = 0;
		}

#if (WEIGHT_SCALE_OTA)
		ota_timeout_s = 30;   //weight project, need long time to finish OTA
#else
		ota_timeout_s = 5+5*(n_firmware>>14);  //32k, timeout 10+5s; 64k, timeout 20+5s
#endif

		p_firmware = (u8 *)flash_adr_ota_master;
		host_ota_start = 3;
		ota_adr = 0;

		write_ota_cmd[11] = remote_ota_handle;

		//reg_dma_tx_rptr = FLD_DMA_RPTR_CLR;  //clear fifo
		write_ota_cmd[14] = CMD_OTA_START>>8;  //send ota start
		write_ota_cmd[13] = CMD_OTA_START&0xff;
		reg_dma_tx_fifo = (u16) write_ota_cmd;

		tick_page = clock_time ();
	}
	else if (host_ota_start == 3)
	{
		if(clock_time_exceed(tick_page, ota_timeout_s*1000*1000)){  //OTA fail
			ota_set_result(0);
			return;
		}
		int idx = (ota_adr >> 4) & 7;
		rf_packet_att_data_t *p = &ota_buffer[idx];

		int nlen = ota_adr < n_firmware ? 16 : 0;

		p->type = 2;
		p->l2cap = 7 + nlen;
		p->chanid = 0x04;
		p->att = ATT_OP_WRITE_CMD;
		p->hl = remote_ota_handle; //0x31 for 8267 remote, 0xa1 for 8266 fullstack
		p->hh = 0x0;
		p->dat[0] = ota_adr>>4;
		p->dat[1] = ota_adr>>12;
		if(nlen == 16){
			memcpy(p->dat + 2, p_firmware + ota_adr, 16);
		}
		else{  //OTA data finish, send OTA end
#if 0
			p->l2cap =5;
			p->dat[0] = CMD_OTA_END&0xff;  //ota end cmd
			p->dat[1] = CMD_OTA_END>>8;
			memset(p->dat + 2, 0, 16);
#else
			p->l2cap = 9;
			p->dat[0] = CMD_OTA_END&0xff;  //ota end cmd
			p->dat[1] = CMD_OTA_END>>8;

			adr_index_max_l = (ota_adr-16)>>4;
			adr_index_max_h = (ota_adr-16)>>12;
			p->dat[2] = adr_index_max_l;    //adr_index_max for slave to check if any packet miss
			p->dat[3] = adr_index_max_h;
			p->dat[4] = ~adr_index_max_l;  //adr_index_max check
			p->dat[5] = ~adr_index_max_h;

			memset(p->dat + 6, 0, 12);
#endif
		}

		if(nlen == 16){  //OTA end cmd do not need CRC
			u16 crc = crc16(p->dat, 2+nlen);
			p->dat[nlen + 2] = crc;
			p->dat[nlen + 3] = crc >> 8;
		}



		p->rf_len = p->l2cap + 4;
		p->dma_len = p->l2cap + 6;

		if (ble_master_add_tx_packet ((u32)p))
		{
			ota_adr += 16;
			if (nlen == 0)  //all data push fifo OK
			{
				host_ota_start = 4;
			}
		}
	}
	else if(host_ota_start == 4){
		if(master_hw_fifo_data_num() == 0 ){ //all data acked,OTA OK
			ota_set_result(1);
		}
	}
}

#endif

u32 att_dma = 0;
rf_packet_att_write_data_t att;
att_func_t read_by_type_rsp_cb = NULL; 
void register_read_by_type_rsp_cb(att_func_t cb_func){
    read_by_type_rsp_cb = cb_func;
}

_attribute_ram_code_ void	ble_master_data_callback (u8 *ps)
{

    rf_packet_att_write_data_t *p = (rf_packet_att_write_data_t *)(ps + 4);
    memcpy(&att, p, sizeof(rf_packet_att_write_data_t));

       /*Process notify data*/
	if (p->chanId == 0x04 && (p->opcode == 0x1b) && p->handle < MAX_ATT_FUNC && att_func[p->handle]) {
		att_func[p->handle](p);
	}
	else if(p->chanId == 0x04  && p->opcode == 0x0b){      /*Process read response data*/
	    if ((current_red_req_handle <= MAX_ATT_FUNC) && att_func[current_red_req_handle])
	    {
		    att_func[current_red_req_handle](p); // rf_packet_att_read_rsp_t actually
	    }
    }
	else if(p->chanId == 0x04  && p->opcode == 0x09){      /*Process read by type response*/
	    if (read_by_type_rsp_cb){
	        read_by_type_rsp_cb(p);
	    }
    }          /*Process conn para update command*/
	else if(p->chanId == 0x05  && p->opcode == 0x12){  //CONNECTION PARAMETER UPDATE REQUEST
		rf_packet_conn_para_update_req_t  * req = (rf_packet_conn_para_update_req_t *)(ps+4);
		blt_conn_para_update (pkt_master_init.wsize, pkt_master_init.woffset, req->interval_min,req->latency,req->timeout,10);
	}
}

