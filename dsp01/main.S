/** @file   main.S
    @author Gastón SALAZAR  <gaston_salazar@yahoo.com>
    @modified by Victor Ramirez Anguiano

    @brief  A blink program for the STM32F446re. It implements a state machine.
            It requires a basic link script. The interrupts vectors must be
            defined.

    @modification: Add two delays, one for high state and another for low state

    This example is used as a didactical resource.
*/
    .syntax unified
    .arch   armv7e-m
    .cpu    cortex-m4
    .thumb

/**
    Interface

    @def    LED
    @brief  The bit address of pin 5 on port A.

    @def    DELAY
    @brief  The number of loops to execute before changing th state of LED.
*/
    .equ    LED,            (1 << 5)
    .equ    DELAYOFF,        200000
    .equ    DELAYON,         1000000

@ Register for enabling the AHB1 clock for GPIOA
    .equ    RCC_AHB1ENR,    0x40023830
    .equ    GPIOAEN,        (1 << 0)

@ Base address of port-A registers
    .equ    GPIOA,          0x40020000

@ Offsets of the registers for port X, using as base the port address
    .equ    GPIO_MODER,     0x00
    .equ    GPIO_PUPDR,     0x0c
    .equ    GPIO_ODR,       0x14

@ Bit addresses in the port registers, in order to configure the behaviour of the output pin
    .equ    RESET_MODER5,   (3 << 10)
    .equ    SET_MODER5,     (1 << 10)
    .equ    RESET_PUPDR5,   (3 << 10)

/**
    @fn     _start
    @brief  After the reset the fuction _start is the entry point.
*/
    .text
    .global _start
    .type   _start,   %function
_start:
/*
    Machine setup
*/
    @ Enable port A
    ldr r0, =RCC_AHB1ENR    @ r0 = RCC_AHB1ENR
    ldr r1, [r0]            @ r1 = *r0
    orr r1, r1, GPIOAEN     @ r1 = r1 | GPIOAEN
    str r1, [r0]            @ *r0 = r1

    @ A minimal initialization for PA5 as output (LED) on the STM32F446re
    ldr r4, =GPIOA

    ldr r1, [r4, GPIO_MODER]    @ r1 = r4[GPIO_MODER]
    bic r1, r1, RESET_MODER5    @ r1 = r1 & ~RESET_MODER5
    orr r1, r1, SET_MODER5
    str r1, [r4, GPIO_MODER]    @ r4[GPIO_MODER] = r1

    ldr r1, [r4, GPIO_PUPDR]
    bic r1, r1, RESET_PUPDR5
    str r1, [r4, GPIO_PUPDR]

/*
    State initilization
*/
    @ LED = 0
    ldr r1, [r4, GPIO_ODR]
    bic r1, r1,LED
    str r1, [r4, GPIO_ODR]

    ldr r0, =DELAYOFF

encendido:
/*
    State update
*/
    cbnz    r0, waitON                  @ if (!r0) goto waitON    Salto condicional si NO es cero
    ldr     r1, [r4, GPIO_ODR]          @Se carga una copia del valor de r1 en la localidad r4 con offset GPIO_ODR      Se mantiene apagado el led
    //orr     r1, r1, 0xff                @ r1 = r1 | 0xff    Se realiza la operación, guardando en r1 el valor 0xff
    mov     r1, 0xff                    @El registro r1 se coloca en 0xff
    str     r1, [r4, GPIO_ODR]          @El valor r1=0xff se almacena en la localidad r4                                Se apaga el led
    ldr     r0, =DELAYON                @Se restaura el valor de r0 en 500000


apagado:
    cbnz    r0, waitOFF                 @ if (!r0) goto waitOFF    Salto condicional si NO es cero
    ldr     r1, [r4, GPIO_ODR]          @Se carga el valor de r1 en la localidad r4 con offset GPIO_ODR                 Se mantiene encendido el led.
    //and     r1, r1, 0x00                @ r1 = r1 & 0x00     Se realiza la operacion, guardando en r1 el valor 0x00
    mov     r1, 0x00                    @el registro r1 se coloca en 0x00
    str     r1, [r4, GPIO_ODR]          @El valor r1=0x00 se almacena en la localidad r4                                Se apaga el led
    ldr     r0, =DELAYOFF               @Se restaura el valor de r0 en 500000
    b       encendido                   @ goto loop

waitON:
    sub     r0, r0, 1                   @ r0 = r0 - 1
    b       encendido                   @ goto loop

waitOFF:
    sub     r0, r0, 1           @ r0 = r0 - 1
    b       apagado                @ goto loop
    bx      lr  @ end of function main, to never be reached

    .size   _start, . - _start

    .section    .int_vector_table, "a", %progbits
    .type       basic_vectors,  %object
basic_vectors:
    .word   _estack
    .word   _start
    .size   basic_vectors, . - basic_vectors
