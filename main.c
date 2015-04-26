#include <avr/io.h>
#include <util/delay.h>
#include "InputInterrupts.h"
#include "LCD.h"
#include "Sound.h"
#include "Serial.h"
#include "I2C.h"

#define FOSC 9830400		// Clock frequency
#define BAUD 9600              // Baud rate used by the LCD
#define MYUBRR FOSC/16/BAUD-1



int main(void)
{

    
    // if touch interrupt is high, get the status and value of x,y coordinates
    
    /*while(1)
    {
    status=i2c_io(testAddress, &abuf, 1, &threshold, 1, NULL, 0);
    if(status != 0x20)
    {
            sci_num(testAddress);
        sci_num(status);
    }
        testAddress++;
    }*/
    
    /*sci_init();
    init_audio();

    uint16_t file = 0;
    while(file < 0x08)
    {
        sci_num(file);
        init_audio();
    send_audio_data(file);
        _delay_ms(100);
    send_audio_data(PLAY_PAUSE);
    _delay_ms(10000);
        file++;
    }*/
    
    //while(1);
    
    //return 0;
    
    //return 0;
    //spi_init_master();
    
    //int x, y;
    

    
    //init_lcdd();
    //while(1)
    //{
        //display_bitmap();
        //color_bars();
    //}
    _delay_ms(4000);
    InputInterruptsInit();
    while(1);
    return 0;
}




