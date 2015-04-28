//
//  InputInterrupts.h
//  final
//
//  Created by Sam Vercauteren on 4/19/15.
//
//

#include <avr/io.h>
#include <avr/interrupt.h>
#include "HeartMonitor.h"
#include "Serial.h"
#include "Accelerometer.h"
#include "I2C.h"
#include "LCD.h"
#include "Sound.h"

#define FOSC 9830400		// Clock frequency
#define BAUD 9600              // Baud rate used by the LCD
#define MYUBRR FOSC/16/BAUD-1

#ifndef __final__InputInterrupts__
#define __final__InputInterrupts__

void InputInterruptsInit();
void calculatedHeart();

#endif /* defined(__final__HeartMonitor__) */
