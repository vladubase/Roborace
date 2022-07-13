#ifndef _CONFIG_H_
#define _CONFIG_H_



float		f_type = 0;
int16_t		i_type = 0;

/********************************** Defines ***********************************/

/* GLOBAL */
#define		RELEASE_AUTO			true
#define		RELEASE_MANUAL			false

#define		DEBUG_MPU6050			false
#define		DEBUG_MOTOR				false
#define		DEBUG_MOTOR_STICK		false
#define		DEBUG_SERVO				false
#define		DEBUG_LASERS			false
#define		DEBUG_MOTOR_LASER_0		false

#define		SEND_DEBUG_INFO_AUTO	false

/* PHYSICS */
#define		CAR_WEIGHT				((uint16_t)		  1000)		// *grams.
#define		MOTOR_KV				((uint16_t)		   930)
#define		MOTOR_GEAR_RATIO		((uint8_t)		   8/1)		// 8:1.
#define		WHEEL_DIAMETER			((uint8_t)			66)		// *mm.
#define		CAR_TURNING_RADIUS		((uint8_t)			75)		// *cm.
#define		WHEEL_MAX_STEERING_ANGLE ((uint8_t)			20)		// *degrees.

// PID
// Setup: P -> PD -> PID
float		kP					  =	EEPROM.get (18, f_type);	// Proportional	feedback coefficient.	//	default: 0.5
float		kI					  =	EEPROM.get (22, f_type);	// Integral 	feedback coefficient.	//	default: 0.0
float		kD					  =	EEPROM.get (26, f_type);	// Differential	feedback coefficient.	// 	default: 0.0
// #define		kP						((float)		   0.6)
// #define		kI						((float)		   0.2)
// #define		kD						((float)		   0.0)
#define		MAIN_CYCLE_DELAY		((uint32_t)	        50)		// The main cycle delay (in ms).
																// for correct work of D-regulation.
#define		QTY_OF_ERR				((uint8_t)			10)		// Quantity of errors in memory during last
																// (QTY_OF_ERR * MAIN_CYCLE_DELAY) ms.

/* COMMUNICATION */
// USART
#define		UART_BAUD				((uint32_t)		  9600)
#define		UART_TIMEOUT			((uint16_t) MAIN_CYCLE_DELAY)

// I2C
#define		I2C_SPEED				((uint32_t)		400000)
#define		I2C_TIMEOUT				((uint16_t)			 1)
#define		I2C_MUX_ADDRESS			((uint8_t)		  0x70)		// I2C Multiplexer address.

/* MOTOR */
#define		ESC_PIN					((uint8_t)			 9)
#define		ESC_SIG_US_ZERO			((uint16_t)		  1350)
#define		ESC_SIG_US_BACK_MAX		((uint16_t)		  1000)
#define		ESC_SIG_US_BACK_MIN		((uint16_t)		  1290)
#define		ESC_SIG_US_FORW_MIN		((uint16_t)		  1430)
#define		ESC_SIG_US_FORW_MAX		((uint16_t)		  2000)


/* SERVO */
#define		SERVO_PIN				((uint8_t)			10)
// float		SERVO_SIG_US_LEFT	  = EEPROM.get (30, i_type);	//	default: 1925
// float		SERVO_SIG_US_0		  = EEPROM.get (32, i_type);	//	default: 1512
// float		SERVO_SIG_US_RIGHT	  = EEPROM.get (34, i_type);	// 	default: 1100
#define		SERVO_SIG_US_LEFT		((uint16_t)		  2050)
#define		SERVO_SIG_US_0			((uint16_t)		  1450)
#define		SERVO_SIG_US_RIGHT		((uint16_t)		  850)

/* SENSORS */
// LASER : VL5301X
#define		NUM_OF_SENSORS			((uint8_t)			 5)
#define		LASER_ID_LEFT			((uint8_t)			 7)		// ID on I2C Mux.
#define		LASER_ID_LEFT45			((uint8_t)			 5)
#define		LASER_ID_0				((uint8_t)			 4)
#define		LASER_ID_RIGHT45		((uint8_t)			 3)
#define		LASER_ID_RIGHT			((uint8_t)			 2)

// MPU6050 : GYRO, ACCELEROMETER, COMPASS
#define		MPU6050_ID				((uint8_t)			 6)


/* ADC */
#define		VOLTAGE_5V				((float)		 5.012)
#define 	ADC_STEP_VOLTS			((float)  (VOLTAGE_5V) / (1 << 10))		// Volts per 1 ADC step.
																			// ADC 10-bit.
#define		ADC_VOLTAGE_MOTOR_PIN	((uint8_t)			 0)
#define		ADC_volt_div_motor_R1	((uint16_t)		 10000)
#define		ADC_volt_div_motor_R2	((uint16_t)		  6650)

/* PERIPHERALS */
#define		SENSOR_IR_PIN			((uint8_t)			 8)
#define		BEEP_PIN				((uint8_t)			11)
#define		LED_PROCESS_PIN			((uint8_t)			13)		// Show that main cycle is work.


#endif /* _CONFIG_H_ */
