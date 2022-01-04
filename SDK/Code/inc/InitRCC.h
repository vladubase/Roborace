#ifndef _INIT_RCC_H_
#define _INIT_RCC_H_

/*******************************************************************************
 *	@name		STM32F4_RCC_PLL_100MHz
 *	@file 		InitRCC.h
 *
 *	@author 	Uladzislau 'vladubase' Dubatouka
 *				<vladubase@gmail.com>
 *	@version	V1.0
 *	@date 		20-July-2021
 ******************************************************************************/


/********************************** Includes **********************************/

#include "stm32f4xx.h"

/********************************** Defines ***********************************/

#define f_HSE		25e6						// Value of the External oscillator in Hz.
#define PLL_M		12							// /M - Division factor for the main PLL (PLL) input clock.
#define	PLL_N		96							// *N - Main PLL (PLL) multiplication factor for VCO.
#define	PLL_P		2							// /P - Main PLL (PLL) division factor for main system clock.
#define	PLL_Q		4							// /Q - Main PLL (PLL) division factor for USB OTG FS, and SDIO clocks.
#define f_VCO		f_HSE * PLL_N / PLL_M		// VCO clock.
#define f_PLL_OUT	f_VCO / PLL_P				// PLL general clock output. 	100 MHz output.
#define f_USB_SDIO	f_VCO / PLL_Q				// USB OTG FS, SDIO clock.

#define f_SYSCLK	f_PLL_OUT					// System clock.				100 MHz.

#define AHB_PRE		1							// AHB prescaller.
#define f_HCLK		f_SYSCLK / AHB_PRE			// HCLK to AHB bus, core, memory, DMA;
												// to Cortex System timer;
												// FCLK Cortex clock;
												// APB1 bus;
												// APB2 bus.

#define f_AHB		f_HCLK						// AHB bus clock.				100 MHz.
#define f_AHB1		f_HCLK						// RCC AHB1 peripheral clock.	100 MHz.
#define f_AHB2		f_HCLK						// RCC AHB2 peripheral clock.	100 MHz.
#define f_APB1		f_HCLK						// RCC APB1 peripheral clock.	100 MHz.
#define f_APB1_TIM	f_HCLK						// RCC APB1 Timer clock.		100 MHz.
#define f_APB2		f_HCLK / 2					// RCC APB2 peripheral clock.	 50 MHz.
#define f_APB2_TIM	f_HCLK						// RCC APB1 Timer clock.		100 MHz.


/***************************** Function  prototypes ***************************/

void InitRCC (void);


#endif /* _INIT_RCC_H_ */
