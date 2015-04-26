#include "Accelerometer.h"

int AccelerometerReadValue()
{
    uint8_t x;
    ADMUX |= (1<<MUX0);//3
    ADMUX &= (0xf0 | (1<<MUX0));//3
    
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    x = ADCH;
    sci_num(x);
    if(x == 0xFF)
    {
        return 0;
    }
    return 1;
    
}