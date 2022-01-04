/*******************************************************************************
 *	@name		STM32F4_UART
 *	@file 		InitUART.c
 *
 *	@author 	Uladzislau 'vladubase' Dubatouka
 *				<vladubase@gmail.com>
 *	@version	V1.0
 *	@date 		21-July-2021
 ******************************************************************************/

#include "InitUART.h"


/********************************* Functions **********************************/

void InitUART (void) {
	InitUSART1 ();
}


/*
 *	@brief	This function setup USART1.
 *			USART1_RX		PA10
 *			USART1_RX		PA9
 */
void InitUSART1 (void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;			// GPIOA clock enable.
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;			// USART1 clock enable.
	
	/* ****************************** PA10 RX ******************************* */
	GPIOA->MODER |= GPIO_MODER_MODER10_1;			// Alternate function mode.
	GPIOA->OTYPER &= GPIO_OTYPER_OT_10;				// Output push-pull (reset state).
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR10;		// High speed.
	GPIOA->PUPDR &= GPIO_PUPDR_PUPDR10;				// No pull-up, pull-down

	/* ****************************** PA9  TX ******************************* */
	GPIOA->MODER |= GPIO_MODER_MODER9_1;			// Alternate function mode.
	GPIOA->OTYPER &= GPIO_OTYPER_OT_9;				// Output push-pull (reset state).
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR10;		// High speed.
	GPIOA->PUPDR &= GPIO_PUPDR_PUPDR9;				// No pull-up, pull-down
	
	// Alternative function selection.
	GPIOA->AFR[1] = (7 << (1 * 4)) | (7 << (2 * 4));
	
	USART1->CR1 |= USART_CR1_UE;					// USART Enable.
	USART1->CR1 &= ~USART_CR1_M;					// Word - 8 data bits.
	USART1->CR2 &= ~USART_CR2_STOP;					// 1 stop bit.
	USART1->BRR = (f_APB1 + USART1_BAUDRATE / 2) / 
		USART1_BAUDRATE;
	USART1->CR1 |= USART_CR1_RE |					// Receiver Enable.
		USART_CR1_TE;								// Transmitter Enable.
	
	/* *************************** USART1 Interrupt ************************* */
	// A USART interrupt is generated whenever ORE=1 or 
	// RXNE=1 in the USART_SR register.
	USART1->CR1 |= USART_CR1_RXNEIE;				// RXNE interrupt enable.
	NVIC_EnableIRQ (USART1_IRQn);
}

void USART1_SendByte (char chr) {
	while (!(USART1->SR & USART_SR_TC));			// Wait Transmission complete flag.
	
	USART1->DR = chr;
}

void USART1_SendString (char* str) {
	uint8_t i = 0;
	
	while (str[i]) {
		USART1_SendByte (str[i++]);
	}
}
