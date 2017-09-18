
#include "../../proj/tl_common.h"

#if(__PROJECT_DRIVER_TEST__)

#include "../../proj/mcu/watchdog_i.h"
#include "../../vendor/common/user_config.h"
#include "../../proj_lib/rf_drv.h"
#include "../../proj_lib/pm.h"
#include "../../proj_lib/blt_ll/blt_ll.h"

extern void user_init();
extern unsigned char Timer0_irq;
extern int uart_enable;
//extern int adc_enable;
_attribute_ram_code_ void irq_handler(void)
{
#if 0
	u32 src = reg_irq_src;
	if(src & FLD_IRQ_TMR1_EN){
		irq_host_timer1();
		reg_tmr_sta = FLD_TMR_STA_TMR1;
	}
#endif
    if(uart_enable) uart_irq_handler_ex();
	//irq_blt_slave_handler ();
	if (read_reg8(0x800648) & 0x01) {                        //if timer0's irq occurs
        //if Timer0 assert irq
        if (read_reg8(0x800623) & 0x01) {
            write_reg8(0x800623,0x01);                 //clear Timer0's status
            Timer0_irq = 1;
        }
    }
}

int main (void) {

	cpu_wakeup_init();

	clock_init();

	gpio_init();


	rf_drv_init(0);

    usb_init();


	user_init ();

    irq_enable();

	while (1) {
        usb_handle_irq();
		main_loop ();
	}
}

#endif
