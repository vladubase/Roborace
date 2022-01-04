/*******************************************************************************
 *	@name		ROBORACE_MCU
 *	@file 		main.c
 *
 *	@author 	Uladzislau 'vladubase' Dubatouka
 *				<vladubase@gmail.com>.
 *	@version	V1.0
 *	@date 		04-January-2022
 ******************************************************************************/

#include "main.h"


/************************************ Main ************************************/


int main (void) {
	// DEFINITION OF VARIABLES
	
	
	// MICROCONTROLLER INITIALIZATION
	InitRCC ();
	InitSWD ();
	InitSystem ();
	InitGPIO ();
	InitUART ();
	InitADC ();
	
	// MAIN CYCLE
	while (true) {
		
	}
}
