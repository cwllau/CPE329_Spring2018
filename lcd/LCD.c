#include "msp.h"
#include "delay.h"
#include "stdio.h"

typedef unsigned char Byte;

// set E(4.6) high, delay, then set it low
static void Send_Nybble() {
    P4->OUT |= BIT6;
    delay_ms(2, FREQ_3_MHz);
    P4->OUT &= ~BIT6;
}

// sets DB7-DB4 accordingly
static void Send_Upper_Four(Byte byte) {
    P4->OUT &= ~(0x0F);
    P4->OUT |= (byte >> 4);
}

static void Send_Command(Byte byte) {
    P4->OUT &= ~(BIT5 | BIT4);  // set both RS and R/W low

    // send upper nybble
    P4->OUT &= ~(0x0F);
    P4->OUT |= (byte >> 4);

    Send_Nybble();

    // send lower nybble
    P4->OUT &= ~(0x0F);
    P4->OUT |= (byte & 0x0F);

    Send_Nybble();
}

/* The following is assumed:
 * P4.7 is NO CONNECTION
 * P4.6 is E
 * P4.5 is RS
 * P4.4 is R/W
 * P4.3 is DB7
 * P4.2 is DB6
 * P4.1 is DB5
 * P4.0 is DB4
 * setup all of P4 as GPIO, even though we wont use 4.7
 */
void init_pins_LCD() {
    P4->SEL0 = 0x00;
    P4->SEL1= 0x00;
    P4->DIR = 0xFF; // All pins are output
}

void Startup_LCD() {
    P4->OUT &= ~(BIT5 | BIT4); // set RS and R/W low

    Send_Upper_Four(0x00);
    delay_ms(100, FREQ_3_MHz);

    Send_Upper_Four(0x30); // wake-up
    delay_ms(30, FREQ_3_MHz);

    Send_Nybble();
    delay_ms(10, FREQ_3_MHz);
    Send_Nybble();
    delay_ms(10, FREQ_3_MHz);
    Send_Nybble();
    delay_ms(10, FREQ_3_MHz);

    Send_Upper_Four(0x20);
    Send_Nybble();

    Send_Command(0x28);
    Send_Command(0x10);
    Send_Command(0x0F);
    Send_Command(0x06);
    //Send_Command(0x80); //set cursor at beginning of line
}

void Clear_LCD() {
    Send_Command(0x01);
    delay_ms(10, FREQ_3_MHz); // delay after sent command nec?
}

void Home_LCD() {
    Send_Command(0x02);
    delay_ms(10, FREQ_3_MHz); // delay after sent command nec?
}

void Write_char_LCD(char c) {
    P4->OUT |= BIT5;   // set RS high
    P4->OUT &= ~BIT4;  // set R/W low

    // send upper nybble
    P4->OUT &= ~(0x0F);
    P4->OUT |= (c >> 4);

    Send_Nybble();

    // send lower nybble
    P4->OUT &= ~(0x0F);
    P4->OUT |= (c & 0x0F);

    Send_Nybble();
}

void HelloWorld()
{
    char *line = "HELLO WORLD";
    int counter;

    Send_Command(0x01);     //clear display
    Send_Command(0x80);     //set cursor at the beginning of the line

    for (counter = 0; line[counter] != 0; counter++)
    {
        //move through the line character by character
        Write_char_LCD(line[counter]);
    }
}

//void Write_string_LCD(char word[256])
//{
//    int counter;
//    Send_Command(0x01);     //clear display
//    Send_Command(0x80);     //set cursor at the beginning of the line
//
//    //move through the string until it finds null character '\0'
//    for (counter = 0; word[counter] < 7; counter++)
//    {
//        //Write_char_LCD(word[counter]);
//        //printf("word[counter])";
//    }
//}
