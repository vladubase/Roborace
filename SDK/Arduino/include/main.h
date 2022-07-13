#ifndef _MAIN_H_
#define _MAIN_H_

/********************************* Includes ***********************************/

#include <stdint.h>
#include <stdbool.h>

#include <Servo.h>
#include <Wire.h>
#include <VL53L1X.h>
#include <Adafruit_MPU6050.h>
#include <EEPROM.h>

// User headers.
#include "config.h"


/********************************** Defines ***********************************/

#define		sbi(port,bit)	(port) |= (1 << (bit))	// Toggle pin.


/***************************** Global Variables *******************************/
uint32_t	i = 0;
uint8_t		STATUS_LASER = 0x00;
uint8_t		STATUS_MPU6050 = 0x00;
uint16_t	laser_data[NUM_OF_SENSORS] = {0};

float		voltage_motor = 12.6;				// *Volts.

char		UART_request = 0;
String		UART_request_str = "";
float		UART_request_float = 0.0;

int16_t		err_dist_side = 0;					// Distance error from robot to side obstacles.
uint16_t	servo_turning = 0;					// Error from PID for turning wheels.

float		motor_speed_coefficient = 0;

float		error_history[QTY_OF_ERR] = {0};	// Storing the values of recent errors.
float		error_sum = 0.0;					// Sum of errors in history.
float		P = 0.0;
float		I = 0.0;
float		D = 0.0;
float		PID_total_correction = 0.0;

bool		start_init_do_once = false;

// Definition of class objects.
Servo	motor,
		servo;
VL53L1X	laser_left,
		laser_left45,
		laser_0,
		laser_right45,
		laser_right,
		laser_back;
Adafruit_MPU6050 mpu;


/********************************* Functions **********************************/

void (* ResetMCU) (void) = 0;					// Software RESET.

void I2CMuxChSelect (uint8_t i) {
	if (i > 7) return;

	Wire.beginTransmission (I2C_MUX_ADDRESS);
	Wire.write (1 << i);
	Wire.endTransmission ();  
}

void InitLasers (void) {
	I2CMuxChSelect (LASER_ID_LEFT);		laser_left.setTimeout 	(I2C_TIMEOUT);
	I2CMuxChSelect (LASER_ID_LEFT45);	laser_left45.setTimeout (I2C_TIMEOUT);
	I2CMuxChSelect (LASER_ID_0);		laser_0.setTimeout 		(I2C_TIMEOUT);
	I2CMuxChSelect (LASER_ID_RIGHT);	laser_right45.setTimeout(I2C_TIMEOUT);
	I2CMuxChSelect (LASER_ID_RIGHT45);	laser_right.setTimeout 	(I2C_TIMEOUT);
	
	// if bit not set - LASER init error.
	I2CMuxChSelect (LASER_ID_LEFT);		if (laser_left.init ())		{ STATUS_LASER |= (1 << 0); }
	I2CMuxChSelect (LASER_ID_LEFT45);	if (laser_left45.init ())	{ STATUS_LASER |= (1 << 1); }
	I2CMuxChSelect (LASER_ID_0);		if (laser_0.init ())		{ STATUS_LASER |= (1 << 2); }
	I2CMuxChSelect (LASER_ID_RIGHT);	if (laser_right45.init ())	{ STATUS_LASER |= (1 << 3); }
	I2CMuxChSelect (LASER_ID_RIGHT45);	if (laser_right.init ())	{ STATUS_LASER |= (1 << 4); }

	// Distance:	Long, Medium, Short
	// Period (ms):  50     33     20
	I2CMuxChSelect (LASER_ID_LEFT);		laser_left.setDistanceMode (VL53L1X::Medium);
	I2CMuxChSelect (LASER_ID_LEFT);		laser_left.setMeasurementTimingBudget (33000);
	I2CMuxChSelect (LASER_ID_LEFT);		laser_left.startContinuous (33);
	
	I2CMuxChSelect (LASER_ID_LEFT45);	laser_left45.setDistanceMode (VL53L1X::Medium);
	I2CMuxChSelect (LASER_ID_LEFT45);	laser_left45.setMeasurementTimingBudget (33000);
	I2CMuxChSelect (LASER_ID_LEFT45);	laser_left45.startContinuous (33);
	
	I2CMuxChSelect (LASER_ID_0);		laser_0.setDistanceMode (VL53L1X::Medium);
	I2CMuxChSelect (LASER_ID_0);		laser_0.setMeasurementTimingBudget (33000);
	I2CMuxChSelect (LASER_ID_0);		laser_0.startContinuous (33);
	
	I2CMuxChSelect (LASER_ID_RIGHT);	laser_right45.setDistanceMode (VL53L1X::Medium);
	I2CMuxChSelect (LASER_ID_RIGHT);	laser_right45.setMeasurementTimingBudget (33000);
	I2CMuxChSelect (LASER_ID_RIGHT);	laser_right45.startContinuous (33);
	
	I2CMuxChSelect (LASER_ID_RIGHT45);	laser_right.setDistanceMode (VL53L1X::Medium);
	I2CMuxChSelect (LASER_ID_RIGHT45);	laser_right.setMeasurementTimingBudget (33000);
	I2CMuxChSelect (LASER_ID_RIGHT45);	laser_right.startContinuous (33);
}

