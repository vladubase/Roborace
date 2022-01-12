/*******************************************************************************
 *	@name		STM32F4_SWD
 *	@file 		InitSWD.c
 *
 *	@author 	Uladzislau 'vladubase' Dubatouka
 *				<vladubase@gmail.com>
 *	@version	V1.0
 *	@date 		20-July-2021
 ******************************************************************************/

#include "InitSWD.h"


/********************************** Function **********************************/

/*
 *	@brief	This function setup Serial Wire Debug.
 *			PA14 SWCLK in pull-down.
 *			PA13 SWDIO in pull-up.
 */
void InitSWD (void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;			// GPIOA clock enable.
	
	GPIOA->MODER |= GPIO_MODER_MODER14_1 |			// Alternate function mode.
		GPIO_MODER_MODER13_1;
	
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_14 |			// Output push-pull (reset state).
		GPIO_OTYPER_OT_13);
	
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR14 |		// High speed.
		GPIO_OSPEEDER_OSPEEDR13;
	
	// PA14 SWCLK: input pull-down,
	// PA13 SWDIO: input pull-up.
	// @note	Reference Manual: General-purpose I/O (GPIO).	
	//			The debug pins are in AF pull-up/pull-down after reset.
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR14_1 | GPIO_PUPDR_PUPDR13_0;
	
	// GPIO_AFRH should be 0000: AF0 (SYS_AF).
	// @note	Datasheet: 			Alternate function mapping.
	// @note	Reference Manual: 	STM32F411xC/E register boundary addresses.
	// [1] element - means GPIO alternate function high register (GPIOx_AFRH).
	GPIOA->AFR[1] &= ~(1 << 0);						// Clear 0th bit.
}
