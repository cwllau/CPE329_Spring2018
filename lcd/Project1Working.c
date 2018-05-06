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

#define RS 1     /* P4.0 mask */
#define RW 2     /* P4.1 mask */
#define EN 4     /* P4.2 mask */

void checkKey(int *keyAttempt, int *key);
void manageInput(int *keyAttempt, int *key);
void resetKey(int *keyAttempt);
void displayCharacters(int *keyAttempt);
void displayHello(int *keyAttempt, int *key);

void delayMs(int n);
void LCD_nibble_write(unsigned char data, unsigned char control);
void LCD_command(unsigned char command);
void LCD_data(unsigned char data);
void LCD_init(void);

void delay(void);
void keypad_init(void);
char keypad_getkey(void);


int main(void) {
    LCD_init();

    keypad_init();

    int keyAttempt[4] = {-1, -1, -1, -1};
    int key[4] = {1, 2, 3, 4};

        /*Print first display*/
        displayCharacters(keyAttempt);

        while (1) {
            manageInput(keyAttempt, key);
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

void checkKey(int *keyAttempt, int *key) {
    char good = 1;
    int counter;

    /*check for correct key*/
    for (counter = 0; counter < 4; counter++) {
        if (keyAttempt[counter] != key[counter])
            good = 0;
    }

    /*if correct, show hello, otherwise reset*/
    if (good) {
        displayHello(keyAttempt, key);
    }
    else {
        /*reset key and display empty attempt*/
        resetKey(keyAttempt);
        displayCharacters(keyAttempt);
    }
}

void manageInput(int *keyAttempt, int *key) {
    //TODO: Get input from keypad

    char input = 0;
    while (input == 0)
        input = keypad_getkey();

    if (input == 10) {
        resetKey(keyAttempt);
        displayCharacters(keyAttempt);

    }

    else if (input >=1 && input <= 9 || input == 11) {
        int position = 0;
        if (input == 11)
            input =  0;


        /*find first empty spot*/
        while (keyAttempt[position] > -1 && position < 3)
            position++;

        /*add number to attempt*/
        if (position < 4)
            keyAttempt[position] = input;

        /*Update display to show new character*/
        displayCharacters(keyAttempt);

        /*check for full key attempt to pass to checker*/
        if (position == 3)
            checkKey(keyAttempt, key);
    }
}

void resetKey(int *keyAttempt) {
    int counter;
    /*reset attempt to all -1*/
    for (counter = 0; counter < 4; counter++) {
        keyAttempt[counter] = -1;
    }
}

void displayCharacters(int *keyAttempt) {
    char *firstLine = "LOCKED";
    char *secLine = "ENTER KEY";
    int counter;

    LCD_command(1);     /* clear display */
    LCD_command(0x80);  /* set cursor at beginning of first line */

    /*Move through firstLine character by character*/
    for(counter = 0; firstLine[counter] != 0; counter++) {
        LCD_data(firstLine[counter]);      /* write the character*/
    }

    LCD_command(0xC0);  /* set cursor at beginning of second line */
    /*Move through secLine character by character*/
    for(counter = 0; secLine[counter] != 0; counter++) {
        LCD_data(secLine[counter]);      /* write the character*/
    }
    LCD_data(' ');
    for (counter = 0; counter < 4; counter++) {
        if (keyAttempt[counter] > -1) {
             LCD_data(keyAttempt[counter] + '0');
        }
    }
    delayMs(500); /*Delay long enough to read*/
}

void displayHello(int *keyAttempt, int *key) {
    char *line = "HELLO WORLD";
    int counter;

    LCD_command(1);     /* clear display */
    LCD_command(0x80);  /* set cursor at beginning of first line */

    /*Move through line character by character*/
    for(counter = 0; line[counter] != 0; counter++) {
        LCD_data(line[counter]);      /* write the character*/
    }

    /*move back to waiting on input*/
    manageInput(keyAttempt, key);
}

/* make a small delay */
void delay(void) {
}

void LCD_init(void) {
    P5->DIR = 0xFF;     /* make P4 pins output for data and controls */
    delayMs(30);                /* initialization sequence */
    LCD_nibble_write(0x30, 0);
    delayMs(10);
    LCD_nibble_write(0x30, 0);
    delayMs(1);
    LCD_nibble_write(0x30, 0);
    delayMs(1);
    LCD_nibble_write(0x20, 0);  /* use 4-bit data mode */
    delayMs(1);

    LCD_command(0x28);      /* set 4-bit data, 2-line, 5x7 font */
    LCD_command(0x06);      /* move cursor right after each char */
    LCD_command(0x01);      /* clear screen, move cursor to home */
    LCD_command(0x0F);      /* turn on display, cursor blinking */
}

/* With 4-bit mode, each command or data is sent twice with upper
 * nibble first then lower nibble.
 */
void LCD_nibble_write(unsigned char data, unsigned char control) {
    data &= 0xF0;           /* clear lower nibble for control */
    control &= 0x0F;        /* clear upper nibble for data */
    P5->OUT = data | control;       /* RS = 0, R/W = 0 */
    P5->OUT = data | control | EN;  /* pulse E */
    delayMs(0);
    P5->OUT = data;                 /* clear E */
    P5->OUT = 0;
}

void LCD_command(unsigned char command) {
    LCD_nibble_write(command & 0xF0, 0);    /* upper nibble first */
    LCD_nibble_write(command << 4, 0);      /* then lower nibble */

    if (command < 4)
        delayMs(4);         /* commands 1 and 2 need up to 1.64ms */
    else
        delayMs(1);         /* all others 40 us */
}

void LCD_data(unsigned char data) {
    LCD_nibble_write(data & 0xF0, RS);    /* upper nibble first */
    LCD_nibble_write(data << 4, RS);      /* then lower nibble  */

    delayMs(1);
}

/* delay milliseconds when system clock is at 3 MHz */
void delayMs(int n) {
    int i, j;

    for (j = 0; j < n; j++)
        for (i = 750; i > 0; i--);      /* Delay */
}
