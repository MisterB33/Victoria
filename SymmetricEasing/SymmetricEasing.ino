/*
 * SymmetricEasing.cpp
 *
 *  Shows symmetric (end movement is mirror of start movement) linear, quadratic and cubic movements for 3 servos synchronously.
 *
 *  Copyright (C) 2019-2021  Armin Joachimsmeyer
 *  armin.joachimsmeyer@gmail.com
 *
 *  This file is part of ServoEasing https://github.com/ArminJo/ServoEasing.
 *
 *  ServoEasing is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/gpl.html>.
 */

#include <Arduino.h>

/*
 * To generate the Arduino plotter output, you must activate the line #define PRINT_FOR_SERIAL_PLOTTER in ServoEasing.h
 */
#include "ServoEasing.h"

#include "PinDefinitionsAndMore.h"
/*
 * Pin mapping table for different platforms
 *
 * Platform     Servo1      Servo2      Servo3      Analog
 * -------------------------------------------------------
 * AVR + SAMD   9           10          11          A0
 * ESP8266      14 // D5    12 // D6    13 // D7    0
 * ESP32        5           18          19          A0
 * BluePill     PB7         PB8         PB9         PA0
 * APOLLO3      11          12          13          A3
 */

ServoEasing Servo1;
ServoEasing Servo2;
ServoEasing Servo3;
ServoEasing Servo4; 

#define START_DEGREE_VALUE 90
#define PRINT_FOR_SERIAL_PLOTTER
int a = 0;
int b = 90; 
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

    /*
     * Check at least the last call to attach()
     */
#ifndef PRINT_FOR_SERIAL_PLOTTER
    Serial.print(F("Attach servo at pin "));
    Serial.println(SERVO3_PIN);
#endif
    if (Servo3.attach(SERVO3_PIN, START_DEGREE_VALUE, DEFAULT_MICROSECONDS_FOR_0_DEGREE,
    DEFAULT_MICROSECONDS_FOR_180_DEGREE) == INVALID_SERVO) {
        Serial.println(F("Error attaching servo"));
        while (true) {
            digitalWrite(LED_BUILTIN, HIGH);
            delay(100);
            digitalWrite(LED_BUILTIN, LOW);
            delay(100);
        }
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
    Servo1.startEaseToD(45, 1000);
    Servo2.startEaseToD(45, 1000);
    Servo3.startEaseToD(45, 1000);
    Servo4.startEaseToD(45, 1000);    
//delay(1000);

    Servo1.setEasingType(EASE_SINE_IN_OUT);
    Servo2.setEasingType(EASE_SINE_IN_OUT);
    Servo3.setEasingType(EASE_SINE_IN_OUT);
    Servo4.setEasingType(EASE_SINE_IN_OUT); 

//    Servo1.setEasingType(EASE_QUADRATIC_IN_OUT);
//    Servo2.setEasingType(EASE_CUBIC_IN_OUT);
//    Servo3.setEasingType(EASE_SINE_IN_OUT);
//    Serial.println("Quadratic, Cubic, Sine");
    delay(1000);
}

