#include "msp.h"
#include "delay.h"


typedef unsigned char Byte;

// set E(4.6) high, delay, then set it low
static void Send_Nybble() {
    P5->OUT |= BIT2;
    delay_ms(2, FREQ_3_MHz);
    P5->OUT &= ~BIT2;
}

// sets DB7-DB4 accordingly
static void Send_Upper_Four(Byte byte) {
    P5->OUT &= ~(0xF0); // send rs, r/w and e first
    P5->OUT |= (byte & 0x0F);
}

static void Send_Command(Byte byte) {
    P5->OUT &= ~(BIT0 | BIT1);  // set both RS and R/W low

    // send upper nybble
          P5->OUT &= ~(0x0F);
          P5->OUT |= (byte >> 4);


    Send_Nybble();

    // send lower nybble
            P5->OUT &= ~(0x0F);
            P5->OUT |= (byte & 0x0F);


    Send_Nybble();
}

/* The following is assumed:
 * P4.7     P3.0 is NO CONNECTION
 * P4.6     P5.2 is E
 * P4.5     P5.0 is RS
 * P4.4     P5.1 is R/W
 * P4.3     P5.7 is DB7
 * P4.2     P5.6 is DB6
 * P4.1     P5.5 is DB5
 * P4.0     P5.4 is DB4
 * setup all of P4 as GPIO, even though we wont use 4.7
 */
void init_pins_LCD() {
    P5->SEL0 &= 0x01;
    P5->SEL1 &= 0x01;
    P4->DIR |= 0xFE; // All pins but 5.1 are output
}

void Startup_LCD() {
    P5->OUT &= ~(BIT0 | BIT1); // set RS and R/W low

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
    P5->OUT |= BIT0;   // set RS high
    P5->OUT &= ~BIT1;  // set R/W low

    // send upper nybble
       P5->OUT &= ~(0x0F);
       P5->OUT |= (c >> 4);

    Send_Nybble();

    // send lower nybble
    P5->OUT &= ~(0x0F);
    P5->OUT |= (c & 0x0F);


    Send_Nybble();
}

void HelloWorld()
{
    char *a = "Ba"
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
