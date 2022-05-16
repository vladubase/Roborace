/*******************************************************************************
 *  @name		Roborace
 *  @file		main.cpp
 *
 *  @author		Uladzislau 'vladubase' Dubatouka
 *				<vladubase@gmail.com>.
 * 				https://github.com/vladubase/Roborace
 *  @version	v0.4
 *  @date		15-May-2022
 ******************************************************************************/

#include "main.h"


/************************************ Main ************************************/

void setup () {
	// MICROCONTROLLER INITIALIZATION
		pinMode (LED_PROCESS_PIN,		OUTPUT);
		pinMode (ADC_VOLTAGE_MOTOR_PIN,	INPUT );
		pinMode (BEEP_PIN,				OUTPUT);
		pinMode (SENSOR_IR_PIN,			OUTPUT);

	// BLUETOOTH INITIALIZATION
		Serial.begin (UART_BAUD);
		Serial.print ("Bluetooth");
		while (!Serial);
		Serial.setTimeout (UART_TIMEOUT);				// Время ожидания данных.
		Serial.println (" - OK!");

		Serial.println ("\nIf want setup, send symbol 'S'");
		delay (3000);

		/*

		SETUP MENU
		|-- Setup Motor
		|	|-- Speed
		|-- Setup PID
		|	|-- Change kP
		|	|-- Change kI
		|	|-- Change kD

		*/
		RobotSetupMenu ();

		// if (Serial.available () > 0) {
		// 	if (Serial.read () == 'S') {
		// 		RobotSetupMenu ();
		// 	}
		// }
	
	// ESC INITIALIZATION.
		Serial.write ("ESC");
		motor.attach (ESC_PIN);
		motor.writeMicroseconds (ESC_SIG_US_ZERO);		delay (2000);
		motor.writeMicroseconds (ESC_SIG_US_FORW_MAX);	delay (2000);
		motor.writeMicroseconds (ESC_SIG_US_ZERO);
		Serial.write ("       - OK! \n");
	
	// SERVO INITIALIZATION.
		Serial.print ("SERVO");
		servo.attach (SERVO_PIN);
		servo.writeMicroseconds (SERVO_SIG_US_0);		delay (1000);
		servo.writeMicroseconds (SERVO_SIG_US_LEFT);	delay  (500);
		servo.writeMicroseconds (SERVO_SIG_US_0);		delay  (500);
		servo.writeMicroseconds (SERVO_SIG_US_RIGHT);	delay  (500);
		servo.writeMicroseconds (SERVO_SIG_US_0);		delay  (500);
		Serial.println ("     - OK!");

	// LASER SENSORS
		Serial.print ("LASER");		
		InitLasers ();
		Serial.println ("     - OK!");

//		// MPU6050
//		// Serial.print ("MPU6050");		
//		// InitMPU6050 ();
//		// Serial.println ("   - OK!");

	// DEBUG INFO
		Serial.print ("[ STATUS ] LASER: ");
		Serial.println (STATUS_LASER);
//		Serial.print ("[ STATUS ] MPU6050: ");
//		Serial.println (STATUS_MPU6050);
		Serial.print ("\n");
}

