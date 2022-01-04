#ifndef _INIT_UART_H_
#define _INIT_UART_H_

/*******************************************************************************
 *	@name		STM32F4_UART
 *	@file 		InitUART.h
 *
 *	@author 	Uladzislau 'vladubase' Dubatouka
 *				<vladubase@gmail.com>
 *	@version	V1.0
 *	@date 		21-July-2021
 ******************************************************************************/


/********************************** Includes **********************************/

#include "stm32f4xx.h"

/********************************** Defines ***********************************/

#ifndef 	f_APB1
	#define f_APB1				((uint32_t)	 100e6)
#endif
#define 	USART1_BAUDRATE		((uint32_t)	256000)


/***************************** Function  prototypes ***************************/

void InitUART (void);
void InitUSART1 (void);
void USART1_SendByte (char chr);
void USART1_SendString (char* str);


#endif /* _INIT_UART_H_ */
