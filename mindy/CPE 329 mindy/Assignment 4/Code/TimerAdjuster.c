//******************************************************************************
//  MSP432P401 Demo - Timer0_A3, Toggle P1.0, CCR0 Cont Mode ISR, DCO SMCLK
//
//  Description: Toggle P1.0 using software and TA_0 ISR. Timer0_A is
//  configured for continuous mode, thus the timer overflows when TAR counts
//  to CCR0. In this example, CCR0 is loaded with 50000.
//  ACLK = n/a, MCLK = SMCLK = TACLK = default DCO = ~3MHz
//
//           MSP432P401x
//         ---------------
//     /|\|               |
//      | |               |
//      --|RST            |
//        |               |
//        |           P1.0|-->LED
//
//   William Goh
//   Texas Instruments Inc.
//   June 2016 (updated) | November 2013 (created)
//   Built with CCSv6.1, IAR, Keil, GCC
//******************************************************************************
#include "msp.h"
#define CYCLETIMER 20 //Adjust this to shorten ISR Timer

int main(void) {
    WDT_A->CTL = WDT_A_CTL_PW |             // Stop WDT
            WDT_A_CTL_HOLD;

    // Configure LED
    P1->SEL1 &= ~BIT0;
    P1->SEL0 &= ~BIT0;
	P1->DIR |= BIT0;
	
	CS->KEY = CS_KEY_VAL; // unlock CS registers
    CS->CTL0 = 0; // clear register CTL0
	CS->CTL0=CS_CTL0_DCORSEL_4; //24 MHz
	CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3; // select clock sources
    CS->KEY = 0; // lock the CS registers
	
	
	P1->OUT |= BIT0;
	
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; // TACCR0 interrupt enabled
	
    TIMER_A0->CCR[0] = CYCLETIMER; //Set CCR0 to 240 for on cycle
	
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | // SMCLK, continuous mode
            TIMER_A_CTL_MC__CONTINUOUS;

    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;    // Enable sleep on exit from ISR

    // Enable global interrupt
    __enable_irq();

    NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);

    while (1)
    {
        __sleep();

        __no_operation();                   // For debugger
    }
}

// Timer A0 interrupt service routine

void TA0_0_IRQHandler(void) {
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
	
	TIMER_A0->CCR[0] += CYCLETIMER; //increment timer
	P1->OUT ^= BIT0; //turn P1.0 on/off
}


