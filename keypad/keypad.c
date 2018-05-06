//keypad.c
/* p3_5.c: Matrix keypad scanning
 *
 * This program scans a 4x4 matrix keypad and returns a unique number
 * for each key pressed.
 *
 * Port 4 7-4 are connected to the columns and Port 4 3-0 are connected
 * to the rows of the keypad.
 *
 * Tested with Keil 5.20 and MSP432 Device Family Pack V2.2.0
 * on XMS432P401R Rev C.
 */

#include "msp.h"

//void delay(void);
void keypad_init(void);
char keypad_getkey(void);
void LED_init(void);
void LED_set(int value);

static void set_DCO(int freq);
void delay(int ms, int freq);


#define FREQ_1_5_MHz 1500000
#define FREQ_3_MHz 3000000
#define FREQ_6_MHz 6000000
#define FREQ_12_MHz 12000000
#define FREQ_24_MHz 24000000
#define FREQ_48_MHz 48000000

int main(void) {
    unsigned char key;

    keypad_init();
    LED_init();

    while(1) {
        key = keypad_getkey();  /* read the keypad */
        LED_set(key);           /* set LEDs according to the key code */
    }
}

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
    delay(1000, 200);                    /* wait for signals to settle */
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
        delay(1000, 200);                        /* wait for signal to settle */
        col = P4->IN & 0xF0;            /* read all columns */
        P4->OUT |= row_select[row];     /* drive the active row high */
        if (col != 0xF0) break;         /* if one of the input is low, some key is pressed. */
    }
    P4->OUT |= 0x0F;                    /* drive all rows high before disable them */
    P4->DIR &= 0x0F;                    /* disable all rows */
    if (row == 4)
        return 0;                       /* if we get here, no key is pressed */

    /* gets here when one of the rows has key pressed, check which column it is */
    if (col == 0xE0) return row * 4 + 1;    /* key in column 0 */
    if (col == 0xD0) return row * 4 + 2;    /* key in column 1 */
    if (col == 0xB0) return row * 4 + 3;    /* key in column 2 */
    if (col == 0x70) return row * 4 + 4;    /* key in column 3 */

    return 0;   /* just to be safe */
}

/* Initialize tri-color LEDs on the LaunchPad board.
 * P2.0 - red
 * P2.1 - green
 * P2.2 - blue
 */
void LED_init(void) {
    P2->DIR = 0x07;         /* make LED pins output */
    P2->OUT &= ~0x07;       /* turn the LEDs off */
}

/* turn on or off the LEDs according to bit 2-0 of the value */
void LED_set(int value) {
    value &= 0x07;          /* only bit 2-0 are affected */
    P2->OUT = (P2->OUT & ~0x07) | value;
}

static void set_DCO(int freq) {
    CS->KEY = CS_KEY_VAL;

    CS->CTL0 = 0;

    if (freq == FREQ_1_5_MHz)
       CS->CTL0 = CS_CTL0_DCORSEL_0;
    else if (freq == FREQ_3_MHz)
       CS->CTL0 = CS_CTL0_DCORSEL_1;
    else if (freq == FREQ_6_MHz)
       CS->CTL0 = CS_CTL0_DCORSEL_2;
    else if (freq == FREQ_12_MHz)
       CS->CTL0 = CS_CTL0_DCORSEL_3;
    else if (freq == FREQ_24_MHz)
       CS->CTL0 = CS_CTL0_DCORSEL_4;
    else if (freq == FREQ_48_MHz) {
       // setting Vcore to level 1 for 48 MHz operation
       while(PCM->CTL1 & PCM_CTL1_PMR_BUSY);
       PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR_1;
       while(PCM->CTL1 & PCM_CTL1_PMR_BUSY);

       // configure flash wait-state to 1 for both banks 0 & 1
       FLCTL->BANK0_RDCTL = (FLCTL->BANK0_RDCTL &
           ~(FLCTL_BANK0_RDCTL_WAIT_MASK)) | FLCTL_BANK0_RDCTL_WAIT_1;

       FLCTL->BANK1_RDCTL = (FLCTL->BANK1_RDCTL &
                  ~(FLCTL_BANK1_RDCTL_WAIT_MASK)) | FLCTL_BANK1_RDCTL_WAIT_1;

       CS->CTL0 = CS_CTL0_DCORSEL_5;
    }

    CS->CTL1 &= ~(CS_CTL1_SELM_MASK | CS_CTL1_DIVM_MASK) | CS_CTL1_SELM_3;

    CS->KEY = 0;
}

/* make a small delay */
void delay(int ms, int freq) {
    int i, j;

       set_DCO(freq);

       for (i = 0; i < ms; i++)
          for (j = .001 * freq / 10; j > 0; j--);   // delay 1 ms (approx)
}
