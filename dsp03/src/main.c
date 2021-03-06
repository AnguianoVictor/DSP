/** @file   main.c
    @author Gast�n SALAZAR <gaston_salazar@yahoo.com>
    Editado por: Victor Ramirez Anguiano.
# Interface #

## Physical connections ##

Port    Segment Color
----    ------- -----
PB0     A       blue
PB1     B       red
PB2     C       yellow
PB3     D       green
PB4     E       orange
PB5     F       violet
PB6     G       gray

## LEDS on a 7-segment Display ##

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

#include "stm32f4xx_conf.h"
#include <stdbool.h>
#define DISPLAY GPIOB->ODR
#define BUTTON  (GPIOC->IDR & GPIO_IDR_IDR_13)
#define LED     GPIOA->ODR
#define DELAY   1000000UL

// Values for activating LEDs on a 7-segments display with common anode
typedef enum {
       CERO =     0xc0,
       UNO =      0xf9,
       DOS =      0xa4,
       TRES =     0xb0,
       CUATRO =   0x99,
       CINCO =    0x92,
       SEIS =     0x82,
       SIETE =    0xf8,
       OCHO =     0x80,
       NUEVE =    0x90
} digits;
typedef enum{
       CA =       0x88,
       CB =       0x83,
       CC =       0xc6,
       CD =       0xa1,
       CE =       0x86,
       CF =       0x8e,
       CG =       0x90
} letter;

const unsigned short int segmentos[] = {CERO, UNO, DOS, TRES, CUATRO, CINCO,
                                        SEIS, SIETE, OCHO, NUEVE};
const unsigned short int letters[] = {CA,CB,CC,CD,CE,CF,CG};

void setup()
{
    // Configuraci�n de m�quina

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOB->MODER &= ~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1 | GPIO_MODER_MODER2 | GPIO_MODER_MODER3
                    | GPIO_MODER_MODER4 | GPIO_MODER_MODER5 | GPIO_MODER_MODER6);
    GPIOB->MODER |=  (GPIO_MODER_MODER0_0  | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0
                    | GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0);

    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR0  | GPIO_PUPDR_PUPDR1 | GPIO_PUPDR_PUPDR2 | GPIO_PUPDR_PUPDR3
                    | GPIO_PUPDR_PUPDR4 | GPIO_PUPDR_PUPDR5 | GPIO_PUPDR_PUPDR6);

    GPIOC->MODER &= ~GPIO_MODER_MODER13;
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR13;

    GPIOA->MODER &= ~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1 | GPIO_MODER_MODER2 | GPIO_MODER_MODER3
                    | GPIO_MODER_MODER4 | GPIO_MODER_MODER5 | GPIO_MODER_MODER6);
    GPIOA->MODER |=  (GPIO_MODER_MODER0_0  | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0
                    | GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0);
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR0  | GPIO_PUPDR_PUPDR1 | GPIO_PUPDR_PUPDR2 | GPIO_PUPDR_PUPDR3
                    | GPIO_PUPDR_PUPDR4 | GPIO_PUPDR_PUPDR5 | GPIO_PUPDR_PUPDR6);


    // Inicializaci�n de variables

    DISPLAY = CERO;
    LED = 0b11111111;
}

void loop(void)
{
    static short unsigned int num_count = 0;
    static short unsigned int let_count = 0;
    static long unsigned int delay_count = DELAY;
    bool flag = true;

    if (BUTTON)
        {
            if (!delay_count)
            {
                let_count = 0;
                DISPLAY = segmentos[num_count];
                num_count++;
                num_count &= 0x07;
                delay_count = DELAY;
            }
                delay_count--;
            }
        else
        {
            if (!delay_count)
            {
                num_count=0;
                DISPLAY = letters[let_count];
                let_count++;
                let_count &= 0x07;
                delay_count = DELAY;
            }
            delay_count--;
        }
    }

int main(void)
{
    setup();
    while(1)
    {
        loop();
    }
}
