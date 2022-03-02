/*******************************************************************************
 *  @name		Roborace
 *  @file		main.cpp
 *
 *  @author		Uladzislau 'vladubase' Dubatouka
 *				<vladubase@gmail.com>.
 * 				https://github.com/vladubase/Roborace
 *  @version	V0.1
 *  @date		2-March-2022
 ******************************************************************************/

#include "main.h"


// GLOBAL VARIABLES
uint32_t	i = 0, j = 0;
uint8_t		STATUS_LASER = 0x00;
uint8_t		STATUS_BLUETOOTH = 0x00;
uint8_t		STATUS_MPU6050 = 0x00;
uint16_t	laser_data[6] = {0};
uint8_t		bluetooth_data = 0;


/************************************ Main ************************************/

void setup () {
	// MICROCONTROLLER INITIALIZATION.
		pinMode (LED_BUILTIN, OUTPUT);

	// BLUETOOTH INITIALIZATION
		Serial.begin (UART_BAUD);
		Serial.print ("Bluetooth");
		while (!Serial);
		Serial.setTimeout (UART_TIMEOUT);				// Время ожидания данных.
		STATUS_BLUETOOTH |= 1 << 7;						// Bluetooth - OK!
		Serial.println (" - OK!");
	
	// ESC INITIALIZATION.
		Serial.write ("ESC");
		ESC.attach (ESC_PIN);
		ESC.writeMicroseconds (ESC_MIN_SIG_US);	delay (2000);
		ESC.writeMicroseconds (ESC_MAX_SIG_US);	delay (1000);
		Serial.write (" - OK! \n");
	
	// SERVO INITIALIZATION.
		Serial.print ("SERVO");
		servo.attach (SERVO_PIN);
		servo.writeMicroseconds (SERVO_0_US);			// Смотрим прямо.
		Serial.println (" - OK!");

	// LASER SENSORS
		Serial.print ("LASER");
		Wire.begin ();
		Wire.setClock (I2C_SPEED);						// Use 400 kHz I2C.
		
		I2CMuxChSelect (0);	laser_left.setTimeout 	(I2C_TIMEOUT);
		I2CMuxChSelect (1);	laser_left45.setTimeout (I2C_TIMEOUT);
		I2CMuxChSelect (2);	laser_0.setTimeout 		(I2C_TIMEOUT);
		I2CMuxChSelect (3);	laser_right45.setTimeout(I2C_TIMEOUT);
		I2CMuxChSelect (4);	laser_right.setTimeout 	(I2C_TIMEOUT);
		I2CMuxChSelect (5);	laser_back.setTimeout 	(I2C_TIMEOUT);

		// if bit not set - LASER init error.
		I2CMuxChSelect (0);	if (laser_left.init ())		{ STATUS_LASER |= (1 << 0); }
		I2CMuxChSelect (1);	if (laser_left45.init ())	{ STATUS_LASER |= (1 << 1); }
		I2CMuxChSelect (2);	if (laser_0.init ())		{ STATUS_LASER |= (1 << 2); }
		I2CMuxChSelect (3);	if (laser_right45.init ())	{ STATUS_LASER |= (1 << 3); }
		I2CMuxChSelect (4);	if (laser_right.init ())	{ STATUS_LASER |= (1 << 4); }
		I2CMuxChSelect (5);	if (laser_back.init ())		{ STATUS_LASER |= (1 << 5); }

		if (STATUS_LASER == 0b00111111)					{ STATUS_LASER |= (1 << 7);	}
		Serial.println (" - OK!");

		// Distance:	Long, Medium, Short
		// Period (ms):  50     33     20
		// 50 ms.
		I2CMuxChSelect (0);	laser_left.setDistanceMode (VL53L1X::Short);
		I2CMuxChSelect (0);	laser_left.setMeasurementTimingBudget (20000);
		I2CMuxChSelect (0);	laser_left.startContinuous (20);
		
		I2CMuxChSelect (1);	laser_left45.setDistanceMode (VL53L1X::Medium);
		I2CMuxChSelect (1);	laser_left45.setMeasurementTimingBudget (33000);
		I2CMuxChSelect (1);	laser_left45.startContinuous (33);
		
		I2CMuxChSelect (2);	laser_0.setDistanceMode (VL53L1X::Long);
		I2CMuxChSelect (2);	laser_0.setMeasurementTimingBudget (50000);
		I2CMuxChSelect (2);	laser_0.startContinuous (50);
		
		I2CMuxChSelect (3);	laser_right45.setDistanceMode (VL53L1X::Medium);
		I2CMuxChSelect (3);	laser_right45.setMeasurementTimingBudget (33000);
		I2CMuxChSelect (3);	laser_right45.startContinuous (33);
		
		I2CMuxChSelect (4);	laser_right.setDistanceMode (VL53L1X::Short);
		I2CMuxChSelect (4);	laser_right.setMeasurementTimingBudget (20000);
		I2CMuxChSelect (4);	laser_right.startContinuous (20);
		
		I2CMuxChSelect (5);	laser_back.setDistanceMode (VL53L1X::Long);
		I2CMuxChSelect (5);	laser_back.setMeasurementTimingBudget (50000);
		I2CMuxChSelect (5);	laser_back.startContinuous (50);

	// MPU6050
		Serial.print ("MPU6050");
		I2CMuxChSelect (7);	while (!mpu.begin ());
		STATUS_MPU6050 |= (1 << 7);
		
		I2CMuxChSelect (7);	mpu.setAccelerometerRange (MPU6050_RANGE_16_G);
		I2CMuxChSelect (7);	mpu.setGyroRange (MPU6050_RANGE_2000_DEG);
		I2CMuxChSelect (7);	mpu.setFilterBandwidth (MPU6050_BAND_44_HZ);
		Serial.println (" - OK!");
	
	// DEBUG INFO
		Serial.print ("STATUS BLUETOOTH: ");
		Serial.println (STATUS_BLUETOOTH);
		Serial.print ("STATUS LASER: ");
		Serial.println (STATUS_LASER);
		Serial.print ("STATUS MPU6050: ");
		Serial.println (STATUS_MPU6050);
}

