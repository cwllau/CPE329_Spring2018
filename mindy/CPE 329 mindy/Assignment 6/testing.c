/*
 * Project 1 - Combination Lock
 */

#include <ti/devices/msp432p4xx/inc/msp.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <inttypes.h>

#define VARTYPE double

VARTYPE TestFunction(VARTYPE n);
void delayMs(int n);
void LCD_nibble_write(unsigned char data, unsigned char control);
void LCD_command(unsigned char command);
void LCD_data(unsigned char data);
void LCD_init(void);

int main(void) {
    VARTYPE y;
    int counter;
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer
    LCD_init();
//    P1->SEL1 &= ~BIT0; //set P1.0 as simple I/O
//    P1->SEL0 &= ~BIT0;

//    P1->DIR |= BIT0; //set P1.0 as output

//    P2->SEL1 &= ~(BIT2 | BIT1 | BIT0); //set P2.0-2.2 as simple I/O
//    P2->SEL0 &= ~(BIT2 | BIT1 | BIT0);

//    P2->DIR |= (BIT2 | BIT1 | BIT0); //set P2.0-2.2 as output pins

//    P2->OUT |= (BIT2 | BIT1 | BIT0); // turn on RGB LED

    SysTick_enableModule();
    SysTick_setPeriod(16777216);

    uint32_t start = SysTick_getValue();

    y = TestFunction(15); // test function for timing

    uint32_t end = start - SysTick_getValue();
    end = end / 3;

    printf("%" PRIu32 " us\n", end);

//    P2->OUT &= ~(BIT2 | BIT1 | BIT0); // turn off RGB LED
    while (1)
        delayMs(100);
}

VARTYPE TestFunction(VARTYPE y) {
    VARTYPE x;
//    P1->OUT |= BIT0; // set P1.0 LED on

    //x = y;
    //x = y + 1;
    //x = y + 7;
    //x = y * 2;
    //x = y * 3;
    //x = y / 2;
    //x = y / 3;
    //x = sin(y);
    //x = sinh(y);
    //x = sqrt(y);
    //x = abs(y);

//    P1->OUT &= ~BIT0; // set P1.0 LED off
    return y;
}