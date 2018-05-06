/*
 * Project 2
 */

#include "msp.h"
#include "LCD.h"
#include "keypad.h"
#include "sin.h"
#include <stdio.h>

#define SQUARE 7
#define SINE 8
#define SAWTOOTH 9
#define DUTYMINUS 10
#define DUTYMID 11
#define DUTYPLUS 12
#define CYCLETIMER 15 //Adjust this for ISR Timer
#define MAXDAC 3700

void driveWave();
void Drive_DAC(unsigned int level);
volatile unsigned int outHigh;
int wave;
int duty;
int frequency;
int squareTimeHigh;
int squareTimeLow;
int numInterrupts;
int sawtoothMax;

void displayWaveform();
void manageInput();

int main(void) {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;   // Stop watchdog timer

     // DCO = 24 MHz, SMCLK and MCLK = DCO
     CS->KEY = CS_KEY_VAL;
     CS->CTL0 = 0;
     CS->CTL0 = CS_CTL0_DCORSEL_4;   // DCO = 24 MHz
     CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
     CS->KEY = 0;

     // Configure port bits for SPI
     P6->DIR |= BIT1;                     // Will use BIT4 to activate /CE on the DAC
     P1SEL0 |= BIT6 + BIT5;               // Configure P1.6 and P1.5 for UCB0SIMO and UCB0CLK
     P1SEL1 &= ~(BIT6 + BIT5);            //

     // SPI Setup
     EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST;   // Put eUSCI state machine in reset

     EUSCI_B0->CTLW0 = EUSCI_B_CTLW0_SWRST |   // Remain eUSCI state machine in reset
                       EUSCI_B_CTLW0_MST   |   // Set as SPI master
                       EUSCI_B_CTLW0_SYNC  |   // Set as synchronous mode
                       EUSCI_B_CTLW0_CKPL  |   // Set clock polarity high
                       EUSCI_B_CTLW0_MSB;      // MSB first

     EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SSEL__SMCLK; // SMCLK
     EUSCI_B0->BRW = 0x01;                         // divide by 16, clock = fBRCLK/(UCBRx)
     EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;      // Initialize USCI state machine, SPI
                                                   // now waiting for something to
                                                   // be placed in TXBUF

     EUSCI_B0->IFG |= EUSCI_B_IFG_TXIFG;  // Clear TXIFG flag

     TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; // TACCR0 interrupt enabled
     TIMER_A0->EX0 = 7;
     TIMER_A0->CCR[0] = CYCLETIMER; //Set CCR0 for on cycle

     TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | // SMCLK, continuous mode
                 0x10;


     // Enable global interrupt
     __enable_irq();

      NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);





    /* Initialize LCD and keypad
     */
    LCD_init();
    keypad_init();

    /* Set default wave type
     */
    wave = SQUARE;
    frequency = 100;
    duty = 50;
    squareTimeHigh = duty * 2000 / frequency;
    squareTimeLow = duty * 2000 / frequency;
    sawtoothMax = 200000 / frequency;
    numInterrupts = 0;

    displayWaveform();

    while (1) {
        /*Manage inputs infinitely*/
        manageInput();
        driveWave();
    }
}

/*
 * Manages input from the keypad
 */
void manageInput(int *keyAttempt, int *key) {
    /*Get input from keypad*/
    char input = 0;
        input = keypad_getkey();

    if (input != 0)
    {
        /*debounce*/
        delayMs(100);
        /*Check for 1-5*/
        if (input >=1 && input <= 5) {
            frequency = input * 100;
            squareTimeHigh = duty * 2000 / frequency;
            squareTimeLow = (100 - duty) * 2000 / frequency;
            numInterrupts = 0;
            sawtoothMax = 200000 / frequency;
        }

        /*handle wave/duty cases*/
        switch (input) {
                    case SQUARE:
                        wave = SQUARE;
                        numInterrupts = 0;
                        break;
                    case SINE:
                        wave = SINE;
                        numInterrupts = 0;
                        break;
                    case SAWTOOTH:
                        wave = SAWTOOTH;
                        numInterrupts = 0;
                        break;
                    case DUTYMINUS:
                        if (duty > 10 && wave == SQUARE)
                            duty -= 10;
                        squareTimeHigh = duty * 2000 / frequency;
                        squareTimeLow = (100 - duty) * 2000 / frequency;
                        numInterrupts = 0;
                        break;
                    case DUTYMID:
                        duty = 50;
                        squareTimeHigh = duty * 2000 / frequency;
                        squareTimeLow = duty * 2000 / frequency;
                        numInterrupts = 0;
                        break;
                    case DUTYPLUS:
                        if (duty < 90 && wave == SQUARE)
                            duty += 10;
                            squareTimeHigh = duty * 2000 / frequency;
                            squareTimeLow = (100 - duty) * 2000 / frequency;
                        numInterrupts = 0;
                        break;
        }

        displayWaveform();
    }
}

