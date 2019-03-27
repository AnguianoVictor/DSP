/** 
    @file   main.cpp
    @author Gastón SALAZAR  <gaston_salazar@yahoo.com>
    @modified Victor Ramírez. 
    @brief  A small example of an interrupt application.
*/

#include "mbed.h"

const float SAMPLING_PERIOD  = 3.0;

Serial      serial(USBTX, USBRX);
InterruptIn button(USER_BUTTON);
DigitalOut  led(LED2);
InterruptIn   decr(D8);

volatile signed long interruptCount;
volatile bool flag;

void
incremento()
{
    interruptCount++;
    flag = true;
}

void decremento()
{
    interruptCount--;
    flag = false;
    }

void
setup()
{
    button.rise(&incremento);
    decr.mode(PullUp);
    decr.rise(&decremento);
    interruptCount = 0;
    serial.baud(9600);
    serial.format(8, SerialBase::None, 1);
}

void
loop()
{
    serial.printf("Frequency = %d\r\n", interruptCount);
    //interruptCount = 0;
    led.write(flag);
    wait(SAMPLING_PERIOD);
}

int
main()
{
    setup();
    while(1)
        loop();
}