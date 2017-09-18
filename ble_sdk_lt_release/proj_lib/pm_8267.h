
#pragma once

#if(__TL_LIB_8267__ || MCU_CORE_TYPE == MCU_CORE_8267)

typedef void (*pm_optimize_handler_t)(void);

static inline void usb_dp_pullup_en (int en)
{
	unsigned char dat = ReadAnalogReg(0x08);
	if (en) {
		dat = (dat & 0x3f) | BIT(7);
	}
	else {
		dat = (dat & 0x3f) | BIT(6);
	}

	WriteAnalogReg (0x08, dat);
}

enum {
	 PM_WAKEUP_PAD   = BIT(4),
	 PM_WAKEUP_CORE  = BIT(5),
	 PM_WAKEUP_TIMER = BIT(6),
	 PM_WAKEUP_COMP  = BIT(7),
};


///////////////////////////////////////////////////////////////////////
////////////////////////////battery dectect////////////////////////////
////////////////////////////////////////////////////////////////////////
/*test data
 * standard      test
 * 0.98v <--->1.022v ~ 1.024v
 * 1.1v  <--->1.144v ~ 1.150v
 * 1.18v <--->1.214v ~ 1.218v
 * 1.25v <--->1.285v ~ 1.289v
 * 1.3v  <--->1.355v ~ 1.358v
 * 1.6v  <--->1.701v ~ 1.708v
 * */
enum  COMP_CHANNALE {
	COMP_ANA3 = 0x00,	COMP_GPIO_GP11 = 0x00,
	COMP_ANA4 = 0x02,	COMP_GPIO_GP12 = 0x02,
	COMP_ANA5 = 0x04,	COMP_GPIO_SWS = 0x04,
	COMP_ANA6 = 0x06,	COMP_GPIO_CN = 0x06,
	COMP_ANA7 = 0x08,	COMP_GPIO_CK = 0x08,
	COMP_ANA8 = 0x0a,	COMP_GPIO_DO = 0x0a,
	COMP_ANA9 = 0x0c,	COMP_GPIO_DI = 0x0c,
	COMP_ANA10 = 0x0e, 	COMP_GPIO_MSCN = 0x0e,
	COMP_ANA11 = 0x10,	COMP_GPIO_MCLK = 0x10,
	COMP_ANA12 = 0x12,	COMP_GPIO_MSDO = 0x12,
	COMP_ANA13 = 0x14,	COMP_GPIO_MSDI = 0x14,
	COMP_ANA14 = 0x16,	COMP_GPIO_DMIC_CK = 0x16,	COMP_GPIO_I2S_REFCLK = 0x16,
	COMP_ANA15 = 0x18,	COMP_GPIO_I2S_BCK = 0x18,
	COMP_AVDD =  0x1a
};

enum{
	V0P98,
	V1P1,
	V1P18,
	V1P25,
	V1P3,
	V1P66,
};

#define SCALING_SELECT_QUARTER 		0x00//25%
#define SCALING_SELECT_HALF 		0x20//50%
#define SCALING_SELECT_3QUARTER 	0x40//75%
#define SCALING_SELECT_FULL 		0x60//100%

#define REF_VOLTAGE_SEL_0			0x00//float
#define REF_VOLTAGE_SEL_1			0x01//981mv
#define REF_VOLTAGE_SEL_2			0x02//937mv
#define REF_VOLTAGE_SEL_3			0x03//885mv
#define REF_VOLTAGE_SEL_4			0x04//832mv
#define REF_VOLTAGE_SEL_5			0x05//ana3
#define REF_VOLTAGE_SEL_6			0x06//ain9
#define REF_VOLTAGE_SEL_7			0x07//avddh

#ifndef		VBAT_LOW_LEVLE
#define		VBAT_LOW_LEVLE		V0P98
#endif

