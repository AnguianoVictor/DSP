/** @file   main.c
    @author Gastón SALAZAR <gaston_salazar@yahoo.com>
    @modified Victor Ramirez <vramireza1401@alumno.ipn.mx>
    @brief  Display a (hexadecimal-) number sequence on a display
# Interface #
## Physical connections ##
Port    Segment Color
----    ------- -----
PC0     A       blue
PC1     B       red
PC2     C       yellow
PC3     D       green
PC4     E       orange
PC5     F       violet
PC6     G       gray
#% LEDS on a 7-segment Display ##
 AAA
F   B
F   B
F   B
 GGG
E   C
E   C
E   C
 DDD
*/

#include "mbed.h"

const unsigned long     DELAY   = 5000000UL;
const unsigned short    DISPLAY = 0x7F;

DigitalOut  led(LED1);
PortOut     myDisplay(PortC, DISPLAY);
DigitalIn   myButton(USER_BUTTON);

typedef enum {
    ZERO =   0x3F,
    ONE =    0x06,
    TWO =    0x5B,
    THREE =  0x4F,
    FOUR =   0x66,
    FIVE =   0x6D,
    SIX =    0x7D,
    SEVEN =  0x07,
    EIGHT =  0x7F,
    NINE =   0x6F,
    CA =     0x77,
    CB =     0x7C,
    CC =     0x39,
    CD =     0x5E,
    CE =     0x79,
    CF =     0x71
} digits;

// This was handle in automation, some time ago, with a cam or "drum".
// And in electronics, it was implemented with a decoder in a ROM.
// Of course you could implemented it with logic gates.
const unsigned short int segments[] = {ZERO, ONE, TWO, THREE, FOUR, FIVE,
                                       SIX, SEVEN, EIGHT, NINE,
                                       CA, CB, CC, CD, CE, CF
                                      };

void setup()
{
    myDisplay = ZERO;
}

void loop()
{
    static short unsigned   display_count   = 0;
    static long unsigned    delay_count     = DELAY;
    static bool flag = true;
    if(flag)
    {
        if (!delay_count) {
            display_count++;
            display_count &= 0x0f;
            myDisplay = segments[display_count];
            led = 0;
            delay_count = DELAY;
        }
        delay_count--;
    }else
    {
        if (!delay_count) {
            display_count--;
            display_count &= 0x0f;
            myDisplay = segments[display_count];
            led = 1;
            delay_count = DELAY;
        }
        delay_count--;
    }
    
    if(!myButton)
    {
        flag=!flag;
        }
    }
int main()
{
    setup();
    while(1) {
        loop();
    }
}
