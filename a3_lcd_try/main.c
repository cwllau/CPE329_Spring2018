#include "msp.h"
#include "lcd_functions.h"
//need the definition of a function before it gets called


/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
}

