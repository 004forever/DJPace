

#include <avr/io.h>
#include "Serial.h"

#define FOSC 9830400		// Clock frequency
#define BAUD 9600              // Baud rate used by the LCD
#define MYUBRR FOSC/16/BAUD-1

#ifndef __final__Accelerometer__
#define __final__Accelerometer__

int AccelerometerReadValue();

#endif 