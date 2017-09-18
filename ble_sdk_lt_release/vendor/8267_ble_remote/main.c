
#include "../../proj/tl_common.h"

#if(__PROJECT_BLE_REMOTE__)

#include "../../proj/mcu/watchdog_i.h"
#include "../../vendor/common/user_config.h"
#include "../../proj_lib/rf_drv.h"
#include "../../proj_lib/pm.h"
#include "../../proj_lib/blt_ll/blt_ll.h"

extern void user_init();
extern void deep_wakeup_proc(void);

_attribute_ram_code_ void irq_handler(void)
{
#if (BLE_IR_ENABLE)
	u32 src = reg_irq_src;
	if(src & FLD_IRQ_TMR1_EN){
		ir_irq_send();
		reg_tmr_sta = FLD_TMR_STA_TMR1;
	}

	if(src & FLD_IRQ_TMR2_EN){
		ir_repeat_handle();
		reg_tmr_sta = FLD_TMR_STA_TMR2;
	}
#endif

	irq_blt_slave_handler ();
}

int main (void) {

	cpu_wakeup_init();

	clock_init();

	gpio_init();

	deep_wakeup_proc();

	rf_drv_init(CRYSTAL_TYPE);

	user_init ();

    irq_enable();

	while (1) {
#if (MODULE_WATCHDOG_ENABLE)
		wd_clear(); //clear watch dog
#endif
		main_loop ();
	}
}

#endif
