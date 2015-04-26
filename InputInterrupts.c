//
//  InputInterrupts.c
//  final
//
//  Created by Sam Vercauteren on 4/19/15.
//
//

#include "InputInterrupts.h"

int times[10];

int currentI;
int high;
int count;

int currentScreen;

void InputInterruptsInit()
{
    spi_init_master();
    init_lcdd();
    currentScreen = 0;
    display_bitmap(currentScreen);
    
    ADMUX |= (1<<REFS0);
    ADMUX &=~(1<<REFS1);
    ADMUX |= (1<<ADLAR);
    ADCSRA |= (7<<ADPS0);
    ADCSRA |= (1<<ADEN);
    
    currentI = 0;
    high = 0;
    
    unsigned char threshold  = 40;
    unsigned char status;
    unsigned char abuf = 128;
    DDRC &= ~(1 << DDC3);   // configure PC3 for touch interrupt
    PORTC |= (1 << PC3);//x and y coordinate of touch
    i2c_init(BDIV);
    // Set threshold of touch sensitivity of touch panel
    status=i2c_io(ADDR_TOUCH, &abuf, 1, &threshold, 1, NULL, 0);

    
    //set up interrupts
    TCCR1B |= (1 << WGM12);     // Set for CTC mode.  OCR1A = modulus
    TIMSK1 |= (1 << OCIE1A);    // Enable CTC interrupt
    sei();                      // Enable global interrupts
    OCR1A = 100;              // Set the counter modulus
    TCCR1B |= (1 << CS12);      // Set prescaler for divide by 256,
}

void calculateHeart()
{
        uint16_t x, y;
    sci_init();
    if(HeartMonitorReadValue())
    {
        if(!high)
        {
            high = 1;
            count = 0;
            sci_num(0x11);
        }
        else
            count++;
    }
    else
    {
        high = 0;
    }

    if(!(PINC &(1<<PC3)))
    {
        display_bitmap(currentScreen);
        currentScreen++;
        if(currentScreen > 2)
            currentScreen = 0;
    }
}

ISR(TIMER1_COMPA_vect)
{
    calculateHeart();
}