void loop() {

    uint16_t tSpeed = analogRead(SPEED_IN_PIN);
#if defined(__STM32F1__)
    tSpeed = map(tSpeed, 0, 4096, 5, 150); // 12 bit ADC
#else
    tSpeed = map(tSpeed, 0, 1023, 5, 150);
#endif
    setSpeedForAllServos(70);

    /*
     * Move three servos synchronously without interrupt handler
     */
#ifndef PRINT_FOR_SERIAL_PLOTTER
    Serial.print(F("Move to 135 degree with "));
    Serial.print(Servo1.getSpeed());
    Serial.println(F(" degree per second with with updates by own do-while loop"));
#endif
    /*
     * Here we use the allServos functions
     */
    //setDegreeForAllServos(4,180,180,90, 180);
    //setEaseToForAllServos();
    //synchronizeAllServosAndStartInterrupt(); // false, since we call updateAllServos() manually below
//delay(1000);
    //setDegreeForAllServos(4,90,170,90, 90);
    //setEaseToForAllServos();
    //synchronizeAllServosAndStartInterrupt(); // false, since we call updateAllServos() manually below


    //setDegreeForAllServos(4,135,135,90, 135);
    //setEaseToForAllServos();
    //synchronizeAllServosAndStartInterrupt(); // false, since we call updateAllServos() manually below
   

    setDegreeForAllServos(4,b,a,90,90);
    setEaseToForAllServos();
    synchronizeAllServosAndStartInterrupt(); // false, since we call updateAllServos() manually below
   do {
       if(Serial.available()>0){; // here you can call your own program
        String  c = Serial.readString();
        if(c == "w"){;
                //Serial.println(c);
                a = a+10;
        }
        if(c == "s"){
                a = a-10;
        }
        if(c == "a"){
                b = b+10;
        }
        if(c == "d"){
                b  = b-10;  
        }
       }
        delay(REFRESH_INTERVAL / 1000); // optional 20ms delay - REFRESH_INTERVAL is in Microseconds
    } while (!updateAllServos());

    /*
     * Move three servos synchronously with interrupt handler
     */
//delay(1000);
#ifndef PRINT_FOR_SERIAL_PLOTTER
    Serial.print(F("Move to 45 degree with "));
    Serial.print(Servo1.getSpeed());
    Serial.println(F(" degree per second using interrupts"));
#endif
/*    Servo1.setEaseToD(10,1200);
    Servo2.setEaseToD(10, Servo1.mMillisForCompleteMove);
    Servo3.startEaseToD(100, Servo1.mMillisForCompleteMove);
    Servo4.startEaseToD(10, Servo1.mMillisForCompleteMove);
    delay(1000);

    Servo1.setEaseTo(90);
    Servo2.startEaseToD(90, Servo1.mMillisForCompleteMove);
    Servo3.startEaseToD(90, Servo1.mMillisForCompleteMove);
    Servo4.startEaseToD(90, Servo1.mMillisForCompleteMove);
    delay(1000);

    Servo1.setEaseTo(135);
    Servo2.startEaseToD(135, Servo1.mMillisForCompleteMove);
    Servo3.startEaseToD(90, Servo1.mMillisForCompleteMove);
    Servo4.startEaseToD(135, Servo1.mMillisForCompleteMove);
    delay(1000);

    Servo1.setEaseToD(120,1000);
    Servo2.startEaseToD(90, Servo1.mMillisForCompleteMove);
    Servo3.startEaseToD(90, Servo1.mMillisForCompleteMove);
    Servo4.startEaseToD(90, Servo1.mMillisForCompleteMove);
    delay(1000);


    Servo1.setEaseTo(90);
    Servo2.startEaseToD(90, Servo1.mMillisForCompleteMove);
    Servo3.startEaseToD(130, Servo1.mMillisForCompleteMove);
    Servo4.startEaseToD(90, Servo1.mMillisForCompleteMove);
    delay(1000);


    Servo1.setEaseToD(90,1000);
    Servo2.startEaseToD(90, Servo1.mMillisForCompleteMove);
    Servo3.startEaseToD(50, Servo1.mMillisForCompleteMove);
    Servo4.startEaseToD(90, Servo1.mMillisForCompleteMove);
    delay(1000);

//    Servo3.setEaseTo(180);
//    Servo2.setEaseToD(90, Servo3.mMillisForCompleteMove);
//    Servo1.startEaseToD(90, Servo3.mMillisForCompleteMove);
//    Servo4.startEaseToD(90, Servo3.mMillisForCompleteMove);
//    delay(1000);

  //  Servo3.setEaseTo(10);
//    Servo2.setEaseToD(90, Servo3.mMillisForCompleteMove);
//    Servo1.startEaseToD(90, Servo3.mMillisForCompleteMove);
//    Servo4.startEaseToD(90, Servo3.mMillisForCompleteMove);
//    delay(1000);
//    Servo1.setEaseTo(30);
//    delay(1000);
//    Servo1.setEaseTo(130); 
//    delay(1000);

//    Servo2.setEaseToD(20,Servo1.mMillisForCompleteMove);
//    Servo2.setEaseToD(70,Servo1.mMillisForCompleteMove); 
    /*
     * No need to call synchronizeAllServosAndStartInterrupt(), since I know that all durations are the same
     * Since all servos stops at the same time I have to check only one
     * Must call yield here for the ESP boards, since we do not call delay in the loop
     */
    while (ServoEasing::areInterruptsActive()) {
        ; // no delays here to avoid break between forth and back movement
    }

}
