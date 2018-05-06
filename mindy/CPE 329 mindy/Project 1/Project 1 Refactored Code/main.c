/*
 * Project 1 - Combination Lock
 */

#include "msp.h"
#include "LCD.h"
#include "keypad.h"

void checkKey(int *keyAttempt, int *key);
void manageInput(int *keyAttempt, int *key);
void resetKey(int *keyAttempt);
void displayCharacters(int *keyAttempt);
void displayHello();

int main(void) {
    /* Initialize LCD and keypad
     * Initialize keyAttempt
     * Set key to 1234
     */
    LCD_init();
    keypad_init();

    int keyAttempt[4] = {-1, -1, -1, -1};
    int key[4] = {1, 2, 3, 4};

        /*Print first display*/
        displayCharacters(keyAttempt);

        while (1) {
            /*Manage inputs infinitely*/
            manageInput(keyAttempt, key);
        }
}

/*
 * Checks the key attempt against the valid key
 * calls displayHello if good
 * otherwise calls resetKey and displayCharacters
 */
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
        displayHello();
    }
    else {
        /*reset key and display empty attempt*/
        resetKey(keyAttempt);
        displayCharacters(keyAttempt);
    }
}

/*
 * Manages input from the keypad
 * if * pressed, calls resetKey
 * Otherwise, reads in number and adds to attempt
 * Calls displayCharacters
 * If attempt is full, calls checkKey
 */
void manageInput(int *keyAttempt, int *key) {
    /*Get input from keypad*/
    char input = 0;
    while (input == 0)
        input = keypad_getkey();

    /*Check for 10 (* on keypad)*/
    if (input == 10) {
        resetKey(keyAttempt);
        displayCharacters(keyAttempt);
    }

    /*Check for 1-9 or 11 (1-9 and 0 on keypad)*/
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

/*
 * Resets keyAttempt to all -1 (invalid)
 */
void resetKey(int *keyAttempt) {
    int counter;
    /*reset attempt to all -1*/
    for (counter = 0; counter < 4; counter++) {
        keyAttempt[counter] = -1;
    }
}

/*
 * displays the following
 * LOCKED
 * ENTER KEY xxxx
 * Where xxxx is the current attempt, up to 4 numbers
 */
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

    /*Write any entered numbers*/
    LCD_data(' ');
    for (counter = 0; counter < 4; counter++) {
        if (keyAttempt[counter] > -1) {
             LCD_data(keyAttempt[counter] + '0');
        }
    }
    delayMs(500); /*Delay long enough to read*/
}

/*
 * Displays the following:
 * HELLO WORLD
 */
void displayHello(int *keyAttempt, int *key) {
    char *line = "HELLO WORLD";
    int counter;

    LCD_command(1);     /* clear display */
    LCD_command(0x80);  /* set cursor at beginning of first line */

    /*Move through line character by character*/
    for(counter = 0; line[counter] != 0; counter++) {
        LCD_data(line[counter]);      /* write the character*/
    }
}


