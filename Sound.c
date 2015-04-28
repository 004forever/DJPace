////pins 4 and 5 of the sound breakout is going to PD2 and PD3.


#import "Sound.h"

uint16_t audio_address;
uint16_t volume;




void init_audio()
{
    DDRB |= 1 << PB0;          // Set PORTB bit 0 for output for clock
    DDRD |= 1 << PD7;          // Set PORTD bit 7 for output for data
    DDRD |= 1 << PD6;          // Set PORTD bit 6 for output for reset
    
    PORTB |= 1 << PB0;         // Set clock output to high
    PORTD |= 1 << PD7;         // Set data output to high
    PORTD |= 1 << PD6;         // Set reset output to high
    
    PORTD &= ~(1<< PD6);
    _delay_ms(5);
    PORTD |= 1 << PD6;
}

void audio_reset()
{
    PORTD &= ~(1<< PD6);
    _delay_ms(5);
    PORTD |= 1 << PD6;
}




void send_audio_data(uint16_t audio_data)
{
    /*First lower clock for 2ms and send first bit  */
    /*lowering the clock   */
    PORTB &= ~(1 << PB0);
    /*get first bit (MSB) of data  */
    uint16_t msb = audio_data;
    msb = msb >> 15;
    /*set the data line to MSB  */
    if(msb)
    {
        PORTD |= 1 << PD7;
    }
    else
    {
        PORTD &= ~(1 << PD7);
    }
    /* clock lowered for 2ms  */
    _delay_ms(2);
    /* clock raised for 200us  */
    PORTB |= 1 << PB0;
    _delay_us(200);
    
    /*Next loop for the next 15 bits of data  */
    int8_t count = 14;
    while (count >= 0)
    {
        /* lower clock  */
        PORTB &= ~(1 << PB0);
        /* get the next significant bit of data  */
        uint16_t next_bit = audio_data;
        next_bit = next_bit >> count;
        next_bit &= 0x01;
        /*set the data line to next bit of data  */
        if(next_bit)
        {
            PORTD |= 1 << PD7;
        }
        else
        {
            PORTD &= ~(1 << PD7);
        }
        /* clock lowered for 200us  */
        _delay_us(200);
        /* clock raised for 200us  */
        PORTB |= 1 << PB0;
        _delay_us(200);
        count--;			
    }
    /* reset clock and data lines to high  */
    PORTD |= 1 << PB0;         // Set clock output to high
    PORTD |= 1 << PD7;         // Set data output to high
    
}



