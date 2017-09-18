#if(__PROJECT_DRIVER_TEST__)

#include "../../proj/tl_common.h"
#include "../../proj_lib/pm.h"

#include "../../proj/drivers/uart.h"
#include "../../proj/drivers/adc.h"
#include "../../proj/mcu/pwm.h"

extern unsigned char Timer0_irq;
int uart_enable =0;
int adc_enable =0;

enum {
    TEST_NULL = 		0x00,

    TEST_ADC ,          //1
    TEST_I2C_WRITE,
    TEST_SPI_MASTER_W,  //3 check if support another spi master
    TEST_SUSPEND,
    TEST_DEEP_SLEEP,    //5
    TEST_PWM,
    TEST_UART,          //7

    TEST_INVALID_CMD =  0xF0,
    TEST_CMD_DONE =     0xFF,


    TEST_MAX,
};

#define CMD_DONE   write_reg8(0x8000,TEST_CMD_DONE)
#define CMD_INIT   write_reg8(0x8000,TEST_NULL)


void uart_init(void);


void user_init()
{
	usb_dp_pullup_en (1);
	//usb_log_init ();
    //uart_init();
    //test_adc_init();
    CMD_INIT;
}


void test_suspend()
{
    cpu_sleep_wakeup (0, PM_WAKEUP_TIMER, clock_time() + 3*CLOCK_SYS_CLOCK_1S);
}
void test_deep_sleep()
{
    cpu_sleep_wakeup (1, PM_WAKEUP_TIMER, clock_time() + 3*CLOCK_SYS_CLOCK_1S);
}

void test_pwm()
{
	PWM0_CFG_GPIO_A0();
    pwm_Init(15);  //16M/16 = 1M
    pwm_Open(PWM0, NORMAL, 1, 5, 10, 0);
    pwm_Start(PWM0);
}

u8 cmd2I2C[10] = {0};
int host_i2c_write (u8 *p, int n)
{
	static u32 cmd_i2c_no;

    cmd_i2c_no++;
	for (int i=0; i<n ; i++) {
		reg_i2c_do = p[i];
		reg_i2c_ctrl = (i==0 ? FLD_I2C_CMD_START | FLD_I2C_CMD_ID : 0) |
						FLD_I2C_CMD_DO |
						(i == n-1 ? FLD_I2C_CMD_STOP : 0);
		while (reg_i2c_status & FLD_I2C_CMD_BUSY);
	}
	return n;
}

void test_i2c_write()
{
    int n=0;
    //init i2c gpios if necessary
    //...

    cmd2I2C[0] = 0x01;
	cmd2I2C[1] = 0xff;
	cmd2I2C[2] = 0x02;
	cmd2I2C[3] = 0x00;
	cmd2I2C[4] = 24;//15ms
	cmd2I2C[5] = 0x00;
	n = host_i2c_write(cmd2I2C, 6);
    printf("i2c write bytes %d\n",n);
}


void test_spi_master_write()
{
}

//------------------------ uart--------------------------------------------

unsigned char recBuff[128];
void uart_init(void)
{
	unsigned char i;

	CLK16M_UART9600;
	uart_RecBuffInit((unsigned char *)&recBuff,128);//set receive buffer address
    for(i=0;i<128;i++)  recBuff[i] = 0;
    //write_reg8(0x80058e,read_reg8(0x80058e)&~0x0C); //disable GPIO mode - UART_TX/PWM3/ANA_B<2>, UART_RX/PWM3_N/ANA_B<3>
	//write_reg8(0x8005b1,read_reg8(0x8005b1)|0x0C);  //enable UART
    UART_GPIO_CFG_PB2_PB3();

    //TODO : change uart pin not success yet
    //UART_GPIO_CFG_PA6_PA7();
    //UART_GPIO_CFG_PC2_PC3();

}
unsigned char txTest[128];
unsigned short rxL;
//unsigned long t1,t2;
unsigned char enterRXIrq,enterTXIrq;
unsigned char rx_true;
void uart_process(void)
{
	unsigned char i;

		if(rx_true){
			rxL = recBuff[0];
			txTest[0] = rxL;
			txTest[1] = 0;
			txTest[2] = 0;
			txTest[3] = 0;
			for(i = 0; i<rxL;i++){
				txTest[i+4] = recBuff[i+4]+1;
			}
			uart_Send(txTest);
			rx_true = 0;
		}
}


