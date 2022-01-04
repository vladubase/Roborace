/*******************************************************************************
 *	@name		STM32F4_GPIO
 *	@file 		InitRCC.c
 *
 *	@author 	Uladzislau 'vladubase' Dubatouka
 *				<vladubase@gmail.com>
 *	@version	V1.0
 *	@date 		20-July-2021
 ******************************************************************************/

#include "InitGPIO.h"


/********************************* Functions **********************************/

/*
 *	@brief	This function setup General Purpose Input/Output.
 *			LED		PC13	Output
 *			KEY		PA0		Input
 */
void InitGPIO (void) {
	// Setup LED PC13
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;		// GPIOC clock enable.
	GPIOC->MODER |= GPIO_MODER_MODER13_0;		// 0b01:	General purpose output mode.
	GPIOC->OTYPER &= ~GPIO_OTYPER_OT_13;		// 0b00:	Output push-pull (reset state).
	GPIOC->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR13;	// 0b00:	Low speed.
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR13;		// 0b00:	No pull-up, pull-down.
	
	// Setup KEY PA0
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;		// GPIOA clock enable.
	GPIOA->MODER &= ~GPIO_MODER_MODER0;			// 0b00:	Input mode (reset state).
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_0;			// 0b00:	Output push-pull (reset state).
	GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR0;	// 0b00:	Low speed.
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR0_0;		// 0b01:	Pull-up.
}
