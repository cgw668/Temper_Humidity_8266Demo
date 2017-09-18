//#include "../../proj/drivers/bsp.h"
#include "../../proj/drivers/adc.h"

extern int adc_enable;
//-------------------Clock_init---------------------

struct  S_SYS_RST0_BITS {
   unsigned char  spi:1;
   unsigned char  i2c:1;
   unsigned char  usb:1;
   unsigned char  usb_phy:1;
   unsigned char  mcu:1;
   unsigned char  mac:1;
   unsigned char  aif:1;
   unsigned char  zb:1;
};
union   U_RST0 {
  struct  S_SYS_RST0_BITS  bits;
  unsigned  char   all;
};

struct  S_SYS_RST1_BITS {
	unsigned char  system_timer:1;
	unsigned char   algm:1;
	unsigned char   dma:1;
	unsigned char   rs232:1;
	unsigned char   pwm0:1;
	unsigned char   aes:1;
	unsigned char   bbpll48m:1;
	unsigned char   swires:1;
};
union  U_RST1 {
   struct   S_SYS_RST1_BITS  bits;
	unsigned char  all;
};
struct  S_SYS_RST2_BITS {
	unsigned char   sbc:1;
	unsigned char   audio:1;
	unsigned char   dfifo:1;
	unsigned char   adc:1;
	unsigned char   mcic:1;
	unsigned char   reset_mcic_enable:1;
	unsigned char   mspi:1;
	unsigned char   algs:1;
};

union  U_RST2 {
   struct   S_SYS_RST2_BITS  bits;
	unsigned char  all;
};

union  U_CLK0 {
	struct   S_SYS_RST0_BITS  bits;
	unsigned char  all;
};

struct  S_SYS_CLK1_BITS {
	unsigned char  system_timer:1;
	unsigned char   algm:1;
	unsigned char   dma:1;
	unsigned char   rs232:1;
	unsigned char   pwm0:1;
	unsigned char   aes:1;
	unsigned char   clk_32k_system_timer:1;
	unsigned char   swires:1;
};
union  U_CLK1 {
   struct   S_SYS_CLK1_BITS  bits;
	unsigned char  all;
};

struct  S_SYS_CLK2_BITS {
	unsigned char  clk32k_qdec:1;
    unsigned char  audio:1;
    unsigned char  dfifo:1;
    unsigned char  key_scan:1;
	unsigned char  mcic:1;
	unsigned char  qdec:1;
	unsigned char  pwm32k:1;
	unsigned char  keyscan32K:1;

};
union  U_CLK2 {
    struct  S_SYS_CLK2_BITS  bits;
	unsigned char  all;
};


enum  M_SYSCLK_SEL {
  SYS_CLK_RC = 0,
  SYS_CLK_HS_DIV = 1,
 // SYS_CLK_PAD_16M = 2,
 // SYS_CLK_RC_32K = 3
};
struct  S_CLK_CTL {
    unsigned short   clk_div:5;
    unsigned short   sysclk_sel:2;
	unsigned short   hs_sel:2;
};
union  U_CLK_CTL {
    struct  S_CLK_CTL   bits;
	unsigned short      all;

};

struct  S_SYS_CTL {
	union U_RST0   rst0;
	union U_RST1   rst1;
	union U_RST2   rst2;
    union U_CLK0   clk0;
	union U_CLK1   clk1;
	union U_CLK2   clk2;
	union U_CLK_CTL  clkctl;

};
//-------------------------------------------------------------

#define BIT(n)                  		( 1<<(n) )

#define BIT0                            0x1
#define BIT1                            0x2
#define BIT2                            0x4
#define BIT3                            0x8
#define BIT4                            0x10
#define BIT5                            0x20
#define BIT6                            0x40
#define BIT7                            0x80
#define BIT8                            0x100

unsigned long	tick_per_us;

unsigned char loop = 0;
unsigned char Timer0_irq = 0;

unsigned short BatteryValue[128];
unsigned char BatteryTick = 0;

unsigned short Sensor1Value[128];
unsigned char Sensor1Tick = 0;

unsigned short Sensor2Value[128];
unsigned char Sensor2Tick = 0;


#if 0
void irq_handler(void )
{
    if (read_reg8(0x800648) & 0x01) {                        //if timer0's irq occurs
        //if Timer0 assert irq
        if (read_reg8(0x800623) & 0x01) {
            write_reg8(0x800623,0x01);                 //clear Timer0's status
            Timer0_irq = 1;
        }
    }
}
#endif

//-------------------Clock_init---------------------
void SetTickUs (unsigned int t)
{
	tick_per_us = t;
}


