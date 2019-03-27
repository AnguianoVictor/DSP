#include "mbed.h"
#include "arm_math.h"

const float SAMPLING_PERIOD = 1.0/180.0;
volatile double SIGNAL_PERIOD   = 10.0;

PwmOut          led(LED1);
InterruptIn     button(USER_BUTTON);
InterruptIn     decr(D8);
Serial          serial(USBTX, USBRX);

void
incremento()
{
    SIGNAL_PERIOD+=0.5;
}

void decremento()
{
    SIGNAL_PERIOD-=0.5;
    if (SIGNAL_PERIOD <= 0.00)
        SIGNAL_PERIOD=0.0;
    }


void
setup()
{
    button.rise(&incremento);
    decr.mode(PullUp);
    decr.rise(&decremento);
    led.period(SAMPLING_PERIOD);
    serial.baud(9600);
    serial.format(8, SerialBase::None, 1);
}

void
loop()
{
    static unsigned long count   = 0;    
    
    float   output  = 0;
    
    output  = sin(2*PI/SIGNAL_PERIOD*SAMPLING_PERIOD*count);
    if (output <= 0)
        output = 0;
    led.write(output);
    serial.printf("%lf \r\n",output);
    count++;
    
    wait(SAMPLING_PERIOD);
}

int main()
{
    setup();
    while(1)
        loop();
}
