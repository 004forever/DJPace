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
    DDRB |= 1 << DDB1;
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
            sci_init();
    DDRB |= (1 << DDB2); // set SS as output
    PORTB |= (1<< PB2); // set SS as 1
    
    DDRB |= (1 << DDB5); // set SCK as output
    DDRB |= (1 << DDB3); // set MOSI as output
    
    PORTB &= ~(1 << PB5);// set SCK low
    PORTB &= ~(1 << PB3);// set MOSI low
    
    SPCR |= (1 << MSTR);//set device as Master
    
    SPCR |= (1<<SPR0)|(1<<SPR1);
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
        setAddrWindow(0,y1,ILI9341_TFTWIDTH-1, y2);
        //sci_num(0x05);
        //LCD_CS_ACTIVE; //LCD_CS_Active
        //LCD_DC_COMMAND;
        writecommand(ILI9341_RAMWR);
        
        color = color565(red[j],grn[j], blu[j]);
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