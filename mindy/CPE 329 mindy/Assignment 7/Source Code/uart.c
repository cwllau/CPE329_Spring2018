#include "uart.h"
#include "msp.h"

static int entered;
static int ready;

/*Returns ready flag*/
int getFlag() {
    return ready;
}

/*Clears ready flag*/
void resetFlag() {
    ready = 0;
}

/*Returns number read*/
int getNumber() {
    return entered;
}

/*UART setup*/
void UART0_init() {
    ready = 0;

    EUSCI_A0->CTLW0 |= 1; /*Reset mode for config*/
    EUSCI_A0->MCTLW = 0; /*Disable oversampling*/
    EUSCI_A0->CTLW0 = 0x0081; /*1 stop bit, no parity, SMCLK, 8 bit data*/
    EUSCI_A0->BRW = 26;
    P1->SEL0 |= 0x0C; /*P1.3, P1.2 for UART*/
    P1->SEL1 &= -0x0C;
    EUSCI_A0->CTLW0 &= ~1; /*Out of reset mode*/
    EUSCI_A0->IE |= 1; /*enable receive interrupt*/
}

/*Interrupt handler*/
void EUSCIA0_IRQHandler() {
    entered = 0;

    /*read and echo*/
    char c = EUSCI_A0->RXBUF;
    EUSCI_A0->TXBUF = c;

    /*loop while enter hasn't been hit*/
    while (c != 13) {
        /*Check for number*/
        if (c <= '9' && c >= '0') {
            /*Add number as next digit to entered*/
            entered = entered * 10;
            entered = entered + (c - '0');
        }

        /*read and echo*/
        c = EUSCI_A0->RXBUF;
        EUSCI_A0->TXBUF = c;
    }

    /*set ready flag*/
    ready = 1;
}
