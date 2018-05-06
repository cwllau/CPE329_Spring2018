#include "msp.h"
#include "adc.h"

void ADC_init() {
    ADC14->CTL0 = 0x00000010; /*on, disabled during config*/
    ADC14->CTL0 |= 0x04080300; /*SH pulse, sysclk, 32 samples, software trigger*/

    ADC14->CTL1 = 0x00000020; /*12 bit resolution*/
    ADC14->MCTL[5] = 6; /*A6 input, single ended, Vref = AVCC*/
    P4->SEL1 |= 0x80; /*Configure P4.7*/
    P4->SEL0 |= 0x80;
    ADC14->CTL1 |= 0x00050000; /*convert for mem reg 5*/
    ADC14->CTL0 |= 2; /*enable adc*/
}

int adcReadVal() {
    int result;

    ADC14->CTL0 |= 1; /*start conversion*/
    while (!ADC14->IFGR0); /*wait until conversion complete*/
    result = ADC14->MEM[5]; /*read result*/

    return result;
}
