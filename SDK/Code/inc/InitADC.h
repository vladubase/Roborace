#ifndef _INIT_ADC_H_
#define _INIT_ADC_H_

/*******************************************************************************
 *	@name		STM32F4_ADC
 *	@file 		InitADC.h
 *
 *	@author 	Uladzislau 'vladubase' Dubatouka
 *				<vladubase@gmail.com>
 *	@version	V1.0
 *	@date 		21-July-2021
 ******************************************************************************/


/********************************** Includes **********************************/

#include "stm32f4xx.h"

/********************************** Defines ***********************************/

#define ADC1_LSB	((float)	3.3 / 4096)	// ADC Least significant bit. LSB = Vref / (2 ^ 12-bit res).


/***************************** Function  prototypes ***************************/

void InitADC (void);
void InitADC1CH1 (void);
uint16_t ADC1_StartConversion (void);


#endif /* _INIT_ADC_H_ */
