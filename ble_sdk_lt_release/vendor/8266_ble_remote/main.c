
#include "../../proj/tl_common.h"

#if(__PROJECT_8266_BLE_REMOTE__)

#include "../../proj/mcu/watchdog_i.h"
#include "../../vendor/common/user_config.h"
#include "../../proj_lib/rf_drv.h"
#include "../../proj_lib/pm.h"
#include "../../proj_lib/blt_ll/blt_ll.h"

extern void user_init();
extern void irq_blt_slave_handler(void);
extern void main_loop (void);
 u8 flag;

_attribute_ram_code_ void irq_handler(void)
{
#if 1

	u32 src = reg_irq_src;
	if(src & FLD_IRQ_GPIO_RISC2_EN){
		if(1)
		{
			flag++;
			 gpio_write(GPIO_PA1,1);
		}
		else
				{
			flag=0;
					 gpio_write(GPIO_PA1,0);
				}

		//reg_tmr_sta = FLD_IRQ_GPIO_RISC2_EN;
		reg_irq_src = FLD_IRQ_GPIO_RISC2_EN;
	}
#endif
	irq_blt_slave_handler ();
}

int main (void) {

	cpu_wakeup_init();

	clock_init();

	gpio_init();

	rf_drv_init(CRYSTAL_TYPE);

	user_init ();

    irq_enable();

	while (1) {
		main_loop ();
	}
}

#endif


