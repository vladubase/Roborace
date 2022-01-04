/*******************************************************************************
 *	@name		STM32F4_RCC_PLL_100MHz
 *	@file 		InitRCC.c
 *
 *	@author 	Uladzislau 'vladubase' Dubatouka
 *				<vladubase@gmail.com>
 *	@version	V1.0
 *	@date 		20-July-2021
 ******************************************************************************/

#include "InitRCC.h"


/********************************* Functions **********************************/

/*
 *	@brief	This function setup RCC. HSE will be used as PLL clock source.
 *			Use external crystal on 25 MHz. HCLK output is 100 MHz.
 */
void InitRCC (void) {
	RCC->CR &= ~RCC_CR_HSEON;
	RCC->CR &= ~RCC_CR_HSEBYP;
	RCC->CR |= RCC_CR_HSEON; 					// External High Speed clock enable.
	while (!(RCC->CR & RCC_CR_HSERDY));			// External High Speed clock ready flag.
	
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;			// Power interface clock enable.
	PWR->CR |= PWR_CR_VOS;						// Regulator voltage scaling output selection.
	
	// Configure Flash prefetch, Instruction cache, Data cache and wait state.
	FLASH->ACR = FLASH_ACR_DCEN |				// Data cache is enabled.
		FLASH_ACR_ICEN |						// Instruction cache is enabled.
		FLASH_ACR_PRFTEN | 						// Prefetch buffer enable.
		FLASH_ACR_LATENCY_3WS;					// The ratio of the CPU clock period to the Flash memory access time
												// Three wait states (4 CPU cycles).
	
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1 |			// SYSCLK not divided. AHB = SYSCLK.
		RCC_CFGR_PPRE1_DIV2 |					// APB1 = SYSCLK / 2.
		RCC_CFGR_PPRE2_DIV1;					// APB2 = SYSCLK.
	
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLM | RCC_PLLCFGR_PLLN |
		RCC_PLLCFGR_PLLP | RCC_PLLCFGR_PLLQ);
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE | 	// PREDIV1 clock selected as PLL entry clock source.
		(PLL_M << 0) |							// PLL M divider = 12.
		(PLL_N << 6) |							// PLL N multiplier = 96.
		(0x00 << 16);							// PLL P divider = 2.
	
	// Enable the main PLL.
	RCC->CR |= RCC_CR_PLLON;					// PLL enable.
	while(!(RCC->CR & RCC_CR_PLLRDY));			// PLL clock ready flag.
	
	// System clock MUX - PLLCLK.
	RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));	// Clear SW bits.
	RCC->CFGR |= RCC_CFGR_SW_PLL;						// Select source SYSCLK = PLL.
	while((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) !=  		// Wait till PLL is used.
		RCC_CFGR_SWS_PLL);
}
