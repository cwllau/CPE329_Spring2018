#include "msp.h"
#include "keypad.c"

/**
 * main.c
 */
void main1(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
}
