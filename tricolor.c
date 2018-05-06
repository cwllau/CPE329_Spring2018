/* p2_1.c Toggling green LED in C using header file register definitions.
 * This program toggles green LED for 0.5 second ON and 0.5 second OFF.
 * The green LED is connected to P2.1.
 * The LEDs are high active (a '1' turns ON the LED).
 *
 * Tested with Keil 5.20 and MSP432 Device Family Pack V2.2.0
 * on XMS432P401R Rev C.
 */

#include "msp.h"

#define DELAY100MS 300000u // DELAY * 1 mS assuming 3 MHz clock

void delayMs(int n);


int main(void) {

    uint32_t counter = 0;
    uint8_t i = 0;
    P2->SEL1 &= ~0x01;         /* configure P2.0 as simple I/O */
    P2->SEL1 &= ~0x02;         /* configure P2.1 as simple I/O */
    P2->SEL1 &= ~0x04;         /* configure P2.2 as simple I/O */

    P2->SEL0 &= ~0x07;          /* configure P2.2-0 as simple I/O*/

    P2->DIR |= BIT2 | BIT1 | BIT0;           /* P2.2-0 set as output pin */
// P20->DIR |= (1<<2) | (1<<1) | (1<<0);

    while (1)
    {

            while (counter <= 2)
            {
            P2->OUT |= (1<<counter);       /* turn on P2.1 green LED */
            __delay_cycles(DELAY100MS);

            P2->OUT &= ~(1<<counter);      /* turn off P2.1 green LED */
            __delay_cycles(DELAY100MS);

            counter ++;
            }

            counter = 0;
    } // end while(1)

} // end main

