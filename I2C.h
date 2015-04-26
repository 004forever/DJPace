

#ifndef __final__I2C__
#define __final__I2C__


#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "Serial.h"

#define FOSC 9830400            // Clock frequency = Oscillator freq.

#define MYUBRR FOSC/16/BAUD-1   // Value for UBRR0 register
#define BDIV (FOSC / 100000 - 16) / 2 + 1    // Puts I2C rate just below 100kHz
#define ADDR_TOUCH 0x70//38
#define FT6206_REG_THRESHHOLD 0x80

void i2c_init(uint8_t);
uint8_t i2c_io(uint8_t, uint8_t *, uint16_t,
               uint8_t *, uint16_t, uint8_t *, uint16_t);
unsigned char getPoint(uint16_t *x, uint16_t *y);



#endif /* defined(__final__I2C__) */
