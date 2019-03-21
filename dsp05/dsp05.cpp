 
    @file   main.cpp
    @author Gastón SALAZAR  gaston_salazar@yahoo.com


#include mbed.h
#include stdio.h
#include string.h

const unsigned long DELAY   = 5000000UL;


Serial      serial(USBTX, USBRX);
DigitalOut  led(LED1);
char cadena[6];
int numero;
void setup()
{
    serial.baud(9600);
    serial.format(8, SerialBaseNone, 1);
    serial.baud(57600);
    serial.format(7, SerialBaseEven, 1);
    serial.printf(Hello, world!rnrn);
}

void actividad2()
{
    int val;
    if(serial.readable())
        {
            val = int(serial.getc());
            if (val == 177)
            {
                led.write(1);
                serial.printf(ONrn);
            }
            if (val == 48)
            {    
                led.write(0);
                serial.printf(OFFrn);
            }
            serial.putc(val);
            serial.printf(Printf  %drn,val);
        }
    
    static unsigned short   display_count   = 0;
    static unsigned long    delay_count     = DELAY;    

    if (delay_count == 0)
      {
        serial.printf(Display %xrn, display_count);
        display_count++;
        display_count &= 0x0F;
        delay_count = DELAY;
      }
    
    delay_count--;
    

}

void actividad3()
{
    int size;
    if(serial.readable()  0)
        {
            size = serial.getc();
            char cadena[size];
            serial.printf(Eco rn);
            for (int i=0; i=size+1; i++)
            {
                cadena[i] = serial.getc();
                serial.putc(cadena[i]);
                if(cadena[i] == '0')
                    serial.printf(Termino de Cadena);
            }
            serial.printf(rn);
        }
}

void actividad4(){
        serial.printf(rn);
        serial.gets(cadena,6);
        numero = atoi(cadena);
        serial.printf(El valor en hex de %s es 0x%xn,cadena,numero);
    }

int main()
{
    setup();
    while(1){
        actividad4();
        }
}