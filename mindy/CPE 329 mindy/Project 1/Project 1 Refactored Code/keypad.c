/*
 * keypad.c
 *
 *  Created on: Apr 17, 2017
 *      Author: Brendan
 */

#include "msp.h"
#include "keypad.h"

/* this function initializes Port 4 that is connected to the keypad.
 * All pins are configured as GPIO input pin. The column pins have
 * the pull-up resistors enabled.
 */
void keypad_init(void) {
    P4->DIR = 0;
    P4->REN = 0xF0;     /* enable pull resistor for column pins */
    P4->OUT = 0xF0;     /* make column pins pull-ups */
}

/*
 * This is a non-blocking function to read the keypad.
 * If a key is pressed, it returns a unique code for the key. Otherwise,
 * a zero is returned.
 * The upper nibble of Port 4 is used as input and connected to the columns.
 * Pull-up resistors are enabled so when the keys are not pressed, these pins
 * are pulled high.
 * The lower nibble of Port 4 is used as output that drives the keypad rows.
 * First all rows are driven low and the input pins are read. If no key is pressed,
 * they will read as all one because of the pull up resistors. If they are not
 * all one, some key is pressed.
 * If some key is pressed, the program proceeds to drive one row low at a time and
 * leave the rest of the rows inactive (float) then read the input pins.
 * Knowing which row is active and which column is active, the program
 * can decide which key is pressed.
 *
 * Only one row is driven so that if multiple keys are pressed and row pins are shorted,
 * the microcontroller will not be damaged. When the row is being deactivated,
 * it is driven high first otherwise the stray capacitance may keep the inactive
 * row low for some time.)
 */
char keypad_getkey(void) {
    int row, col;
    const char row_select[] = {0x01, 0x02, 0x04, 0x08}; /* one row is active */

    /* check to see any key pressed */
    P4->DIR |= 0x0F;            /* make all row pins output */
    P4->OUT &= ~0x0F;           /* drive all row pins low */
    delay();                    /* wait for signals to settle */
    col = P4->IN & 0xF0;        /* read all column pins */
    P4->OUT |= 0x0F;            /* drive all rows high before disable them */
    P4->DIR &= ~0x0F;           /* disable all row pins drive */
    if (col == 0xF0)            /* if all columns are high */
        return 0;               /* no key pressed */

    /* If a key is pressed, it gets here to find out which key.
     * It activates one row at a time and read the input to see
     * which column is active. */
    for (row = 0; row < 4; row++) {
        P4->DIR &= 0x0F;                /* disable all rows */
        P4->DIR |= row_select[row];     /* enable one row at a time */
        P4->OUT &= ~row_select[row];    /* drive the active row low */
        delay();                        /* wait for signal to settle */
        col = P4->IN & 0xF0;            /* read all columns */
        P4->OUT |= row_select[row];     /* drive the active row high */
        if (col != 0xF0) break;         /* if one of the input is low, some key is pressed. */
    }
    P4->OUT |= 0x0F;                    /* drive all rows high before disable them */
    P4->DIR &= 0x0F;                    /* disable all rows */
    if (row == 4)
        return 0;                       /* if we get here, no key is pressed */

    /* gets here when one of the rows has key pressed, check which column it is */
    if (col == 0xE0) return row * 3 + 1;    /* key in column 0 */
    if (col == 0xD0) return row * 3 + 2;    /* key in column 1 */
    if (col == 0xB0) return row * 3 + 3;    /* key in column 2 */
   // if (col == 0x70) return row * 4 + 4;    /* key in column 3 */

    return 0;   /* just to be safe */
}

/*Create a small delay*/
void delay(void) {
}
