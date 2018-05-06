#include "uart.h"
#include "msp.h"

/*UART setup*/
void UART0_init() {
    /*Configure UART pins*/
    P1->SEL0 |= BIT2 | BIT3;
    P1->SEL1 &= ~(BIT2 | BIT3);

    /* Configure UART */
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST;
    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST | EUSCI_B_CTLW0_SSEL__SMCLK;
    /* Baud Rate calculation */
    /* 49000000/118200 = 416 */
    EUSCI_A0->BRW = 416;
    EUSCI_A0->MCTLW = 0;

    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;
    EUSCI_A0->IE |= EUSCI_A_IE_TXIE;
}

/*pushes next char to UART*/
void uartPrint(char c) {
    while (!(EUSCI_A0->IFG &0x02)); /*make sure txbuf empty*/
    EUSCI_A0->TXBUF = c; /*push c*/
}
