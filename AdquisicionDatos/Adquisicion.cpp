
#include "mbed.h"

Serial      serial(USBTX, USBRX);
InterruptIn button(USER_BUTTON);
AnalogIn    analog(A0);

volatile signed long Count;
double adc;
float periodos[4]={1.0,5.0,10.0,16.6,};

void incremento()
{
    Count++;
    Count &= 0x03;
}

void setup()
{
    button.rise(&incremento);
    Count = 0;
    serial.baud(9600);
    serial.format(8, SerialBase::None, 1);
}

void
loop()
{
    adc = analog.read(); 
    adc = adc * 5.0;
    serial.printf("%3f\r\n", adc);
    wait_ms(periodos[Count]);
}

int
main()
{
    setup();
    while(1)
        loop();
}
