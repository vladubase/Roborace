#ifndef _MAIN_H_
#define _MAIN_H_

/*******************************************************************************
 *	@name		ROBORACE_MCU
 *	@file 		main.h
 *
 *	@author 	Uladzislau 'vladubase' Dubatouka
 *				<vladubase@gmail.com>
 *	@version	V1.0
 *	@date 		04-January-2022
 ******************************************************************************/


/********************************** Includes **********************************/

#include "stm32f4xx.h"

#include "stdint.h"
#include "stdbool.h"

// User headers
#include "InitRCC.h"
#include "InitSWD.h"
//#include "delay.h"
#include "InitGPIO.h"
#include "InitUART.h"
#include "InitADC.h"


//*********************************** Defines **********************************/

__IO uint32_t SysTick_CNT = 0;							// SysTick count.

/********************************** Functions *********************************/

void InitSystem (void) {
	// Enable Hardware Floating Point.
	SCB->CPACR |= ((3UL << 10*2) | (3UL << 11*2));
	
	// SysTick.
	SysTick->LOAD &= ~SysTick_LOAD_RELOAD_Msk;
	SysTick->LOAD = (uint32_t)(f_SYSCLK / (1000 - 1));	// 100 MHz / 1000 -> get ms.
	SysTick->VAL = ~SysTick_VAL_CURRENT_Msk;
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk |		// Processor clock (AHB).
		SysTick_CTRL_TICKINT_Msk |						// Asserts the SysTick exception request.
		SysTick_CTRL_ENABLE_Msk;						// Counter enabled.
}

void delay_ms (uint32_t time) {
	SysTick->VAL &= ~SysTick_VAL_CURRENT_Msk;
	SysTick->VAL = f_SYSCLK / (1000 - 1);
	SysTick_CNT = time;
	while (SysTick_CNT);
}

////////////////////////////////////////////
struct GYRO {
	
};

struct motor {
	uint16_t velocity;
	uint16_t velocity_target;
};

///////////////////////////////////////////








/********************************** Interrupts ********************************/

void SysTick_Handler (void) {
	if (SysTick_CNT > 0) {
		SysTick_CNT--;
	}
}

#endif /* _MAIN_H_ */
