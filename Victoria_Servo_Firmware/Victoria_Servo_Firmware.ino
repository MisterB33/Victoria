#include <Arduino.h>
#include <string.h>
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
int ANGL_VAL[4];
char COMMAND;
int COUNTER = 0;
String MSG = "";

char BUF[11];

void DoCommand(char command, String msg){
        switch( command ){
                case 'w':
                SANGLE1 = SANGLE1+ANG_INC; 
                break ;
                case 's':
                SANGLE1 = SANGLE1-ANG_INC;
                break ;
                case 'a':
                SANGLE2 = SANGLE2+ANG_INC; 
                break ;
                case 'd':
                SANGLE2 = SANGLE2-ANG_INC;
                break ;
                case 'e':
                SANGLE3 = SANGLE3+ANG_INC; 
                break ;
                case 't':
                SANGLE3 = SANGLE3-ANG_INC;
                break ;
                case 'o':
                SANGLE4 =180; 
                break ;
                case 'c':
                SANGLE4 = 90;
                break ;
                case 'p':
                SANGLE1 = ANGL_VAL[0];
                SANGLE2 = ANGL_VAL[1];
                SANGLE3 = ANGL_VAL[2];
                SANGLE4 = ANGL_VAL[3];
                break;
                case 'S':
                setSpeedForAllServos(ANGL_VAL[0]);
                break;
                case 'A':
                ANG_INC = ANGL_VAL[0];
                break;
                case 'R':
                sprintf(BUF,"p %d,%d,%d,%d,;",SANGLE1,SANGLE2,SANGLE3,SANGLE4);
                Serial.println(BUF); 
                break;
                case 'W':
                // insert the wait command option
                break;
        }
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
    
    setSpeedForAllServos(100);

//   synchronizeAllServosAndStartInterrupt(true); // false, since we call updateAllServos() manually below
}

void loop() {
    /*
     * Move three servos synchronously without interrupt handler
     */
    /*
     * Here we use the allServos functions
     */
       while(Serial.available()){ // here you can call your own program`
        char  c = Serial.read();
        if(MSG_REC==1){
        COMMAND = c;
        MSG_REC = 0;
        continue;
        }
        if( c == ';'){
                DoCommand(COMMAND,MSG);
        do{
        setDegreeForAllServos(4,SANGLE1,SANGLE2,SANGLE3,SANGLE4);
        setEaseToForAllServos();
        //updateAllServos();
        delay(50);
        }while(!updateAllServos());
          MSG = "";
          MSG_REC = 1; 
          COUNTER = 0;
        }else{
                if(c != ' '){
                        MSG.concat(c);
                }
                if(c == ','){
                        ANGL_VAL[COUNTER] = atoi(MSG.c_str());
                        MSG = "";
                        COUNTER = COUNTER +1;
                }
        }
}
}
