//
//  HeartMonitor.h
//  final
//
//  Created by Sam Vercauteren on 4/19/15.
//
//

#include <avr/io.h>
#include "Serial.h"

#define FOSC 9830400		// Clock frequency
#define BAUD 9600              // Baud rate used by the LCD
#define MYUBRR FOSC/16/BAUD-1

#ifndef __final__HeartMonitor__
#define __final__HeartMonitor__

int HeartMonitorReadValue();

#endif /* defined(__final__HeartMonitor__) */
