#include <Servo.h>

Servo motor;

void setup() {
    motor.attach (9);
    
    motor.writeMicroseconds (800);
    delay (2000);
    motor.writeMicroseconds (2200);
    delay (2000);
}

void loop () {
    for (int i = 800; i < 2200; i++) {
        motor.writeMicroseconds (i);
        delay (10);
    }
}
