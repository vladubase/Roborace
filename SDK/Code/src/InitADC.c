/*******************************************************************************
 *	@name		STM32F4_ADC
 *	@file 		InitADC.c
 *
 *	@author 	Uladzislau 'vladubase' Dubatouka
 *				<vladubase@gmail.com>
 *	@version	V1.0
 *	@date 		21-July-2021
 ******************************************************************************/

#include "InitADC.h"


/********************************* Functions **********************************/

void InitADC (void) {
	InitADC1CH1 ();
}

void InitADC1CH1 (void) {
/**
 *	@brief	This function setup ADC1 channel 1; PA1.
 */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;		// GPIOA clock enable.
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;			// ADC1 clock enable.
	ADC->CCR |= ADC_CCR_ADCPRE_0;				// 0b01:	PCLK2 divided by 4	@25MHz.
	
	/* ******************************* ADC1 PA1 ***************************** */
	GPIOA->MODER |= GPIO_MODER_MODER1;			// 0b11:	Analog mode.
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_1;			// 0b00:	Output push-pull (reset state).
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR0;	// 0b11:	High speed.
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR1;			// 0b00:	No pull-up, pull-down.
	
	/* ********************************************************************** */
	ADC1->CR1 &= ~ADC_CR1_RES;					// 12-bits data resolution.
	
	ADC1->CR2 |= ADC_CR2_ADON;					// Enable the ADC.
	ADC1->CR2 &= ~ADC_CR2_CONT;					// 0b0:		Single conversion.
	
	// Sampling time selection.
	ADC1->SMPR2 |= ADC_SMPR2_SMP1;				// 480 cycles.
	
	// ADC channel selection.
	ADC1->SQR3 |= ADC_SQR3_SQ1_0;				// Channel 1, PA1
}

uint16_t ADC1_StartConversion (void) {
/**
 *	@brief	Start ADC conversion.
 *	@param	None.
 *	@retval	Return the conversion result from the last converted channel.
 */
	ADC1->CR2 |= ADC_CR2_SWSTART;				// ADC start conversion.
	while (!(ADC1->SR & ADC_SR_EOC));			// Wait end of conversion.
	
	return ADC1->DR;
}