void InitMPU6050 (void) {
	I2CMuxChSelect (MPU6050_ID);		while (!mpu.begin ());
	STATUS_MPU6050 |= (1 << 7);
	
	I2CMuxChSelect (MPU6050_ID);		mpu.setAccelerometerRange (MPU6050_RANGE_16_G);
	I2CMuxChSelect (MPU6050_ID);		mpu.setGyroRange (MPU6050_RANGE_2000_DEG);
	I2CMuxChSelect (MPU6050_ID);		mpu.setFilterBandwidth (MPU6050_BAND_44_HZ);
}

void RobotSetupMenu (void) {
	uint8_t	menu_pos = 1;

menu_1:
	while (menu_pos) {
		menu_pos = 1;
		Serial.print ("\n\n\n");	Serial.println ("*** SETUP MENU ***");

		Serial.println ("[1] Setup Motor");
		Serial.println ("[2] Setup PID");
		Serial.println ("");
		Serial.println ("[0] Exit\n");

		Serial.println ("\t* choose number");
		// Serial.flush ();

		do {
			if (Serial.available () > 0 && menu_pos) {
				UART_request =  Serial.read ();

				if (UART_request == '0') {
					menu_pos = 0;
					return;
				}

				if (UART_request == '1') {
menu_11:
					menu_pos = 11;
					Serial.print ("\n\n\n");	Serial.println ("*** SETUP MOTOR ***");

					Serial.println ("[1] Speed.");
					Serial.println ("");
					Serial.println ("[0] Exit.\n");

					Serial.println ("\t* choose number");
					// Serial.flush ();

					do {
						if (Serial.available () > 0 && menu_pos) {
							UART_request =  Serial.read ();

							if (UART_request == '0') {
								menu_pos = 1;
								goto menu_1;
							}

							if (UART_request == '1') {
menu_111:
								menu_pos = 111;
								Serial.print ("\n\n\n");	Serial.println ("*** SETUP MOTOR SPEED ***");

								Serial.print ("Current value is ");
								Serial.print (EEPROM.read (10));
								Serial.println (" %");

								Serial.println ("[*] Write speed in percents: _____");
								Serial.println ("");
								Serial.println ("[0] Exit\n");

								Serial.println ("\t* write number");
								// Serial.flush ();

								do {
									UART_request = Serial.read ();
									
									if (isDigit (UART_request)) {
										UART_request_str += (char)UART_request;
									}
								} while (!(UART_request == '\n'));

								if (UART_request_str.toInt() == '0') {
									menu_pos = 11;
									goto menu_11;
								}
								
								if (UART_request_str.toInt() > 0) {
									motor_speed_coefficient = UART_request_str.toInt() / 100;
									EEPROM.put (10, motor_speed_coefficient);
									UART_request_str = "";

									Serial.print ("Speed coefficient is set to ");
									Serial.print (EEPROM.get (10, f_type));
									Serial.println (" %");

									goto menu_111;
								}
							}
						}
					} while (UART_request > 0);
				}
				
				if (UART_request == '2') {
menu_12:
					menu_pos = 12;
					Serial.print ("\n\n\n");	Serial.println ("*** SETUP PID ***");

					Serial.println ("Current values are ");
					Serial.print ("\tkP: ");	Serial.println (EEPROM.get (20, f_type));
					Serial.print ("\tkI: ");	Serial.println (EEPROM.get (24, f_type));
					Serial.print ("\tkD: ");	Serial.println (EEPROM.get (28, f_type));
					Serial.println ("");

					Serial.println ("[1] Change kP.");
					Serial.println ("[2] Change kI.");
					Serial.println ("[3] Change kD.");
					Serial.println ("");
					Serial.println ("[0] Exit.\n");

					Serial.println ("\t* choose number");
					// Serial.flush ();

					do {
						if (Serial.available () > 0 && menu_pos) {
							UART_request =  Serial.read ();

							if (UART_request == '0') {
								menu_pos = 1;
								goto menu_1;
							}

							if (UART_request == '1') {
menu_121:
								menu_pos = 121;
								Serial.print ("\n\n\n");	Serial.println ("*** SETUP PID - kP ***");

								Serial.print ("Current value is ");
								Serial.print (EEPROM.get (18, f_type));

								Serial.println ("");
								Serial.println ("[*] Write value: _____");
								Serial.println ("");
								Serial.println ("[0] Exit\n");

								Serial.println ("\t* write number");
								// Serial.flush ();

								// do {
								UART_request_float = Serial.parseFloat ();
									
								// 	if (isDigit (UART_request)) {
								// 		UART_request_str += (char)UART_request;
								// 	}
								// } while (!(UART_request == '\n'));

								if (UART_request_float <= 0.000001 && UART_request_float >= -0.000001) {
									menu_pos = 12;
									goto menu_12;
								}
								
								if (UART_request_float) {
									kP = UART_request_float;
									EEPROM.put (18, kP);
									UART_request_float = 0.0;

									Serial.print ("Speed coefficient is set to ");
									Serial.print (EEPROM.get (18, f_type));
									Serial.println (" %");

									goto menu_121;
								}
							}

							if (UART_request == '2') {
menu_122:
								menu_pos = 122;
								Serial.print ("\n\n\n");	Serial.println ("*** SETUP PID - kI ***");

								Serial.print ("Current value is ");
								Serial.print (EEPROM.get (22, f_type));

								Serial.println ("");
								Serial.println ("[*] Write value: _____");
								Serial.println ("");
								Serial.println ("[0] Exit\n");

								Serial.println ("\t* write number");
								// Serial.flush ();

								// do {
								UART_request_float = Serial.parseFloat ();
									
								// 	if (isDigit (UART_request)) {
								// 		UART_request_str += (char)UART_request;
								// 	}
								// } while (!(UART_request == '\n'));

								if (UART_request_float <= 0.000001 && UART_request_float >= -0.000001) {
									menu_pos = 12;
									goto menu_12;
								}
								
								if (UART_request_float) {
									kP = UART_request_float;
									EEPROM.put (22, kP);
									UART_request_float = 0.0;

									Serial.print ("Speed coefficient is set to ");
									Serial.print (EEPROM.get (22, f_type));
									Serial.println (" %");

									goto menu_122;
								}
							}

							if (UART_request == '3') {
menu_123:
								menu_pos = 123;
								Serial.print ("\n\n\n");	Serial.println ("*** SETUP PID - kD ***");

								Serial.print ("Current value is ");
								Serial.print (EEPROM.get (26, f_type));

								Serial.println ("");
								Serial.println ("[*] Write value: _____");
								Serial.println ("");
								Serial.println ("[0] Exit\n");

								Serial.println ("\t* write number");
								// Serial.flush ();

								// do {
								UART_request_float = Serial.parseFloat ();
									
								// 	if (isDigit (UART_request)) {
								// 		UART_request_str += (char)UART_request;
								// 	}
								// } while (!(UART_request == '\n'));

								if (UART_request_float <= 0.000001 && UART_request_float >= -0.000001) {
									menu_pos = 12;
									goto menu_12;
								}
								
								if (UART_request_float) {
									kP = UART_request_float;
									EEPROM.put (26, kP);
									UART_request_float = 0.0;

									Serial.print ("Speed coefficient is set to ");
									Serial.print (EEPROM.get (26, f_type));
									Serial.println (" %");

									goto menu_123;
								}
							}
						}
					} while (UART_request > 0);
				}
			}
		} while (UART_request > 0);
	}
}

// void MotorSetSpeed (float speed, char *units="m/s") {
// 	float speed_correction = 1.0;

// 	if (units == "m/s")		{ speed_correction = 1.0; }
// 	if (units == "km/h")	{ speed_correction = 3.6; }
// }


#endif /* _MAIN_H_ */