void driveWave() {
    if (wave == SQUARE) {
        if (outHigh)
            Drive_DAC(MAXDAC);
        else
            Drive_DAC(0);
    }
    else if (wave == SAWTOOTH) {
        int driver = MAXDAC * numInterrupts;
        driver = driver / sawtoothMax;
        Drive_DAC(driver);
    }
    else if (wave == SINE) {
        int sinLoc = numInterrupts * frequency / 100;
        int driver = sinVal[sinLoc];
        Drive_DAC(driver);
    }

}

/*
 * Displays the following:
 * <wavetype> WAVE
 * <frequency> HZ <duty>%
 */
void displayWaveform() {
    char *waveform = " WAVE";
    char *type;

    if (wave == SQUARE) {
        type = "SQUARE";
    }
    else if (wave == SINE) {
        type = "SINE";
    }
    else if (wave == SAWTOOTH) {
        type = "SAWTOOTH";
    }

    LCD_command(1);     /* clear display */
    LCD_command(0x80);  /* set cursor at beginning of first line */

    int counter;
    /*Move through line character by character*/
    for(counter = 0; type[counter] != 0; counter++) {
        LCD_data(type[counter]);      /* write the character*/
    }
    for(counter = 0; waveform[counter] != 0; counter++) {
        LCD_data(waveform[counter]);      /* write the character*/
    }

    LCD_command(0xC0);  /* set cursor at beginning of second line */

    LCD_data(frequency / 100 + '0');
    LCD_data('0');
    LCD_data('0');
    LCD_data(' ');
    LCD_data('H');
    LCD_data('Z');

    if (wave == SQUARE) {
        LCD_data(' ');
        LCD_data(duty / 10 + '0');
        LCD_data('0');
        LCD_data('%');
    }

    /*for debugging
     */
    printf("%s wave %d Hz %d%% duty\n", type, frequency, duty);
}






// Timer A0 interrupt service routine
void TA0_0_IRQHandler(void) {
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
    numInterrupts++;

    if (wave == SQUARE && outHigh && numInterrupts == squareTimeHigh) {
        outHigh = ~outHigh;
        numInterrupts = 0;
    }
    if (wave == SQUARE && !outHigh && numInterrupts == squareTimeLow) {
        outHigh = ~outHigh;
        numInterrupts = 0;
    }
    if (wave == SAWTOOTH && numInterrupts == sawtoothMax) {
        numInterrupts = 0;
    }
    if (wave == SINE && numInterrupts == sawtoothMax) {
        numInterrupts = 0;
    }
}

void Drive_DAC(unsigned int level){
  unsigned int DAC_Word = 0;
  int i;

  DAC_Word = (0x3000) | (level & 0x0FFF);   // 0x1000 sets DAC for Write
                                            // to DAC, Gain = 1, /SHDN = 1
                                            // and put 12-bit level value
                                            // in low 12 bits.

  P6->OUT &= ~BIT1;                                   // Clear P4.1 (drive /CS low on DAC)
                                                      // Using a port output to do this for now

  EUSCI_B0->TXBUF = (unsigned char) (DAC_Word >> 8);  // Shift upper byte of DAC_Word
                                                      // 8-bits to right

  while (!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG));              // USCI_A0 TX buffer ready?

  EUSCI_B0->TXBUF = (unsigned char) (DAC_Word & 0x00FF);     // Transmit lower byte to DAC

  while (!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG));      // Poll the TX flag to wait for completion

  for(i = 200; i > 0; i--);                          // Delay 200 16 MHz SMCLK periods
                                                     //to ensure TX is complete by SIMO

  P6->OUT |= BIT1;                                   // Set P4.1   (drive /CS high on DAC)

  return;
}
