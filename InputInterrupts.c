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
int first;

int beat[10];
int beatIndex;

uint8_t currentSong;

int currentScreen;

void InputInterruptsInit()
{
    spi_init_master();
    init_lcdd();
    currentScreen = 0;
    currentSong = 0x08;

    init_audio();
    _delay_ms(10);
    
    beatIndex = 0;
    
    moveToMenu();
    
    ADMUX |= (1<<REFS0);
    ADMUX &=~(1<<REFS1);
    ADMUX |= (1<<ADLAR);
    ADCSRA |= (7<<ADPS0);
    ADCSRA |= (1<<ADEN);
    
    currentI = 0;
    high = 0;
    first = 0;
    
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

void moveToPause()
{
    init_audio();
    _delay_ms(10);
    send_audio_data(currentSong);
        send_audio_data(PLAY_PAUSE);
    currentScreen = 2;
    display_bitmap(currentScreen);
}

void moveToPlay()
{
    send_audio_data(STOP);
    currentScreen = 1;
    display_bitmap(currentScreen);
}

void moveToMenu()
{
    send_audio_data(STOP);
    currentScreen = 0;
    display_bitmap(currentScreen);
}

void calculateHeart()
{
    uint16_t x, y;
    int calc;
    int i;
    if(HeartMonitorReadValue())
    {
        if(!high)
        {
            calc = 384*60/count;
            beat[beatIndex] = calc;
            beatIndex++;
            if(beatIndex > 9)
                beatIndex = 0;
            calc = 0;
            for(i = 0;i < 10;i++)
            {
                calc += beat[i];
            }
            calc/=10;
            //display_bitmap(calc);
            sci_num(calc);
            /*if(calc < 80 && currentSong != 0x08)
            {
                sci_num(0x08);
                currentSong = 0x08;
                if(currentScreen == 2)
                {
                    init_audio();
                    send_audio_data(currentSong);
                    _delay_ms(100);
                    send_audio_data(PLAY_PAUSE);
                }
            }
            if(calc >= 80 && calc < 120 && currentSong != 0x09)
            {
                sci_num(0x08);
                currentSong = 0x09;
                if(currentScreen == 2)
                {
                    init_audio();
                    send_audio_data(currentSong);
                    _delay_ms(100);
                    send_audio_data(PLAY_PAUSE);
                }
            }
            if(calc >= 120 && currentSong != 0x0A)
            {
                sci_num(0x08);
                currentSong = 0x0A;
                if(currentScreen == 2)
                {
                    init_audio();
                    send_audio_data(currentSong);
                    _delay_ms(100);
                    send_audio_data(PLAY_PAUSE);
                }
            }*/
            //sci_num(calc);
            high = 1;
            count = 0;
        }
        else
            count++;
    }
    else
    {
        high = 0;
        count++;
    }

    if(!(PINC &(1<<PC3)))
    {
        getPoint(&x,&y);    //get the x and y point when screen is pressed
        //sci_num(0xFF);
        //sci_num(x);
        //sci_num(y);
        switch (currentScreen) {
            case 0:   //main screen, choosing between h and p
            {
                if (x > 0x00A0) {       // right side of screen
                    moveToPause();
                }
                else{
                    moveToPause();  // left side of screen
                }
                break;
            }
            case 1:                   // music playing screen
            {
                if ((x>0x0037) & (x<0x00A3) & (y>0x003D) & (y<0x00DF)) {                 // paused button pressed, x> 120, x< 200
                    moveToPause();
                }
                else if((x>0x00A3) & (y<0x003D)){              // back button pressed, assuming bottom left is origin
                    moveToMenu();
                }
                else{                    // pressed in area that doesnt matter
                }
                
                break;
            }
            case 2:                   // music pause screen
            {

                if ((x>0x0037) & (x<0x00A3) & (y>0x003D) & (y<0x00DF)) {                 // play button pressed
                    moveToPlay();
                }
                else if((x>0x00A3) & (y<0x003D)){              //back button pressed
                    moveToMenu();
                }
                else
                {
                }
                break;
            }
            default:
                break;
        }
    }
}

ISR(TIMER1_COMPA_vect)
{
    calculateHeart();
}