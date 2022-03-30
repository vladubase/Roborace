#ifndef _CONFIG_H_
#define _CONFIG_H_


/********************************** Defines ***********************************/

/* GLOBAL */
#define		RELEASE					false						// Use for full-test.
#define		DEBUG_MOTOR				true
#define		DEBUG_SERVO				false
#define		DEBUG_BLUETOOTH			false
#define		DEBUG_LASERS			false
#define		DEBUG_MPU6050			false

#define		SEND_DEBUG_INFO			true

/* PHYSICS */
#define		CAR_WEIGHT				((uint16_t)	  1000)			// *grams.
#define		MOTOR_KV				((uint16_t)	   930)
#define		MOTOR_GEAR_RATIO		((uint8_t)	   8/1)			// 8:1.
#define		WHEEL_DIAMETER			((uint8_t)		60)			// *mm.
#define		CAR_TURNING_RADIUS		((uint8_t)		34)			// *cm.
#define		WHEEL_MAX_STEERING_ANGLE ((uint8_t)		21)			// *degrees.
#define		TIME_TO_FULL_STOP		((uint8_t)		50)			// *(sec * 10). Use int instead float. 1-digit accuracy.

/* COMMUNICATION */
// USART
#define		UART_BAUD				((uint32_t)	115200)
#define		UART_TIMEOUT			((uint16_t)	   200)

// I2C
#define		I2C_SPEED				((uint32_t)	400000)
#define		I2C_TIMEOUT				((uint16_t)		 1)
#define		I2C_ADDRESS_MUX			((uint8_t)	  0x70)			// I2C Multiplexer address.

/* MOTOR */
#define		ESC_PIN					((uint8_t)		 9)
#define		ESC_MIN_SIG_US			((uint16_t)	  1000)
#define		ESC_MAX_SIG_US			((uint16_t)	  2000)

/* SERVO */
#define		SERVO_PIN				((uint8_t)		10)
#define		SERVO_LEFT_US			((uint16_t)	  1000)
#define		SERVO_0_US				((uint16_t)	  1500)
#define		SERVO_RIGHT_US			((uint16_t)	  2000)

/* SENSORS */
// LASER
#define		NUM_OF_SENSORS			((uint8_t)		 5)
#define		LASER_LEFT_ID			((uint8_t)		 0)			// ID on I2C Mux.
#define		LASER_LEFT45_ID			((uint8_t)		 1)
#define		LASER_0_ID				((uint8_t)		 2)
#define		LASER_RIGHT45_ID		((uint8_t)		 3)
#define		LASER_RIGHT_ID			((uint8_t)		 4)

// MPU6050 : GYRO, ACCELEROMETER, COMPASS

#endif /* _CONFIG_H_ */
