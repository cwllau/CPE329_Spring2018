#include "msp.h"
#include "keypad.h"
#include <stdio.h>

#define CYCLETIMER 100
#define MAXANGLE 180

void processKey();
void portRemap();
void delayDuration(int n);

static int angle;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD; /*Stop watchdog timer*/

    CS->KEY = CS_KEY_VAL; // unlock CS registers
    CS->CTL0 = 0; // clear register CTL0
    CS->CTL0=CS_CTL0_DCORSEL_0; /*1.5 MHz*/

    CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3; // select clock sources
    CS->KEY = 0; // lock the CS registers


    P2->SEL0 |= 0x80; /*P2.7 is TA0.4 out*/
    P2->SEL1 &= ~0x80;
    P2->DIR |= 0x80;

    portRemap();

    TIMER_A0->CCR[0] = 30000 - 1; /*period*/
    TIMER_A0->CCR[4] = 0; /*PWM duty cycle*/
    TIMER_A0->CCTL[4] = 0xE0; /*reset/set mode*/
    TIMER_A0->CTL = 0x214; /*SMCLK, count up, clear TA0R register*/


    //TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | // SMCLK, continuous mode
           // TIMER_A_CTL_MC__CONTINUOUS;

    keypad_init(); /*initialize keypad pins*/
    angle = 0; /*initialize angle*/

    while (1) {
        processKey(); /*get input from keypad*/
        TIMER_A0->CCR[4] = 1100 + 2300 * angle / MAXANGLE; /*PWM duty cycle*/
        printf("%d\n", angle);
    }
}

void processKey() {
    int firstKey, secondKey = 0;

    firstKey = keypad_getkey();
    delayDuration(100); /*software delay*/

    /*check for numeric input*/
    if (firstKey > 0 && firstKey != 10 && firstKey != 12) {

        while (secondKey == 0) {
            secondKey = keypad_getkey();
            delayDuration(100); /*software delay*/
        }

        if (firstKey == 11) /*convert if 0 pressed*/
            firstKey = 0;
        if (secondKey == 11) /*convert if 0 pressed*/
            secondKey = 0;

        angle = firstKey * 100 + secondKey * 10; /*set angle*/

        if (angle > 180)
            angle = 180;
    }
    /*check for * input and valid angle*/
    else if (firstKey == 10 && angle > 0)
        angle -= 10;
    /*check for # input and valid angle*/
    else if (firstKey == 12 && angle < 180)
        angle += 10;
}

void portRemap() {
    PMAP->KEYID = 0x2D52; /*unlock PMAP*/

    P2MAP->PMAP_REGISTER1 = 34; /*remap P2.1 to 23 (TPM0.4)*/
    P2->DIR |= 2;
    P2->SEL0 |= 2;
    P2->SEL1 &= !2;

    PMAP->CTL = 1; /*lock PMAP*/
    PMAP->KEYID = 0;
}

/* delay milliseconds when system clock is at 12 MHz */
void delayDuration(int n) {
    int i, j;

    for (j = 0; j < n; j++)
        for (i = 300; i > 0; i--);      /* Delay */
}
