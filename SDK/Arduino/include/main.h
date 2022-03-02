#ifndef _MAIN_H_
#define _MAIN_H_

/********************************* Includes ***********************************/

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include <Servo.h>
#include <Wire.h>
#include <VL53L1X.h>
#include <Adafruit_MPU6050.h>

// User headers
// #include "InitMPU6050.h"

// #include <MedianFilterLib.h>

/********************************** Defines ***********************************/

/* GLOBAL */
#define		RELEASE					true						// Use for full-test.
#define		DEBUG_MOTOR				false
#define		DEBUG_SERVO				false
#define		DEBUG_BLUETOOTH			false
#define		DEBUG_LASERS			false
#define		DEBUG_MPU6050			false

#define		UART_BAUD				((uint32_t)	115200)
#define		UART_TIMEOUT			((uint16_t)	   200)
#define		I2C_SPEED				((uint32_t)	400000)
#define		I2C_TIMEOUT				((uint16_t)		 1)
// #define		I2C_ADDRESS_MPU6050		((uint8_t)	  0x68)
#define		I2C_ADDRESS_MUX			((uint8_t)	  0x70)			// I2C Multiplexer address.
// #define		I2C_ADDRESS_LASER		((uint8_t)	  0x29)
// #define		I2C_MUX_LASER_LEFT_ID	((uint8_t)		 0)
// #define		I2C_MUX_LASER_LEFT45_ID	((uint8_t)		 1)
// #define		I2C_MUX_LASER_0_ID		((uint8_t)		 2)
// #define		I2C_MUX_LASER_RIGHT45_ID ((uint8_t)		 3)
// #define		I2C_MUX_LASER_RIGHT_ID	((uint8_t)		 4)
// #define		I2C_MUX_LASER_BACK_ID	((uint8_t)		 5)

/* PHYSICS */
// #define		CAR_WEIGHT				((uint16_t)	   750)			// *grams.
// #define		MOTOR_VOLTAGE			((float)	 3*4.2)
// #define		MOTOR_KV				((uint16_t)	   930)
// #define		MOTOR_GEAR_RATIO		((uint8_t)	   8/1)			// 8:1.
// #define		WHEEL_DIAMETER			((uint8_t)		60)			// *mm.
// #define		CAR_TURNING_RADIUS		((uint8_t)		30)			// *cm.

/* ESC */
#define		ESC_PIN					((uint8_t)		 9)
#define		ESC_MIN_SIG_US			((uint16_t)	  1000)
#define		ESC_MAX_SIG_US			((uint16_t)	  2000)

/* SERVO */
#define		SERVO_PIN				((uint8_t)		10)
#define		SERVO_LEFT_US			((uint16_t)	  1000)
#define		SERVO_0_US				((uint16_t)	  1500)
#define		SERVO_RIGHT_US			((uint16_t)	  2000)
#define		WHEEL_MAX_STEERING_ANGLE ((uint8_t)		22)			// *degrees.

/* SENSORS */
// LASER
#define		LASER_HZ				((uint8_t)		50)

// MPU6050 : GYRO, ACCELEROMETER, COMPASS


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
