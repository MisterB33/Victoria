#include <Arduino.h>
/*
 * To generate the Arduino plotter output, you must activate the line #define PRINT_FOR_SERIAL_PLOTTER in ServoEasing.h
 */
#include "ServoEasing.h"

#include "PinDefinitionsAndMore.h"
/*
 * Platform     Servo1      Servo2      Servo3      Servo4
 * AVR + SAMD   9           10          11          6
 */

//Initializging Servoeasing class
ServoEasing Servo1;
ServoEasing Servo2;
ServoEasing Servo3;
ServoEasing Servo4; 

#define START_DEGREE_VALUE 90
#define PRINT_FOR_SERIAL_PLOTTER

//Setting the inciment angles
//Important variables 
int SANGLE1 = 90;
int SANGLE2 = 90;
int SANGLE3 = 90; 
int SANGLE4 = 90;
int ANG_INC = 30; //default increment angle is set to 10 degrees
int MSG_REC = 1;
char COMMAND;
String MSG = "";
void CommandParser(char command, String msg){
}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
#if defined(__AVR_ATmega32U4__) || defined(SERIAL_USB) || defined(SERIAL_PORT_USBVIRTUAL)  || defined(ARDUINO_attiny3217)
    delay(2000); // To be able to connect Serial monitor after reset or power up and before first printout
#endif
    // Just to know which program is running on my Arduino
#ifndef PRINT_FOR_SERIAL_PLOTTER
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_SERVO_EASING));
#endif

#if defined(ESP32)
    analogReadResolution(10);
#endif

    /************************************************************
     * Attach servo to pin and set servos to start position.
     * This is the position where the movement starts.
     *
     * The order of the attach() determine the position
     * of the Servos in internal ServoEasing::ServoEasingArray[]
     ***********************************************************/

#ifndef PRINT_FOR_SERIAL_PLOTTER
    Serial.print(F("Attach servo at pin "));
    Serial.println(SERVO1_PIN);
#endif
    if (Servo1.attach(SERVO1_PIN, START_DEGREE_VALUE, DEFAULT_MICROSECONDS_FOR_0_DEGREE,
    DEFAULT_MICROSECONDS_FOR_180_DEGREE) == INVALID_SERVO) {
        Serial.println(F("Error attaching servo"));
    }

#ifndef PRINT_FOR_SERIAL_PLOTTER
    Serial.print(F("Attach servo at pin "));
    Serial.println(SERVO2_PIN);
#endif
    if (Servo2.attach(SERVO2_PIN, START_DEGREE_VALUE, DEFAULT_MICROSECONDS_FOR_0_DEGREE,
    DEFAULT_MICROSECONDS_FOR_180_DEGREE) == INVALID_SERVO) {
        Serial.println(F("Error attaching servo"));
    }
    
#ifndef PRINT_FOR_SERIAL_PLOTTER
    Serial.print(F("Attach servo at pin "));
    Serial.println(SERVO3_PIN);
#endif
// while statemaent will have the lights blinking. 
    if (Servo3.attach(SERVO3_PIN, START_DEGREE_VALUE, DEFAULT_MICROSECONDS_FOR_0_DEGREE,
    DEFAULT_MICROSECONDS_FOR_180_DEGREE) == INVALID_SERVO) {
        Serial.println(F("Error attaching servo"));
    } 
    if (Servo4.attach(SERVO4_PIN, START_DEGREE_VALUE, DEFAULT_MICROSECONDS_FOR_0_DEGREE, DEFAULT_MICROSECONDS_FOR_180_DEGREE) == INVALID_SERVO){
        Serial.println(F("Error attaching servo"));
	while(true){
	    digitalWrite(LED_BUILTIN, HIGH);
	    delay(100); 
            digitalWrite(LED_BUILTIN,LOW); 
	    delay(100); 
	}
    }

    // Wait for servos to reach start position.
    delay(2000);

#ifndef PRINT_FOR_SERIAL_PLOTTER
    // Legend for Arduino Serial plotter
    Serial.println();
    Serial.println("Linear, Quadratic, Cubic");
#endif

#ifndef PRINT_FOR_SERIAL_PLOTTER
    Serial.println(F("Move from 90 to 45 degree in 1 second"));
#endif
//delay(1000);

    Servo1.setEasingType(EASE_SINE_IN_OUT);
    Servo2.setEasingType(EASE_SINE_IN_OUT);
    Servo3.setEasingType(EASE_SINE_IN_OUT);
    Servo4.setEasingType(EASE_SINE_IN_OUT); 
    
    Servo1.startEaseToD(SANGLE1, 200);
    Servo2.startEaseToD(SANGLE2, 200);
    Servo3.startEaseToD(SANGLE3, 200);
    Servo4.startEaseToD(SANGLE4, 200);    
    delay(1000);
    Serial.flush();
//    Serial.read();

//   setEaseToForAllServos();
//   synchronizeAllServosAndStartInterrupt(true); // false, since we call updateAllServos() manually below
}

void loop() {

//    uint16_t tSpeed = analogRead(SPEED_IN_PIN);
//#if defined(__STM32F1__)
//    tSpeed = map(tSpeed, 0, 4096, 5, 150); // 12 bit ADC
//#else
//    tSpeed = map(tSpeed, 0, 1023, 5, 150);
//#endif
    setSpeedForAllServos(100);

    /*
     * Move three servos synchronously without interrupt handler
     */
    /*
     * Here we use the allServos functions
     */
//   updateAllServos();   
//   setEaseToForAllServos();
//    synchronizeAllServosAndStartInterrupt(true); // false, since we call updateAllServos() manually below
//   do {
       while(Serial.available()){ // here you can call your own program`
        char  c = Serial.read();
        if(MSG_REC==1){
//        c = Serial.read(); 
        COMMAND = c;
        //Serial.println(COMMAND);
        MSG_REC = 0;
        continue;
        }
        //c = Serial.read();

        Serial.println(c);
        Serial.println(COMMAND);
        if( c = ';'){
                if(COMMAND == 'w'){
                        SANGLE2 = SANGLE2-ANG_INC;
                        Serial.println("you got here");
                }if(COMMAND =='s'){
                        SANGLE2 = SANGLE2+ANG_INC;
                }
        do{
        setDegreeForAllServos(4,SANGLE1,SANGLE2,SANGLE3,SANGLE4);
        setEaseToForAllServos();
        updateAllServos();
        delay(50);
        Serial.println("updating...");
        }while((!updateAllServos()));
        Serial.println("updated!");
        delay(100);
          MSG = "";
          MSG_REC = 1; 
        }else{
        MSG.concat(c);
        }
//        setDegreeForAllServos(4,SANGLE1,SANGLE2,SANGLE3,SANGLE4);
//        delay(50); 
}
}
