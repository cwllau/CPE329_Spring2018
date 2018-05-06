//*****************************************************************************
//
// MSP432 main.c template - Empty main
//
//****************************************************************************

#include "msp.h"
#include <stdio.h>

void main(void)
{
	
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer

    P6->SEL0 &= ~BIT0;
    P6->SEL1 &= ~BIT0;
    P6->DIR |= BIT0;
    P6->OUT &= ~BIT0;

    ADC14->CTL0 = 0x10;
    ADC14->CTL0 |= 0x4080300;
    ADC14->CTL1 = 0x20;
    ADC14->MCTL[5] = 0;
    P5->SEL1 |= 0x20;
    P5->SEL0 |= 0x20;
    ADC14->CTL1|= 0x00050000;
    ADC14->CTL0 |= 2;


    int counter;
    while (1) {
        ADC14->CTL0 |= 1;
        while(!ADC14->IFGR0);
        int result = ADC14->MEM[5];
        printf("%d\n", result);
        if (result < 2000) {
            P6->OUT |= BIT0;
            for (counter = 0; counter < 10000; counter++);
            P6->OUT &= ~BIT0;
        }
    }
}
