#ifndef _DELAY_H_
#define _DELAY_H_

/*******************************************************************************
 *	@name		STM32F4_DELAY
 *	@file 		delay.h
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


//********************************** Defines **********************************/

#define f_PROCESSOR		100e6						// System clock.	100 MHz.


/***************************** Function  prototypes ***************************/

void delay_ms (uint32_t time);



#endif /* _DELAY_H_ */
