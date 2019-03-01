/*  @file   main.c
**  @author Victor M. Ramirez Anguiano
**  Generador de funciones utiizando 8 bits de salida, la salida es transformada analogicamente por una red
**      R-2R junto con un amplificador operacional en modo de amplificacion positiva.
**      Las funciones generadas son: Sinusoidal, Diente de sierra y cuadrada.
**      Para la función sinusoidal se utilizo una tabla LookUp
**      Podemos ir cambiando entre las funciones ya definidas mediante la pulsacion del boton de usuario (GPIOC13)
**********************************************************************/
/*
   Last committed:     $Revision: 00
   Last changed by:    $Author: Victor Manuel Ramírez Anguiano
   Last changed date:  $Date:  28/02/2019

**********************************************************************/
#include "stm32f4xx_conf.h"
#include <stdbool.h>
#include <math.h>
#define SALIDA  GPIOB->ODR                              //Se asocia GPIOB a la variable SALIDA
#define BUTTON  (GPIOC->IDR & GPIO_IDR_IDR_13)          //Se asocia el GPIOC13 a la variable BUTTON
#define PI      3.14159265                              //Constante pi
#define f       3                                       //Variable para ajustar la frecuencia

//Definicion de la tabla de LookUp para la onda sinusoidal.
double Seno[] = {    0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
0x30,0x31,0x32,0x33,0x34,0x35,0x35,0x36,
0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3d,
0x3e,0x3f,0x40,0x41,0x41,0x42,0x43,0x44,
0x44,0x45,0x46,0x46,0x47,0x48,0x48,0x49,
0x49,0x4a,0x4a,0x4b,0x4b,0x4c,0x4c,0x4d,
0x4d,0x4d,0x4e,0x4e,0x4e,0x4f,0x4f,0x4f,
0x4f,0x4f,0x50,0x50,0x50,0x50,0x50,0x50,
0x50,0x50,0x50,0x50,0x50,0x50,0x50,0x4f,
0x4f,0x4f,0x4f,0x4f,0x4e,0x4e,0x4e,0x4d,
0x4d,0x4d,0x4c,0x4c,0x4b,0x4b,0x4a,0x4a,
0x49,0x49,0x48,0x48,0x47,0x46,0x46,0x45,
0x44,0x44,0x43,0x42,0x41,0x41,0x40,0x3f,
0x3e,0x3d,0x3d,0x3c,0x3b,0x3a,0x39,0x38,
0x37,0x36,0x35,0x35,0x34,0x33,0x32,0x31,
0x30,0x2f,0x2e,0x2d,0x2c,0x2b,0x2a,0x29,
0x28,0x27,0x26,0x25,0x24,0x23,0x22,0x21,
0x20,0x1f,0x1e,0x1d,0x1c,0x1b,0x1b,0x1a,
0x19,0x18,0x17,0x16,0x15,0x14,0x13,0x13,
0x12,0x11,0x10,0xf,0xf,0xe,0xd,0xc,
0xc,0xb,0xa,0xa,0x9,0x8,0x8,0x7,
0x7,0x6,0x6,0x5,0x5,0x4,0x4,0x3,
0x3,0x3,0x2,0x2,0x2,0x1,0x1,0x1,
0x1,0x1,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,
0x1,0x1,0x1,0x1,0x2,0x2,0x2,0x3,
0x3,0x3,0x4,0x4,0x5,0x5,0x6,0x6,
0x7,0x7,0x8,0x8,0x9,0xa,0xa,0xb,
0xc,0xc,0xd,0xe,0xf,0xf,0x10,0x11,
0x12,0x13,0x13,0x14,0x15,0x16,0x17,0x18,
0x19,0x1a,0x1b,0x1b,0x1c,0x1d,0x1e,0x1f,
0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27};

    static short unsigned int tamano = sizeof(Seno)/sizeof(Seno[0]);    //Se obtiene el tamaño de la tabla de LookUp para recorrerla mediante un for
    static long unsigned int frecuencia = f;                            //Se asigna en la variable frecuencia el valor de f
    static long signed int y = 0;                                       //La variable que portara el valor de las señales, se inicializa en 0;
    static short int index_signal = 0;                                  //Se utiliza una variable para cambiar de señal en la salida y
    bool BotonActual;                                                   //Variables auxiliares para la funcion antirrebote
    bool BotonAnterior;

void setup()
{
    //Configuracion de los GPIO's a utilizar
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOB->MODER &= ~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1 | GPIO_MODER_MODER2 | GPIO_MODER_MODER3
                    | GPIO_MODER_MODER4 | GPIO_MODER_MODER5 | GPIO_MODER_MODER6 | GPIO_MODER_MODER7);
    GPIOB->MODER |=  (GPIO_MODER_MODER0_0  | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0
                    | GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0);

    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR0  | GPIO_PUPDR_PUPDR1 | GPIO_PUPDR_PUPDR2 | GPIO_PUPDR_PUPDR3
                    | GPIO_PUPDR_PUPDR4 | GPIO_PUPDR_PUPDR5 | GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR7);

    GPIOC->MODER &= ~GPIO_MODER_MODER13;
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR13;

     SALIDA = 0x00;                                                 //El GPIOB se inicializa en 0;
}

int D2B(int decimal)                                                //Funcion para conversion de decimal a binario
{
     unsigned long int k;                                           //variable que contendra el valor en binario
     for(int c=7; c>=0; c--)                                        //El valor de 7 se elige respecto a la cantidad de bits deseados.
     {
          k = decimal >> c;
     }
     return k;
}

bool antirebote (int pin)
{
     static bool EdoAnterior;                               //Guarda el estado anterior del boton
     static bool EdoActual;                                 //Guarda el estado actual del boton.
     short int countRebote = 0;
     const int tiempoAntirebote = 10;                       //Se fija un numero de rebotes a ignorar.
     do{
          EdoActual = pin;                                   //Se guarda el valor del pin en EdoActual(False)
          if (EdoActual != EdoAnterior)                     //Si son difentes los valores son diferentes...
          {
               countRebote = 0;
               EdoAnterior = EdoActual;
          }
          else
          {
               countRebote++;
          }

     }while(countRebote < tiempoAntirebote);
     return EdoActual;

}
void loop(void)
{
     BotonActual = BUTTON;
     if(!frecuencia)
     {

          switch(index_signal)
          {
               case 0:   //Funcion SENO
                    for (int i=tamano; i>=0; i--)
                    {
                         SALIDA = Seno[i];
                    }
                    break;
               case 1:   //Funcion Diente de sierra
                    for (int i=0; i<=360; i+=5)
                    {
                         y =((i*PI/180)*PI)-1 ;
                         SALIDA = D2B(y);
                    }
                    break;
               case 2:   //Funcion cuadrada
                    for (int i=0; i<=360; i+=5)
                    {
                         float rad = (i/180*PI);
                         if (rad >= PI)
                         {
                              SALIDA = D2B(0);
                         }
                         else
                         {
                              SALIDA = D2B(1);
                         }
                    }
                    break;
               case 3:   //Funcion CERO
                    SALIDA = 0x00;
               default:
                    y=0;
                    SALIDA = 0x00;
          }
          frecuencia = f;
     }
     if(BotonActual != BotonAnterior)
     {
          if(antirebote(BUTTON) == false)
          {
               index_signal++;
               index_signal &= 0x03;
          }

     }
     frecuencia--;
     BotonAnterior = BotonActual;
}



int main(void)
{
     setup();
  while(1)
  {
     loop();
  }
}
