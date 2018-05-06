#include "msp.h"
#include "uart.h"
#include "adc.h"
#include "terminal.h"
#include <math.h>

#define CONVERSION 1.3 //Adjust this for voltage measurements
#define CYCLETIMER 30000 //Adjust this for ISR Timer

void measureVal(int val);
void RMS();
void DC();

static int topPeak;
static int bottomPeak;
static int topPeakTime;
static int rising;
static int DCoffset;
static int peakToPeak;
static short measurements[12000];
static int location;
static int trueRMS;
static int calcRMS;
static int dc;
static int frequency;
static int numMeasurements;

int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW |             // Stop watchdog timer
            WDT_A_CTL_HOLD;

    CS->KEY = CS_KEY_VAL;                   // Unlock CS module for register access
    CS->CTL0 = 0;                           // Reset tuning parameters
    CS->CTL0 = CS_CTL0_DCORSEL_3;           // Set DCO to 12MHz (nominal, center of 8-16MHz range)
    CS->CTL1 = CS_CTL1_SELA_2 |             // Select ACLK = REFO
            CS_CTL1_SELS_3 |                // SMCLK = DCO
            CS_CTL1_SELM_3;                 // MCLK = DCO
    CS->KEY = 0;                            // Lock CS module from unintended accesses

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


    UART0_init();
    ADC_init();
    terminal_init();
    location = 0;
    numMeasurements = 0;


    topPeak = 0;
    bottomPeak = 0;
    topPeakTime = 0;
    rising = 0;

    DCoffset = 0;
    peakToPeak = 0;
    trueRMS = 0;
    calcRMS = 0;
    dc = 0;
    frequency = 0;

    while (1)
    {
        /*get adc value*/
        int val = adcReadVal();

        /*convert adc value to mV*/
        val = val / CONVERSION;
        measurements[location] = val;
        location++;
        if (location > numMeasurements)
            numMeasurements = location;

        measureVal(val);
        RMS();
        DC();

        printVals(DCoffset, peakToPeak, trueRMS, calcRMS, dc, frequency);
    }

}

// Timer A0 interrupt service routine
void TA0_0_IRQHandler(void) {
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
    numMeasurements = location;
    location = 0;
}

void measureVal(int val)
{
    if (rising && val > topPeak)
    {
        topPeak = val;
    }

    else if (rising)
    {
        rising = 0;
        bottomPeak = val;
        // take a time
        int time = 0;
        frequency = 1 / (time - topPeakTime);
        topPeakTime = time;
    }

    else if (val > topPeak)
    {
        rising = 1;
    }
    else
    {
        bottomPeak = val;
    }
}

void RMS()
{
    DCoffset = (topPeak + bottomPeak) / 2;
    peakToPeak = topPeak - bottomPeak;
    int sum = 0;
    int counter;
    for (counter = 0; counter < numMeasurements; counter++)
    {
        sum += measurements[counter]*measurements[counter];
    }
    trueRMS = sqrt(sum / counter);
    calcRMS = trueRMS - DCoffset;
    if (calcRMS < 0)
        calcRMS = 0;
}

void DC()
{
    int sum = 0;
    int counter;
    for (counter = 0; counter < numMeasurements; counter++)
    {
        sum += measurements[counter];
    }
    dc = sum / counter;
}