void Sys_Init(struct  S_SYS_CTL  * p_sys_ctl,enum M_SYSCLK_SEL clock_src,unsigned char clock_divider)
{

	if(p_sys_ctl!=NULL)
	{
		write_reg8(0x800060,p_sys_ctl->rst0.all);
		write_reg8(0x800061,p_sys_ctl->rst1.all);
		write_reg8(0x800062,p_sys_ctl->rst2.all);
		write_reg8(0x800063,p_sys_ctl->clk0.all);
		write_reg8(0x800064,p_sys_ctl->clk1.all);
		write_reg8(0x800065,p_sys_ctl->clk2.all);
	}
	//swires
	write_reg8 (0x800061, read_reg8(0x800061)&(~BIT7));
	write_reg8 (0x800064, read_reg8(0x800064)|BIT7);
	//algm
	write_reg8 (0x800061, read_reg8(0x800061)&(~BIT1));
	write_reg8 (0x800064, read_reg8(0x800064)|BIT1);
	//usb
	write_reg8 (0x800060, read_reg8(0x800060)&(~BIT2));
	write_reg8 (0x800063, read_reg8(0x800063)| BIT2 |BIT3);
	WriteAnalogReg (0x88, 0x0f);
	WriteAnalogReg (0x05, 0x60);
	//system timer
	write_reg8 (0x800061, read_reg8(0x800061)&(~BIT0));
	write_reg8 (0x800064, read_reg8(0x800064)| BIT0);


	write_reg8(0x800073, read_reg8(0x800073)&0x0f);//disable low power mode
	if(SYS_CLK_HS_DIV == clock_src)
	{
		//set FHS select and divider
		write_reg8(0x800070,0x00);//FHS选择，和remington不同
		write_reg8(0x800066,(0x20 | clock_divider));
		SetTickUs(192/clock_divider);
	}
	else if(SYS_CLK_RC == clock_src)
	{
		write_reg8(0x800066,0x00);
		SetTickUs(32);
	}
	write_reg8(0x80074f,0x01);//sys timer beginning to work
}



void adc_ChannelInit(enum ADCINPUTCH channel,
	                enum ADCINPUTMODE mode,
	                enum ADCRFV ref,
	                enum ADCRESOLUTION res,
	                enum ADCST time)
{
	/***1.set adc mode and input***/
	adc_AnaChSet(channel);
	adc_AnaModeSet(mode);

	/***2.set adc reference voltage***/
	adc_RefVoltageSet(ref);     //Set reference voltage (V_REF)as  1.428V

	/***3.set adc resultion***/
	adc_ResSet(res);               //Set adc resolution to 14 bits, bit[14] to bit bit[1]

	/***4.set adc sample time***/
	adc_SampleTimeSet(time);          //set sample time

	/***5.enable manual mode***/
	write_reg8(0x800033,0x00);
}

unsigned short sv;
void test_adc(void )
{
	//struct  S_SYS_CTL  sys_ctl;

    //sys_ctl.rst0.all = 0x00;
    //sys_ctl.rst1.all = 0x00;
    //sys_ctl.rst2.all = 0x00;
    //sys_ctl.clk0.all = 0xff;
    //sys_ctl.clk1.all = 0xff;
    //sys_ctl.clk2.all = 0xff;
    //Sys_Init(&sys_ctl, SYS_CLK_HS_DIV, 6);

    /***Initialize ADC Module***/
    adc_Init();

    // timer can affect UART!!
    /***enable Timer0's periodic Interrupt(0.166ms, so every channel's sample frequency is about 2KHz)***/
    //write_reg32(0x800630, 0); //set initial tick value of Timer0
    //write_reg32(0x800624, tick_per_us*166); //set capture value of Timer0(0.166 ms)
    //write_reg8(0x800620, read_reg8(0x800620)&0xf9); //set Timer0 to Mode0
    //write_reg8(0x800620, read_reg8(0x800620)|0x01); //enable Timer0
    //write_reg8(0x800640, read_reg8(0x800640)|0x01); //enable Timer0's IRQ
    //write_reg8(0x800643, read_reg8(0x800643)|0x01); //enable IRQ

    //write_reg8(0x80058e,read_reg8(0x80058e)|0x0C); //enable GPIO mode - UART_TX/PWM3/ANA_B<2>, UART_RX/PWM3_N/ANA_B<3>


	//while (1) {
    for(int i=0;i<(128*3);i++){
	    //if (Timer0_irq == 1) {
	        //Timer0_irq = 0;
	        //write_reg32(0x800630, 0); //set initial tick value of Timer0
		    //write_reg32(0x800624, tick_per_us*166); //set capture value of Timer0(0.166 ms)
		    //write_reg8(0x800620, read_reg8(0x800620)&0xf9); //set Timer0 to Mode0
		    //write_reg8(0x800620, read_reg8(0x800620)|0x01); //enable Timer0
            WaitMs(20);
	        if (loop == 0) {                        //do Battery voltage detect
                adc_BatteryCheckInit(1);
                WaitUs(30);
                BatteryValue[BatteryTick++] = adc_BatteryValueGet();
	        }
	        else if (loop == 1) {                   //do sensor1 voltage detect
                adc_ChannelInit(B1, SINGLEEND, RV_1P428, RES14, S_3);
                WaitUs(30);
                Sensor1Value[Sensor1Tick++] = adc_SampleValueGet() & 0x3FFF;
	        }
	        else if (loop == 2) {                   //do sensor2 voltage detect
	            adc_ChannelInit(B4, SINGLEEND, RV_1P428, RES14, S_3);
                WaitUs(30);
                Sensor2Value[Sensor2Tick++] = adc_SampleValueGet() & 0x3FFF;
	        }
	        loop = (loop+1) % 3;
	    //}
	}
    adc_enable =FALSE;// turn off
}

