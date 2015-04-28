//
//  LCD.c
//  final
//
//  Created by Sam Vercauteren on 4/19/15.
//
//

#include "LCD.h"

uint8_t red[8] = { 0, 0xff, 0, 0, 0xff, 0, 0xff, 0xff };
uint8_t grn[8] = { 0, 0, 0xff, 0, 0xff, 0xff, 0, 0xff };
uint8_t blu[8] = { 0, 0, 0, 0xff, 0, 0xff, 0xff, 0xff };

int isWhite(int x, int y, int screen)
{
    x -= 160;
    y -= 120;
    //selection
    if(screen == 0)
    {
        //border
        if(x < -150 || x > 150 || y < -110 || y > 110)
            return 1;
        if(x > -5 && x < 5)
            return 1;
        //H
        if(y > -90 && y < 90 && x < -25 && x > -35)
            return 1;
        if(y > -90 && y < 90 && x > -130 && x < -120)
            return 1;
        if(x < -30 && x > -125 && y < 5 && y > -5)
            return 1;
        //P
        if(y > -90 && y < 90 && x > 25 && x < 35)
            return 1;
        if(x  > 30 && x < 125 && y < 5 && y > -5)
            return 1;
        if(x  > 30 && x < 125 && y < 90 && y > 80)
            return 1;
        if(x  > 115 && x < 125 && y < 90 && y > 4)
            return 1;
        return 0;
    }
    //play
    else if(screen == 1)
    {
        //disc
        if(x*x+y*y >10000 && x*x+y*y < 12000)
            return 1;
        //play
        if(x > -40 && y > x/2-30 && y < -x/2+30)
            return 1;
        //arrow
        if(x > -140 && x < -100 && y < 100 && y > 90)
            return 1;
        if(x < -120 && x/2+170 >y && y > -x/2+20)
            return 1;
        return 0;
    }
    //pause
    else if(screen == 2)
    {
        //disc
        if(x*x+y*y >10000 && x*x+y*y < 12000)
            return 1;
        //pause
        if(y > -80 && y < 80 && x < 40 && x > 20)
            return 1;
        if(y > -80 && y < 80 && x > -40 && x < -20)
            return 1;
        //arrow
        if(x > -140 && x < -100 && y < 100 && y > 90)
            return 1;
        if(x < -120 && x/2+170 >y && y > -x/2+20)
            return 1;
        return 0;
    }
    else
    {
        int value = screen;
        int digits[3];
        int i;
        digits[2] = value % 10;
        digits[1] = (value/10)%10;
        digits[0] = value/100;
        for(i = -1;i < 2;i++)
        {
            //middle
            if(x > -40+i*320/3 && x < 40+i*320/3 && y > -5 && y < 5
               && (digits[i+1] == 2 || digits[i+1] == 3 || digits[i+1] == 4 || digits[i+1] == 5 || digits[i+1] == 6
                   ||digits[i+1] == 8 || digits[i+1] == 9))
                return 1;
            //bottom
            if(x > -40+i*320/3 && x < 40+i*320/3 && y > -240/3-5 && y < -240/3+5
               && (digits[i+1] == 0 || digits[i+1] == 2 || digits[i+1] == 3 || digits[i+1] == 5 || digits[i+1] == 6
                   ||digits[i+1] == 8 || digits[i+1] == 9))
                return 1;
            //top
            if(x > -40+i*320/3 && x < 40+i*320/3 && y > 240/3-5 && y < 240/3+5
               && (digits[i+1] == 0 || digits[i+1] == 2 || digits[i+1] == 3 || digits[i+1] == 5 || digits[i+1] == 6
                   ||digits[i+1] == 7 || digits[i+1] == 8 || digits[i+1] == 9))
                return 1;
            //top left
            if(x > -40+i*320/3 && x < -30+i*320/3 && y > 4 && y < 240/3-4
               && (digits[i+1] == 0 || digits[i+1] == 4 || digits[i+1] == 5 || digits[i+1] == 6
                   ||digits[i+1] == 8 || digits[i+1] == 9))
                return 1;
            //top right
            if(x > 30+i*320/3 && x < 40+i*320/3 && y > 4 && y < 240/3-4
               && (digits[i+1] == 0 || digits[i+1] == 1 || digits[i+1] == 2 || digits[i+1] == 3 || digits[i+1] == 4
                   ||digits[i+1] == 7 || digits[i+1] == 8 || digits[i+1] == 9))
                return 1;
            //bottom left
            if(x > -40+i*320/3 && x < -30+i*320/3 && y > -240/3-4 && y < -4
               && (digits[i+1] == 0 || digits[i+1] == 2 || digits[i+1] == 6 || digits[i+1] == 8))
                return 1;
            //bottom right
            if(x > 30+i*320/3 && x < 40+i*320/3 && y > -240/3-4 && y < -4
               && (digits[i+1] == 0 || digits[i+1] == 1 || digits[i+1] == 3 || digits[i+1] == 4 || digits[i+1] == 5
                   ||digits[i+1] == 6 || digits[i+1] == 7 || digits[i+1] == 8 || digits[i+1] == 9))
                return 1;
        }
        return 0;
    }
    return 0;
}

void writecommand (unsigned char c)
{
    //DC pin goes low-->command
    PORTB &= ~(1 << PB1);
    //set CS Slave active(low)
    PORTB &= ~(1 << PB2);
    //_delay_ms(50);
    spiwrite(c);
    
    // turn CS Slave off(high)
    PORTB |= (1 << PB2);
    //_delay_ms(50);
}

