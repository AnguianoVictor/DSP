/** @file   main.c
    @author Gastón SALAZAR <gaston_salazar@yahoo.com>

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



/*
    struct G{
        int a;
        char b;
    };

    struct G g;                     instancia de una estrucutra
    g.a = 1;                        Accesando a la variable a de la estructura G
    g->a =1;                         Puntero a la estructura

*/
#include "stm32f4xx_conf.h"

#define DISPLAY GPIOB->ODR              /*Apunta al registro ODR del GPIO B         */
#define DELAY   1000000UL               /**/

// Values for activating LEDs on a 7-segments display with common anode
//  DP G F E D C B A

//0x0f = 15             16 elementos.
typedef enum {
       CERO =     0xc0,                             /*bin = 11000000*/
       UNO =      0xf9,
       DOS =      0xa4,
       TRES =     0xb0,
       CUATRO =   0x99,
       CINCO =    0x92,
       SEIS =     0x82,
       SIETE =    0xf8,
       OCHO =     0x80,
       NUEVE =    0x90,
       CA =       0x88,
       CB =       0x83,
       CC =       0xc6,
       CD =       0xa1,
       CE =       0x86,
       CF =       0x8e,
       Ch =       0x8b,                                /*10001011*/
       Co =       0xa3,                                /*10100011*/
       CL =       0xc7                               /*11000111*/
} digits;

//Memoria asocitiva: segmentos[1] = UNO         segmentos[0] = CERO
//const unsigned short int segmentos[] = {CERO, UNO, DOS, TRES, CUATRO, CINCO,
//                                        SEIS, SIETE, OCHO, NUEVE,
//                                        CA, CB, CC, CD, CE, CF};

const unsigned short int hoLA[] = {Ch,Co,CL,CA};
void setup()
{
    // Configuración de máquina
    //
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    GPIOB->MODER &= ~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1 | GPIO_MODER_MODER2 | GPIO_MODER_MODER3
                    | GPIO_MODER_MODER4 | GPIO_MODER_MODER5 | GPIO_MODER_MODER6);
    GPIOB->MODER |=  (GPIO_MODER_MODER0_0  | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0
                    | GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0);

    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR0  | GPIO_PUPDR_PUPDR1 | GPIO_PUPDR_PUPDR2 | GPIO_PUPDR_PUPDR3
                    | GPIO_PUPDR_PUPDR4 | GPIO_PUPDR_PUPDR5 | GPIO_PUPDR_PUPDR6);

    // Inicialización de variables

    DISPLAY = Ch;
}

void loop(void)
{
    static short unsigned int display_count = 0;
    static long unsigned int delay_count = DELAY;

    if (!delay_count)
    {
        DISPLAY = hoLA[display_count];
        display_count++;
        display_count &= 0x05;              /*0x04 = 1111              Direccionamiento circular       Pool Select-> Sistemas asincronos*/
        delay_count = DELAY;
    }
    delay_count--;
}

int main(void)
{
    setup();
    while(1)
    {
        loop();
    }
}
