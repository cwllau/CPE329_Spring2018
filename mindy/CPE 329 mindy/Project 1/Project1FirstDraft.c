//*****************************************************************************
//
// MSP432 main.c
// Project 1
// Manages a key lock hello world program
//
//****************************************************************************

#include "msp.h"
#include <stdio.h>
#include <String.h>

void checkKey(int *keyAttempt, int *key);
void manageInput(int *keyAttempt, int *key);
void resetKey(int *keyAttempt);
void displayCharacters(int *keyAttempt);
void displayHello(int *keyAttempt, int *key);

void main(void)
{
    int keyAttempt[4] = {-1, -1, -1, -1};
    int key[4] = {1, 2, 3, 4};

    /*Print first display*/
    displayCharacters(keyAttempt);

    while (1) {
        manageInput(keyAttempt, key);
    }
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

    if (input == '*')
        resetKey(keyAttempt);

    else if (input >='0' && input <= '9') {
        int position = 0;

        /*find first empty spot*/
        while (keyAttempt[position] > -1)
            position++;

        /*add number to attempt*/
        keyAttempt[position] = input - '0';

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
    char secLine[20] = "ENTER KEY";
    int counter;

    /*Append any entered numbers*/
    for (counter = 0; counter < 4; counter++) {
        if (keyAttempt[counter] > -1) {
             char append[3];
             sprintf(append, " %d", keyAttempt[counter]);
             strcpy(secLine, append);
        }
    }

    //TODO: print both lines
}

void displayHello(int *keyAttempt, int *key) {
    char *line = "HELLO WORLD";

    //TODO: print line

    /*move back to waiting on input*/
    manageInput(keyAttempt, key);
}