void writedata (unsigned char c)
{
    //DC pin goes high-->data
    PORTB |= 1 << DDB1;
    //set CS Slave active(low)
    PORTB &= ~(1 << PB2);
    spiwrite(c);
    //sci_num(c);
    // turn CS Slave off(high)
    PORTB |= (1 << PB2);
}

void spiwrite(unsigned char c)
{
    //unsigned char temp_backup = SPCR;
    
    SPDR = c;
    //Wait until transmission complete
    while(!(SPSR & (1<<SPIF)));
    
    //SPCR = temp_backup;
    
}

void spi_init_master(void)
{
    int x, y;
            sci_init();
    DDRB |= (1 << DDB2); // set SS as output
    PORTB |= (1<< PB2); // set SS as 1
    
    DDRB |= (1 << DDB5); // set SCK as output
    DDRB |= (1 << DDB3); // set MOSI as output
    
    PORTB &= ~(1 << PB5);// set SCK low
    PORTB &= ~(1 << PB3);// set MOSI low
    
    SPCR |= (1 << MSTR);//set device as Master
    
    SPCR &= ~(1<<SPR0);
    SPCR &= ~(1<<SPR1);
    SPSR |= (1<<SPI2X);
    SPCR &= ~(1 << DORD); // Sends MSB first
    
    SPCR |= (1 << SPE);// Enable SPI
    
    //_dc set to output
    DDRB |= (1 << DDB1);    // D/C
       // sci_num(0x01);
}

void init_lcdd()//follows Professor Weber's code
{
           // sci_num(0x02);
    writecommand(ILI9341_SWRESET);
    //writedata(0x00);
    _delay_ms(50);
    writecommand(ILI9341_DISPOFF);
    //writedata(0x00);
    
    writecommand(ILI9341_PWCTR1);
    writedata(0x23);
    
    writecommand(ILI9341_PWCTR2);
    writedata(0x10);
    
    writecommand(ILI9341_VMCTR1);
    writedata(0x2b);
    writedata(0x2b);
    
    writecommand(ILI9341_VMCTR2);
    writedata(0xc0);
    
    writecommand(ILI9341_MADCTL);
    writedata(ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR);
    
    writecommand(ILI9341_PIXFMT);
    writedata(0x55);
    
    writecommand(ILI9341_FRMCTR1);
    writedata(0x00);
    writedata(0x1b);
    
    writecommand(ILI9341_ENTRYMODE);
    writedata(0x07);
    
    writecommand(ILI9341_SLPOUT);
    //writedata(0x00);
    _delay_ms(150);
    writecommand(ILI9341_DISPON);
    //writedata();
    _delay_ms(500);
           // sci_num(0x03);
}


/*void SPI_MasterInit(void)
{

    //Set MOSI and SCK output, all others input
    DDRB |= (1<<DDB3)|(1<<DDB5);
    //Enable SPI, Master, set clock rate fck/16
    SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR0);
    PRR |= (0<<PRSPI);
    DDRB |= (1 << DDB2);

}
*/

unsigned char SPI_MasterTransmit(char cData)
{
    /* Start transmission */
    SPDR = cData;
    /* Wait for transmission complete */
    while(!(SPSR & (1<<SPIF)));
    return SPDR;
}

void color_bars()
{
    //write code later
    unsigned char i,j,k;
    unsigned int color;
    unsigned int y1, y2;
    unsigned char hi, lo;
    
    for (j = 0; j < 8; j++) {
        y1 = j * 40;
        y2 = y1 + 39;
        //set window to 240x40 strip from [0,y1] to [239, y2]
        //sci_num(0x04);
        setAddrWindow(0,y1,239, y2);
        //sci_num(0x05);
        //LCD_CS_ACTIVE; //LCD_CS_Active
        //LCD_DC_COMMAND;
        writecommand(ILI9341_RAMWR);
        
        color = color565(red[j],grn[j], blu[j]);
        //color = color565(0,0,0);
        hi = color >> 8;
        
        lo = color;
        for (k = 0; k < 40; k++) {      // 40 rows in each bar
           for (i = 0; i < 240; i++) { // 240 pixels in each row
                writedata(hi);
                writedata(lo);
           }
        }
        
        
    }
}

void display_bitmap(int screen)
{
    unsigned int color;
    unsigned char hi, lo;
    unsigned int x, y;
    uint8_t mask;
    for(x = 0;x < 240;x++)
    {
        for(y = 0;y < 320;y++)
        {
            setAddrWindow(x, y, x+1, y+1);
            writecommand(ILI9341_RAMWR);
            if(isWhite(y, x, screen))
            {
                color = color565(0xff, 0xff, 0xff);
            }
            else
            {
                color = color565(0,0,0);
            }
            hi = color >> 8;
            lo = color;
            writedata(hi);
            writedata(lo);
            
            
        }
    }
}


void setAddrWindow(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
    unsigned long int w;
    
    w = ((unsigned long int) x1) << 16;
    w |= x2;
    writecommand(ILI9341_CASET);
    writedata(w >> 24);
    writedata(w >> 16);
    writedata(w >> 8);

    writedata(w);
    
    w = (unsigned long int) y1 << 16;
    w |= y2;
    writecommand(ILI9341_PASET);
    writedata(w >> 24);
    writedata(w >> 16);
    writedata(w >> 8);
    writedata(w);
    
    
}

unsigned int color565(unsigned char r, unsigned char g, unsigned char b)
{
    return ((r & 0xf8) << 8) | ((g & 0xfc) << 3) | (b >> 3);
}