#ifndef		VBAT_CHANNEL
#if BATTERY_DETECTION_WITH_LDO_SET
#define		VBAT_CHANNEL		COMP_AVDD
#else
#define		VBAT_CHANNEL		COMP_ANA8
#endif
#endif

#define		V0P98_REF			REF_VOLTAGE_SEL_1
#define		V0P98_SCALE			SCALING_SELECT_FULL

#define		V1P1_REF			REF_VOLTAGE_SEL_4
#define		V1P1_SCALE			SCALING_SELECT_3QUARTER

#define		V1P18_REF			REF_VOLTAGE_SEL_3
#define		V1P18_SCALE			SCALING_SELECT_3QUARTER

#define		V1P25_REF			REF_VOLTAGE_SEL_2
#define		V1P25_SCALE			SCALING_SELECT_3QUARTER

#define		V1P3_REF			REF_VOLTAGE_SEL_1
#define		V1P3_SCALE			SCALING_SELECT_3QUARTER

#define		V1P66_REF			REF_VOLTAGE_SEL_4
#define		V1P66_SCALE			SCALING_SELECT_HALF

#define		VBAT_LOW_SCALE		(VBAT_LOW_LEVLE==V0P98 ? V0P98_SCALE  : V1P1_SCALE )
#define		VBAT_LOW_REF		(VBAT_LOW_LEVLE==V0P98 ? V0P98_REF : V1P1_REF)

int battery_detection_with_ldo (u8 chn, int set);
int battery_direct_detection (u8 chn, int set);
int battery_low_by_set ( u8 chn, u8 v_ref, u8 v_scale ) ;
int battery_low ();
void battery_by_comp_init();



#define SUSPEND_MODE	0
#define DEEPSLEEP_MODE	1

#define WAKEUP_LEVEL_L 	0
#define WAKEUP_LEVEL_H 	1


//8267 analog register 0x34-0x3e can store infomation when MCU in deepsleep mode
//store your information in these ana_regs before deepsleep by calling analog_write function
//when MCU wakeup from deepsleep, read the information by by calling analog_read function

//these five below are stable
#define DEEP_ANA_REG0    0x3a
#define DEEP_ANA_REG1    0x3b
#define DEEP_ANA_REG2    0x3c
#define DEEP_ANA_REG3    0x3d
#define DEEP_ANA_REG4  	 0x3e

//these six below may have some problem when user enter deepsleep but ERR wakeup
// for example, when set a GPIO PAD high wakeup deepsleep, but this gpio is high before
// you call func cpu_sleep_wakeup, then deepsleep will be ERR wakeup, these 6 register
//   infomation loss.
#define DEEP_ANA_REG5    0x34
#define DEEP_ANA_REG6    0x35
#define DEEP_ANA_REG7    0x36
#define DEEP_ANA_REG8    0x37
#define DEEP_ANA_REG9    0x38
#define DEEP_ANA_REG10   0x39


#define ADV_DEEP_FLG	 0x01
#define CONN_DEEP_FLG	 0x02

// usually, we don't use gpio wakeup in suspend mode.
// If you do need it,  pls turn on this micro, add set  wakeup pin before calling cpu_sleep_wakeup

// like:  
// reg_gpio_f_wakeup_en = SUSPEND_WAKEUP_SRC_PWM0;
// reg_gpio_f_pol = SUSPEND_WAKEUP_SRC_PWM0;
// cpu_sleep_wakeup(1, 50, 0, 0)
#define PM_SUSPEND_WAKEUP_BY_GPIO_ENABLE		0

void pm_init(void);

static inline int cpu_get_32k_tick (void) {
	return 1;
}

//deepsleep mode must use this function for resume 1.8V analog register
void cpu_wakeup_init(void);
void cpu_set_gpio_wakeup (int pin, int pol, int en);
int cpu_sleep_wakeup (int deepsleep, int wakeup_src, u32 wakeup_tick);


extern const u16 wakeup_src_pin[];


#endif
