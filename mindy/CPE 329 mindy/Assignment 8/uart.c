#include "uart.h"
#include "msp.h"

/*UART setup*/
void UART0_init() {
    EUSCI_A0->CTLW0 |= 1; /*Reset mode for config*/
    EUSCI_A0->MCTLW = 0; /*Disable oversampling*/
    EUSCI_A0->CTLW0 = 0x0081; /*1 stop bit, no parity, SMCLK, 8 bit data*/
    EUSCI_A0->BRW = 26;
    P1->SEL0 |= 0x0C; /*P1.3, P1.2 for UART*/
    P1->SEL1 &= -0x0C;
    EUSCI_A0->CTLW0 &= ~1; /*Out of reset mode*/
//    EUSCI_A0->IE |= 1; /*enable receive interrupt*/
}

/*Sends c over uart*/
void uartPrint(char c) {
    int counter;
    /*echo c to console*/
    EUSCI_A0->TXBUF = c;
    for(counter = 0; counter < 100; counter++);
}
