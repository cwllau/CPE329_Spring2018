#include "msp.h"
#include "uart.h"
#include "terminal.h"
#include "adc.h"
#include "multimeter.h"

#define CONVERSION 13 /*Conversion factor for ADC measurements*/
#define CYCLETIMER 48000 /*Timer for voltage readings*/
#define CYCLETIMER0 9600 //Adjust this for frequency


/*keeps track of system time in ms & last read voltage*/
static short val;
static int time;

int main() {
    /*Stop watchdog timer*/
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    P1->DIR |= BIT0;
    P1->OUT &= ~BIT0;
    /*Disable interrupts*/
    __disable_irq();
    /* Transition to VCORE Level 1: AM0_LDO --> AM1_LDO */
    while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
     PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR_1;
    while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));

    /* Configure Flash wait-state to 1 for both banks 0 & 1 */
    FLCTL->BANK0_RDCTL = (FLCTL->BANK0_RDCTL &
     ~(FLCTL_BANK0_RDCTL_WAIT_MASK)) | FLCTL_BANK0_RDCTL_WAIT_1;
    FLCTL->BANK1_RDCTL = (FLCTL->BANK0_RDCTL &
     ~(FLCTL_BANK1_RDCTL_WAIT_MASK)) | FLCTL_BANK1_RDCTL_WAIT_1;

    /*Set CLK to 48 MHz*/
    CS->KEY = CS_KEY_VAL;
    CS->CTL0 = 0;
    CS->CTL0 = CS_CTL0_DCORSEL_5;
    CS->CTL1 = CS->CTL1 & ~(CS_CTL1_SELM_MASK | CS_CTL1_DIVM_MASK) |
     CS_CTL1_SELM_3;
    CS->KEY = 0;

    UART0_init(); /*start up uart transmission*/
    terminal_init(); /*set up terminal frame*/
    ADC_init(); /*start up ADC*/
    multimeter_init(); /*start up multimeter system*/

    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; // TACCR0 interrupt enabled

    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_CCIE; // TACCR1 interrupt enabled
    TIMER_A0->CCR[0] = CYCLETIMER0; //Set CCR0 for on cycle
    TIMER_A0->CCR[1] = CYCLETIMER; //Set CCR0 for on cycle


    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | // SMCLK, continuous mode
            TIMER_A_CTL_MC__CONTINUOUS;

    time = 0;

    NVIC_EnableIRQ(EUSCIA0_IRQn); /*enable uart tx interrupt*/
    NVIC_SetPriority(EUSCIA0_IRQn, 6); /*set last priority for uart*/

    NVIC_SetPriority(ADC14_IRQn, 5); /*higher priority for adc*/
    NVIC_EnableIRQ(ADC14_IRQn); /*enable ADC interrupt*/

    NVIC_EnableIRQ(TA0_0_IRQn);
    NVIC_SetPriority(TA0_0_IRQn, 1);
    NVIC_EnableIRQ(TA0_N_IRQn);
    NVIC_SetPriority(TA0_N_IRQn, 7);


    ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC; /*start reading adc*/

    /*enable interrupts*/
    __enable_irq();

    /*endless loop*/
    while(1) {
    }
}

void TA0_0_IRQHandler() {
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
    TIMER_A0 ->CCR[0] += CYCLETIMER0;
    time += 1;
    readFrequency(val, time);
}

/*Timer interrupt handler*/
void TA0_N_IRQHandler() {
    if (TIMER_A0->CCTL[1] & TIMER_A_CCTLN_CCIFG) {
        readVol(val);
        TIMER_A0->CCR[1] += CYCLETIMER;
        TIMER_A0->CCR[1] &= ~TIMER_A_CCTLN_CCIFG;
    }
}

/*Uart interrupt handler*/
void EUSCIA0_IRQHandler() {
    /*push next value over UART*/
    printVals();
}

/*ADC interrupt handler*/
void ADC14_IRQHandler() {
    /*read value from ADC*/
    val = ADC14->MEM[0] / CONVERSION;

    /*start next ADC processing*/
    ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;
}
