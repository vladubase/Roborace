/*******************************************************************************
 *  @name     BLDC_check
 *  @file     BLDC_check.ino
 *
 *  @author   Uladzislau 'vladubase' Dubatouka
 *            <vladubase@gmail.com>.
 *  @version  V1.0
 *  @date     24-January-2022
 ******************************************************************************/
            
#include <Servo.h>

#define ESC_PIN         ((uint8_t)  9)
#define ESC_MIN_SIG     ((uint16_t) 1000)
#define ESC_MAX_SIG     ((uint16_t) 2000)
                        

/************************************ Main ************************************/

int main (void) {
    // DEFINITION OF VARIABLES
    
     
    // ESC INITIALIZATION.
    Servo ESC;
    ESC.attach (ESC_PIN, ESC_MIN_SIG, ESC_MAX_SIG);

    // MAIN CYCLE
    while (true) {
        ESC.writeMicroseconds (1100);
    }
}
