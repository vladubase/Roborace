/*******************************************************************************
 *	@name		STM32F4_DELAY
 *	@file 		delay.c
 *
 *	@author 	Uladzislau 'vladubase' Dubatouka
 *				<vladubase@gmail.com>.
 *	@version	V1.0
 *	@date 		04-January-2022
 ******************************************************************************/

#include "delay.h"


/********************************* Functions **********************************/

void delay_ms (uint32_t time) {
	SysTick->VAL &= ~SysTick_VAL_CURRENT_Msk;
	SysTick->VAL = f_PROCESSOR / (1000 - 1);
	SysTick_CNT = time;
	while (SysTick_CNT);
}