void uart_irq_handler_ex(void ) {
	unsigned char irqS;
	irqS = uart_IRQSourceGet();
	if(irqS == UARTRXIRQ){
		rx_true = 1;
		enterRXIrq++;
	}
	if(irqS == UARTTXIRQ){
		enterTXIrq++;
	}
}

//----------------------- adc --------------------------------------------
#if 0
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

void test_adc_init()
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

    /***enable Timer0's periodic Interrupt(0.166ms, so every channel's sample frequency is about 2KHz)***/
    write_reg32(0x800630, 0); //set initial tick value of Timer0
    //write_reg32(0x800624, tick_per_us*166); //set capture value of Timer0(0.166 ms)
    write_reg32(0x800624, CLOCK_SYS_CLOCK_1US*166); //set capture value of Timer0(0.166 ms)


    write_reg8(0x800620, read_reg8(0x800620)&0xf9); //set Timer0 to Mode0
    write_reg8(0x800620, read_reg8(0x800620)|0x01); //enable Timer0
    write_reg8(0x800640, read_reg8(0x800640)|0x01); //enable Timer0's IRQ
    write_reg8(0x800643, read_reg8(0x800643)|0x01); //enable IRQ

}
unsigned char loop = 0;

unsigned short BatteryValue[256];
unsigned char BatteryTick = 0;

unsigned short Sensor1Value[256];
unsigned char Sensor1Tick = 0;

unsigned short Sensor2Value[256];
unsigned char Sensor2Tick = 0;

void test_adc()
{
    if (Timer0_irq == 1) {
	        Timer0_irq = 0;
	        write_reg32(0x800630, 0); //set initial tick value of Timer0
		    //write_reg32(0x800624, tick_per_us*166); //set capture value of Timer0(0.166 ms)
		    write_reg32(0x800624, CLOCK_SYS_CLOCK_1US*166); //set capture value of Timer0(0.166 ms)
            write_reg8(0x800620, read_reg8(0x800620)&0xf9); //set Timer0 to Mode0
		    write_reg8(0x800620, read_reg8(0x800620)|0x01); //enable Timer0

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
	            adc_ChannelInit(B2, SINGLEEND, RV_1P428, RES14, S_3);
                WaitUs(30);
                Sensor2Value[Sensor2Tick++] = adc_SampleValueGet() & 0x3FFF;
	        }
	        loop = (loop+1) % 3;
    }
}

#endif
//----------------------- main--------------------------------------------
#define CMD_REG 0x8000
u32 tick_loop;
u8 cmd =0;

void main_loop ()
{
	tick_loop ++;
    //static int uart_enable;

    extern void test_adc(void );

    cmd = read_reg8(CMD_REG);
    switch( cmd ){

        case TEST_NULL:
            break;
        case TEST_ADC:                  // 3ch adc
            adc_enable = !adc_enable;
            CMD_DONE;
            break;
        case TEST_SPI_MASTER_W:         // spi master , can i2c spi work at the same time?
            test_spi_master_write();
            CMD_DONE;
            break;
        case TEST_SUSPEND:              // suspend
            test_suspend();
            CMD_DONE;
            break;
        case TEST_DEEP_SLEEP:           // deepsleep
            test_deep_sleep();
            CMD_DONE;
            break;

        case TEST_PWM:                 // pwm
        	test_pwm();
            CMD_DONE;
            break;

        case TEST_UART:                 // uart
            uart_enable = !uart_enable;
            if (uart_enable) uart_init();
            CMD_DONE;
            break;
        case TEST_CMD_DONE:
            break;
        default :
            write_reg8(CMD_REG, TEST_INVALID_CMD);
            break;

    }

    if(uart_enable) uart_process();
    if(adc_enable) test_adc();

}

#endif
