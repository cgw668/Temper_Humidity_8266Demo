/*
 * pwm.h
 *
 *  Created on: 2015-11-27
 *      Author: Administrator
 */

#ifndef PWM_H_
#define PWM_H_


#if(__TL_LIB_8266__ || MCU_CORE_TYPE == MCU_CORE_8266)
#include "../mcu_spec/pwm_8266.h"
#elif(__TL_LIB_8267__ || MCU_CORE_TYPE == MCU_CORE_8267)
#include "../mcu_spec/pwm_8267.h"
#elif(__TL_LIB_8263__ || MCU_CORE_TYPE == MCU_CORE_8263)
#include "../mcu_spec/pwm_8263.h"
#endif


#endif /* PWM_H_ */
