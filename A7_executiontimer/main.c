#include "msp.h"
#include <math.h>
//#include <stdint.h>

typedef int8_t VAR_TYPE;

VAR_TYPE TestFunction(VAR_TYPE num);

int main(void) {
     //var_type mainVar; Var type is pre defined, change to change timing func

    VAR_TYPE mainVar = 15;

     WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer
     P1->SEL1 &= ~BIT0; //set P1.0 as simple I/O
     P1->SEL0 &= ~BIT0;
     P1->DIR |= BIT0; //set P1.0 as output
     P2->SEL1 &= ~(BIT2 | BIT1 | BIT0); //set P2.0-2.2 as simple I/O
     P2->SEL0 &= ~(BIT2 | BIT1 | BIT0);
     P2->DIR |= (BIT2 | BIT1 | BIT0); //set P2.0-2.2 as output pins
     P2->OUT |= (BIT2 | BIT1 | BIT0); // turn on RGB LED
     mainVar = TestFunction(mainVar); // test function for timing
     P2->OUT &= ~(BIT2 | BIT1 | BIT0); // turn off RGB LED
     while(1) // infinite loop to do nothing
     mainVar++; // increment mainVar to eliminate not used warning
}
VAR_TYPE TestFunction(VAR_TYPE num) {
     VAR_TYPE testVar;
     P1->OUT |= BIT0; // set P1.0 LED on

 //{ insert_function_here (ie testVar = num;) }
     testVar = num;

     P1->OUT &= ~BIT0; // set P1.0 LED off
     return testVar;
}