void loop () {
	#if RELEASE_AUTO
		uint32_t t1 = micros ();					// Start counting time.

		// Process input from Bluetooth.
		if (Serial.available () > 0) {
			UART_request = Serial.read ();
			if (UART_request == '!') {
				Serial.println ("RESET MCU");
				ResetMCU ();
				start_init_do_once = true;			// ? about RAM and adresses.
			} else if (UART_request == '#') {
				Serial.println ("STATUS INFO: ");
				Serial.print ("LASER: ");			Serial.println (STATUS_LASER);
//				Serial.print ("MPU6050: ");			Serial.println (STATUS_MPU6050);
			} else if (UART_request == '$') {
				Serial.println ("[ REINIT ] LASER");
				InitLasers ();
			} else if (UART_request == '%') {
				Serial.println ("[ REINIT ] ESC");
				motor.detach ();
				motor.attach (ESC_PIN);
				motor.writeMicroseconds (1000);
				delay (2000);
				motor.writeMicroseconds (2000);
				delay (2000);
				motor.writeMicroseconds (ESC_SIG_US_ZERO);
			} else if (UART_request == '&') {
				Serial.println ("STOP.");
				motor.writeMicroseconds (ESC_SIG_US_ZERO);
			}
		}

		#if SEND_DEBUG_INFO_AUTO
			voltage_motor = (analogRead (ADC_VOLTAGE_MOTOR_PIN) * ADC_STEP_VOLTS) * 	// ADC steps * 
								((ADC_volt_div_motor_R1 + ADC_volt_div_motor_R2) /		// volt. div. coefficient.
											ADC_volt_div_motor_R2);
			Serial.print ("Voltage Motor: ");
			Serial.print (voltage_motor);
			Serial.println (" V");
			if ((voltage_motor / 3) < 3.6) {				// *3 is the number of battery cells.
				
			}
		#endif /* SEND_DEBUG_INFO_AUTO */
		
		if (!(start_init_do_once)) {					// Because of f*cking Arduino main style.
			// Do once at start.
			error_history[QTY_OF_ERR] = {0};			// Storing the values of recent errors.
			error_sum = 0.0;							// Sum of errors in history.
			P = 0.0;
			I = 0.0;
			D = 0.0;
			PID_total_correction = 0.0;
			
			servo.writeMicroseconds (SERVO_SIG_US_0);	// Drive straight - 0 degree.
			motor.writeMicroseconds (ESC_SIG_US_ZERO);	// Drive at 0 speed.

//			// Waiting for a signal on IR sensor
//			#ifdef SENSOR_IR_PIN
//				Serial.println ("Wait for IR signal...");
//				do {
//					digitalWrite (!(LED_PROCESS_PIN, HIGH));
//					delay (30);
//					digitalWrite (LED_PROCESS_PIN, LOW);
//					delay (30);
//				} while (digitalRead (SENSOR_IR_PIN));
//			#endif /* SensorIR */
			
			Serial.println ("Wait for START condition...");
			do {
				if (Serial.available () > 0)	{ UART_request = Serial.read (); }
			} while (!(UART_request == '>'));

			Serial.print ("START after 5 sec ");
			delay (2000);		Serial.print ("...3");	// Delay as required by the rules - 5 sec.
			delay (1000);		Serial.print ("...2");
			delay (1000);		Serial.println ("...1");
			delay (1000);		Serial.println ("RUN RUHIDO, RUN! \n");
		}
		start_init_do_once = true;











		// READ LASERS
			I2CMuxChSelect (0);		laser_data[0] = laser_left.read ();
			I2CMuxChSelect (1);		laser_data[1] = laser_left45.read ();
			I2CMuxChSelect (2);		laser_data[2] = laser_0.read ();
			I2CMuxChSelect (3);		laser_data[3] = laser_right45.read ();
			I2CMuxChSelect (4);		laser_data[4] = laser_right.read ();
			
			#if SEND_DEBUG_INFO_AUTO
				Serial.print ("Laser range:\t");
				for (i = 0; i < NUM_OF_SENSORS; i++) {
					if (!(laser_data[i]))	{ STATUS_LASER &= ~(1 << i); }
					else 					{ STATUS_LASER |= (1 << i); }
					Serial.print (i);
					Serial.print (": ");
					Serial.print (laser_data[i]);
					Serial.print ("\t");
				}
				Serial.print ("\n");
			#else
				for (i = 0; i < NUM_OF_SENSORS; i++) {
					if (!(laser_data[i]))	{ STATUS_LASER &= ~(1 << i); }
					else 					{ STATUS_LASER |= (1 << i); }
				}
			#endif /* SEND_DEBUG_INFO_AUTO */

		// READ MPU6050
//			sensors_event_t a, g, temp;
//			I2CMuxChSelect (7);		mpu.getEvent (&a, &g, &temp);
//			
//			#if SEND_DEBUG_INFO_AUTO
//				Serial.print("Acceleration X: ");
//				Serial.print(a.acceleration.x);
//				Serial.print(", Y: ");
//				Serial.print(a.acceleration.y);
//				Serial.print(", Z: ");
//				Serial.print(a.acceleration.z);
//				Serial.println(" m/s^2");
//
//				Serial.print("Rotation X: ");
//				Serial.print(g.gyro.x);
//				Serial.print(", Y: ");
//				Serial.print(g.gyro.y);
//				Serial.print(", Z: ");
//				Serial.print(g.gyro.z);
//				Serial.println(" rad/s");
//
//				Serial.print("Temperature: ");
//				Serial.print(temp.temperature);
//				Serial.println(" degC");
//			#endif /* SEND_DEBUG_INFO_AUTO */

		// PID
			error_sum = 0.0;

			err_dist_side = laser_data[LASER_ID_RIGHT] + laser_data[LASER_ID_RIGHT45] -
							laser_data[LASER_ID_LEFT]  - laser_data[LASER_ID_LEFT45];
			
			for (i = 0; i < QTY_OF_ERR - 1 - 1; i++) {			// Shift error values.
				error_history[i] = error_history[i + 1];
			}
			error_history[QTY_OF_ERR - 1] = err_dist_side;

			P = error_history[QTY_OF_ERR - 1] * kP;				// current error * kP.
			for (i = 0; i < QTY_OF_ERR; i++) {
				error_sum += error_history[i];
			}
			I = error_sum / QTY_OF_ERR * kI;					// average error * kI.
			D = (error_history[QTY_OF_ERR - 1] -				// (current error - previous error) * kD.
				 error_history[QTY_OF_ERR - 1 - 1]) * kD;

			PID_total_correction = (P + I) + D;

			#if SEND_DEBUG_INFO_AUTO
				Serial.print ("err_dist_side: ");
				Serial.println (err_dist_side);
				Serial.print ("PID: P: ");		Serial.print (P);
				Serial.print (", I: ");			Serial.print (I);
				Serial.print (", D: ");			Serial.print (D);
				Serial.print (", total: ");		Serial.println (PID_total_correction);
			#endif /* SEND_DEBUG_INFO_AUTO */

		// DRIVING
			servo_turning = SERVO_SIG_US_0 - PID_total_correction;

			// Validating a range of variables.
			if (servo_turning < SERVO_SIG_US_RIGHT) {
				servo_turning = SERVO_SIG_US_RIGHT;
			} else if (servo_turning > SERVO_SIG_US_LEFT) {
				servo_turning = SERVO_SIG_US_LEFT;
			}
			servo.writeMicroseconds (servo_turning);

			#if SEND_DEBUG_INFO_AUTO
				Serial.print ("servo_turning:\t");
				Serial.println (servo_turning);
			#endif /* SEND_DEBUG_INFO_AUTO */


			// Serial.print ("LASER distance: ");	Serial.println (LASER_0_ID);
			// Serial.print ("Motor speed: ");	
			// if (laser_data[LASER_0_ID] >= 1000) {
			// 	motor.writeMicroseconds (ESC_SIG_US_FORW_MIN + 100);
			// 	Serial.println (ESC_SIG_US_FORW_MIN + 100);
			// } else if (laser_data[LASER_0_ID] < 1000 && laser_data[LASER_0_ID] > 500) {
			// 	motor.writeMicroseconds (ESC_SIG_US_FORW_MIN + 50);
			// 	Serial.println (ESC_SIG_US_FORW_MIN + 50);
			// } else if (laser_data[LASER_0_ID] < 500 && laser_data[LASER_0_ID] > 200) {
			// 	motor.writeMicroseconds (ESC_SIG_US_FORW_MIN);
			// 	Serial.println (ESC_SIG_US_FORW_MIN);
			// } else {
			// 	motor.writeMicroseconds (ESC_SIG_US_ZERO);
			// 	Serial.println (ESC_SIG_US_ZERO);
			// }

		// MAIN DELAY
		uint32_t t2 = micros();
		uint32_t temp_time;
		if ((t2 - t1) < (MAIN_CYCLE_DELAY * 1000)) {
			temp_time = (MAIN_CYCLE_DELAY * 1000) - (t2 - t1);
			while (temp_time) {
				if (temp_time > 10000) {
					delayMicroseconds (10000);
					temp_time -= 10000;
				} else if (temp_time > 1000) {
					delayMicroseconds (1000);
					temp_time -= 1000;
				} else {
					delayMicroseconds (temp_time);
					temp_time = 0;
				}
			}
		} else {
//			#if SEND_DEBUG_INFO_AUTO
				Serial.println ("[ ERROR ] MAIN DELAY BIGGER THAN CONFIGURED!!!");
//			#endif /* SEND_DEBUG_INFO_AUTO */
		}
		
		if (STATUS_LASER != 0b00011111) {
			Serial.print ("[ ERROR ] STATUS LASER!!!");
			Serial.print ("\t in: ");
			for (i = 0; i < NUM_OF_SENSORS; i++) {
				if ((STATUS_LASER & (1 << i)) == 0) {
					Serial.print (i); 	Serial.print (" ");
				}
			}
			Serial.println ();
		}

		// digitalWrite (LED_PROCESS_PIN, !digitalRead(LED_PROCESS_PIN));	
		sbi (PINB, 5);									// Reverse output value. Toggle pin.

		#if SEND_DEBUG_INFO_AUTO
			uint32_t t3 = micros();
			Serial.print ("Micros t2-t1: ");	Serial.print (t2-t1);	Serial.println (" us"); 
			Serial.print ("Micros t3-t1: ");	Serial.print (t3-t1);	Serial.println (" us"); 
			Serial.print ("Freq: "); 	Serial.print(1000000/(t3-t1));	Serial.println(" Hz");
			Serial.print ("\n");
		#endif /* SEND_DEBUG_INFO_AUTO */

	#endif /* RELEASE_AUTO */

// 	#if RELEASE_MANUAL
// 		// Process input from Bluetooth.
// 		// For 'Bluetooth RC Car' application on Android.
// 		static volatile float RC_motor_speed;

// 		if (Serial.available () > 0) {
// 			UART_request = Serial.read ();
			
// 			if (UART_request == 'F') {							// Forward.
// 				servo.writeMicroseconds (SERVO_0_US);
// 				motor.writeMicroseconds (ESC_SIG_US_FORW_MIN + ((ESC_SIG_US_FORW_MAX - ESC_SIG_US_FORW_MIN) * RC_motor_speed));
// 			} else if (UART_request == 'B') {					// Back.
// 				servo.writeMicroseconds (SERVO_0_US);
// 				motor.writeMicroseconds (ESC_SIG_US_BACK_MIN - ((ESC_SIG_US_BACK_MIN - ESC_SIG_US_BACK_MAX) * RC_motor_speed));
// 			} else if (UART_request == 'L') {					// Left.
// 				servo.writeMicroseconds (SERVO_LEFT_US);
// 				motor.writeMicroseconds (ESC_SIG_US_ZERO);
// 			} else if (UART_request == 'R') {					// Right.
// 				servo.writeMicroseconds (SERVO_RIGHT_US);
// 				motor.writeMicroseconds (ESC_SIG_US_ZERO);
// 			} else if (UART_request == 'G') {					// Forward Left.
// 				servo.writeMicroseconds (SERVO_LEFT_US);
// 				motor.writeMicroseconds (ESC_SIG_US_FORW_MIN + ((ESC_SIG_US_FORW_MAX - ESC_SIG_US_FORW_MIN) * RC_motor_speed));
// 			} else if (UART_request == 'I') {					// Forward Right.
// 				servo.writeMicroseconds (SERVO_RIGHT_US);
// 				motor.writeMicroseconds (ESC_SIG_US_FORW_MIN + ((ESC_SIG_US_FORW_MAX - ESC_SIG_US_FORW_MIN) * RC_motor_speed));
// 			} else if (UART_request == 'H') {					// Back Left.
// 				servo.writeMicroseconds (SERVO_LEFT_US);
// 				motor.writeMicroseconds (ESC_SIG_US_BACK_MIN - ((ESC_SIG_US_BACK_MIN - ESC_SIG_US_BACK_MAX) * RC_motor_speed));
// 			} else if (UART_request == 'J') {					// Back Right.
// 				servo.writeMicroseconds (SERVO_RIGHT_US);
// 				motor.writeMicroseconds (ESC_SIG_US_BACK_MIN - ((ESC_SIG_US_BACK_MIN - ESC_SIG_US_BACK_MAX) * RC_motor_speed));
// 			} else if (UART_request == 'S') {					// Stop.
// 				servo.writeMicroseconds (SERVO_0_US);
// 				motor.writeMicroseconds (ESC_SIG_US_ZERO);
// 			} else {
// 				servo.writeMicroseconds (SERVO_0_US);
// 				motor.writeMicroseconds (ESC_SIG_US_ZERO);
// 			}
			
// 			if (UART_request == '0')	{ RC_motor_speed = 0.0; }		// Speed   0 %.
// 			if (UART_request == '1')	{ RC_motor_speed = 0.1; }		// Speed  10 %.
// 			if (UART_request == '2')	{ RC_motor_speed = 0.2; }		// Speed  20 %.
// 			if (UART_request == '3')	{ RC_motor_speed = 0.3; }		// Speed  30 %.
// 			if (UART_request == '4')	{ RC_motor_speed = 0.4; }		// Speed  40 %.
// 			if (UART_request == '5')	{ RC_motor_speed = 0.5; }		// Speed  50 %.
// 			if (UART_request == '6')	{ RC_motor_speed = 0.6; }		// Speed  60 %.
// 			if (UART_request == '7')	{ RC_motor_speed = 0.7; }		// Speed  70 %.
// 			if (UART_request == '8')	{ RC_motor_speed = 0.8; }		// Speed  80 %.
// 			if (UART_request == '9')	{ RC_motor_speed = 0.9; }		// Speed  90 %.
// 			if (UART_request == 'q')	{ RC_motor_speed = 1.0; }		// Speed 100 %.
// 		}
	
// 	#endif /* RELEASE_MANUAL */


// 	#if DEBUG_MOTOR
// 		// motor.writeMicroseconds (1200);

// 		for (i = 1000; i < 2000; i++) {
// 			motor.writeMicroseconds (i);
// 			delay (10);
// 			Serial.println (i);
// 		}
// 		for (i = 2000; i > 1000; i--) {
// 			motor.writeMicroseconds (i);
// 			delay (10);
// 			Serial.println (i);
// 		}
// 		for (i = 1000; i < 2000; i++) {
// 			motor.writeMicroseconds (i);
// 			delay (5);
// 			Serial.println (i);
// 		}
// 		for (i = 2000; i > 1000; i--) {
// 			motor.writeMicroseconds (i);
// 			delay (5);
// 			Serial.println (i);
// 		}
// 		for (i = 1000; i < 2000; i++) {
// 			motor.writeMicroseconds (i);
// 			delay (3);
// 			Serial.println (i);
// 		}
// 		for (i = 2000; i > 1000; i--) {
// 			motor.writeMicroseconds (i);
// 			delay (3);
// 			Serial.println (i);
// 		}
// 		for (i = 1000; i < 2000; i++) {
// 			motor.writeMicroseconds (i);
// 			delay (2);
// 			Serial.println (i);
// 		}
// 		for (i = 2000; i > 1000; i--) {
// 			motor.writeMicroseconds (i);
// 			delay (2);
// 			Serial.println (i);
// 		}
// 		for (i = 1000; i < 2000; i++) {
// 			motor.writeMicroseconds (i);
// 			delay (1);
// 			Serial.println (i);
// 		}
// 		for (i = 2000; i > 1000; i--) {
// 			motor.writeMicroseconds (i);
// 			delay (1);
// 			Serial.println (i);
// 		}
		
// 		sbi (PINB, 5);
		
// 		// motor.writeMicroseconds (1100);
// 		// Serial.println (1100);
// 		// delay (2000);
// 		// motor.writeMicroseconds (1200);
// 		// Serial.println (1200);
// 		// delay (5000);
// 		// motor.writeMicroseconds (1300);
// 		// Serial.println (1300);
// 		// delay (5000);
// 		// motor.writeMicroseconds (1400);
// 		// Serial.println (1400);
// 		// delay (5000);
// 		// motor.writeMicroseconds (1500);
// 		// Serial.println (1500);
// 		// delay (5000);

// 		delay (5000);
// 	#endif /* DEBUG_MOTOR */

// 	#if DEBUG_SERVO
// 		// for (i = 1000; i < 2000; i++) {
// 		// 	servo.writeMicroseconds (i);
// 		// 	Serial.println (i);
// 		// 	delay (2);
// 		// }
// 		// for (i = 2000; i > 1000; i--) {
// 		// 	servo.writeMicroseconds (i);
// 		// 	Serial.println (i);
// 		// 	delay (2);
// 		// }

// 		servo.writeMicroseconds (SERVO_LEFT_US);
// 		Serial.println (SERVO_LEFT_US);
// 		delay (2000);
// 		servo.writeMicroseconds (SERVO_0_US);
// 		Serial.println (SERVO_0_US);
// 		delay (2000);
// 		servo.writeMicroseconds (SERVO_RIGHT_US);
// 		Serial.println (SERVO_RIGHT_US);
// 		delay (2000);
// 		servo.writeMicroseconds (SERVO_0_US);
// 		Serial.println (SERVO_0_US);
// 		delay (2000);
// 	#endif /* DEBUG_SERVO */
	
// 	#if DEBUG_LASERS
// 		I2CMuxChSelect (0);		laser_left.read ();		laser_data[0] = laser_left.ranging_data.range_mm;
// 		I2CMuxChSelect (1);		laser_left45.read ();	laser_data[1] = laser_left45.ranging_data.range_mm;
// 		I2CMuxChSelect (2);		laser_0.read ();		laser_data[2] = laser_0.ranging_data.range_mm;
// 		I2CMuxChSelect (3);		laser_right.read ();	laser_data[3] = laser_right.ranging_data.range_mm;
// 		I2CMuxChSelect (4);		laser_right45.read ();	laser_data[4] = laser_right45.ranging_data.range_mm;
		
// 		Serial.print ("Laser range:\t");
// 		for (i = 0; i < NUM_OF_SENSORS; i++) {
// 			if (!(laser_data[i]))	{ STATUS_LASER &= ~(1 << i); }
// 			else 					{ STATUS_LASER |= (1 << i); }
// 			Serial.print (i);
// 			Serial.print (": ");
// 			Serial.print (laser_data[i]);
// 			Serial.print ("\t");
// 		}
// 		Serial.print ("\n");
// 	#endif /* DEBUG_LASERS */

// 	#if DEBUG_MPU6050
// 		sensors_event_t a, g, temp;
// 		mpu.getEvent (&a, &g, &temp);
		
// 		Serial.print(a.acceleration.x);
// 		Serial.print(",");
// 		Serial.print(a.acceleration.y);
// 		Serial.print(",");
// 		Serial.print(a.acceleration.z);
// 		Serial.print(", ");
// 		Serial.print(g.gyro.x);
// 		Serial.print(",");
// 		Serial.print(g.gyro.y);
// 		Serial.print(",");
// 		Serial.print(g.gyro.z);
// 		Serial.println("");

// 		delay (1000);
// 	#endif /* DEBUG_MPU6050 */
	
// 	#if DEBUG_MOTOR_LASER_0
// 		I2CMuxChSelect (2);	laser_0.read ();	laser_data[2] = laser_0.ranging_data.range_mm;

// 		Serial.print ("Distance: ");	Serial.println (laser_data[LASER_0_ID]);

// 		if (laser_data[LASER_0_ID] <= 1000)
// 			motor.writeMicroseconds (ESC_SIG_US_ZERO - 50);
// 		else
// 			motor.writeMicroseconds (ESC_SIG_US_ZERO + 50);
// 	#endif /* DEBUG_MOTOR_LASER_0 */
	
// 	#if DEBUG_MOTOR_STICK
// 		if (Serial.available () > 0) {
// 			UART_request = Serial.read ();
// 			if (UART_request == 'F') {						// Forward.
// 				if (i + 10 > 2000) {
// 					i = 2000;
// 				} else {
// 					i += 10;
// 				}
// 				Serial.println (i);
// 				motor.writeMicroseconds (i);
// 				delay (20);
// 			} else if (UART_request == 'B') {				// Back.
// 				if (i - 10 < 1000) {
// 					i = 1000;
// 				} else {
// 					i -= 10;
// 				}
// 				Serial.println (i);
// 				motor.writeMicroseconds (i);
// 				delay (20);
// 			}
// 		}

// 	#endif /* DEBUG_MOTOR_STICK */
}
