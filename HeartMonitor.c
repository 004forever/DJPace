//
//  HeartMonitor.c
//  final
//
//  Created by Sam Vercauteren on 4/19/15.
//
//

#include "HeartMonitor.h"

int HeartMonitorReadValue()
{
    uint8_t x;
    ADMUX |= (0<<MUX0);//3
    ADMUX &= (0xf0 | (0<<MUX0));//3
    sci_init();
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