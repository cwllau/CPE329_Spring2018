#include "msp.h"

//functions to put in header file


/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

}

//initializing port 4 for input
void keypad_init(void)
{
    P4->DIR &= ~(0xF0); //setting up column inputs, clearing upper 4 bits
    P4->REN |= (0xF0); //enable pull resistor for column pins
    P4->OUT &= ~(0xF0); //set pull down resistor

    P4->OUT |= 0x0F; //setting up row outputs, setting lower 4 bits
}



/* make a small delay */
void delay(void) {
    int i, j;
        for (j = 0; j < 500; j++)
            for (i = 750; i > 0; i--);   // delay 1 ms (approx)
}