void loop () {
	// MAIN CYCLE
	#if RELEASE
   		long int t1 = millis();
		
		// READ LASERS
			I2CMuxChSelect (0);	laser_data[0] = laser_left.read ();
			I2CMuxChSelect (1);	laser_data[1] = laser_left45.read ();
			I2CMuxChSelect (2);	laser_data[2] = laser_0.read ();
			I2CMuxChSelect (3);	laser_data[3] = laser_right45.read ();
			I2CMuxChSelect (4);	laser_data[4] = laser_right.read ();
			I2CMuxChSelect (5);	laser_data[5] = laser_back.read ();
			
			Serial.print ("Laser range:\t");
			for (i = 0; i < 6; i++) {
				Serial.print (i);
				Serial.print (": ");
				Serial.print (laser_data[i]);
				Serial.print ("\t");
			}
			Serial.print ("\n");

		// READ MPU6050
			sensors_event_t a, g, temp;
			I2CMuxChSelect (7);	mpu.getEvent (&a, &g, &temp);
			
			Serial.print("Acceleration X: ");
			Serial.print(a.acceleration.x);
			Serial.print(", Y: ");
			Serial.print(a.acceleration.y);
			Serial.print(", Z: ");
			Serial.print(a.acceleration.z);
			Serial.println(" m/s^2");

			Serial.print("Rotation X: ");
			Serial.print(g.gyro.x);
			Serial.print(", Y: ");
			Serial.print(g.gyro.y);
			Serial.print(", Z: ");
			Serial.print(g.gyro.z);
			Serial.println(" rad/s");

			Serial.print("Temperature: ");
			Serial.print(temp.temperature);
			Serial.println(" degC");
		
		long int t2 = millis();
		Serial.print ("Freq: "); Serial.print(1000/(t2-t1)); Serial.println(" Hz");
		Serial.print ("\n");
		delay (500);

	#endif /* RELEASE */



	
	#if DEBUG_MOTOR
		for (i = 1100; i < 1500; i++) {
			ESC.writeMicroseconds (i);
			delay (50);
			digitalWrite (LED_BUILTIN, HIGH);
		}
		ESC.writeMicroseconds (ESC_MIN_SIG_US);
		digitalWrite(LED_BUILTIN, LOW);
		
		delay (5000);
	#endif /* DEBUG_MOTOR */

	#if DEBUG_BLUETOOTH
		if (Serial.available ()) {
			Serial.write ("STATUS_LASER: ");
			Serial.write (STATUS_LASER);
			Serial.write ('\n');
			delay (1000);
		}
	#endif
	
	#if DEBUG_LASERS
		I2CMuxChSelect (0);
		laser_left.read ();		laser_data[0] = laser_left.ranging_data.range_mm;
		// I2CMuxChSelect (1);
		// laser_left45.read ();	laser_data[1] = laser_left45.ranging_data.range_mm;
		// I2CMuxChSelect (2);
		// laser_0.read ();		laser_data[2] = laser_0.ranging_data.range_mm;
		// I2CMuxChSelect (3);
		// laser_right.read ();	laser_data[3] = laser_right.ranging_data.range_mm;
		// I2CMuxChSelect (4);
		// laser_right45.read ();	laser_data[4] = laser_right45.ranging_data.range_mm;
	#endif /* DEBUG_LASERS */

	#if DEBUG_MPU6050
		sensors_event_t a, g, temp;
		mpu.getEvent (&a, &g, &temp);
		
		Serial.print(a.acceleration.x);
		Serial.print(",");
		Serial.print(a.acceleration.y);
		Serial.print(",");
		Serial.print(a.acceleration.z);
		Serial.print(", ");
		Serial.print(g.gyro.x);
		Serial.print(",");
		Serial.print(g.gyro.y);
		Serial.print(",");
		Serial.print(g.gyro.z);
		Serial.println("");

  		delay (1000);
	#endif
}
