#ifndef _MAIN_H_
#define _MAIN_H_

/********************************* Includes ***********************************/

#include <stdint.h>
#include <stdbool.h>

#include <Servo.h>
#include <Wire.h>
#include <VL53L1X.h>
#include <Adafruit_MPU6050.h>

// User headers.
#include "config.h"

// GLOBAL VARIABLES
uint32_t	i = 0, j = 0;
uint8_t		STATUS_LASER = 0x00;
uint8_t		STATUS_BLUETOOTH = 0x00;
uint8_t		STATUS_MPU6050 = 0x00;
uint16_t	laser_data[6] = {0};
uint8_t		voltage_motor = 120;				// *(Volts * 10). Use int instead float. 1-digit accuracy.


// Definition of class objects.
Servo	ESC,
		servo;
VL53L1X	laser_left,
		laser_left45,
		laser_0,
		laser_right45,
		laser_right,
		laser_back;
Adafruit_MPU6050 mpu;


void I2CMuxChSelect (uint8_t i) {
	if (i > 7) return;

	Wire.beginTransmission (I2C_ADDRESS_MUX);
	Wire.write (1 << i);
	Wire.endTransmission ();  
}

#endif /* _MAIN_H_ */
