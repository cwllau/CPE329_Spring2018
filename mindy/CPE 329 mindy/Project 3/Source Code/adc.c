#include "msp.h"
#include "adc.h"

/*initialize ADC*/
void ADC_init() {
    /*ADC pin 5.4*/
    P5->SEL1 |= BIT4;
    P5->SEL0 |= BIT4;


    /*configure ADC*/
    ADC14->CTL0 = ADC14_CTL0_SHT0_2 | ADC14_CTL0_SHP | ADC14_CTL0_ON;
    ADC14->CTL1 = ADC14_CTL1_RES_2;

    ADC14->MCTL[0] |= ADC14_MCTLN_INCH_1;
    ADC14->IER0 |= ADC14_IER0_IE0;

    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;
}
