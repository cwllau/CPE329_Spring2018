#include "msp.h"
#include "uart.h"
#include "adc.h"
#include <math.h>

#define CONVERSION 1.3

void measureVal(int val);
void RMS();
void DC();

static int topPeak;
static int bottomPeak;
static int topPeakTime;
static int rising;
static int DCoffset;
static int peakToPeak;
static short* measurements;
static int location;
static int trueRMS;
static int calcRMS;
static int dc;
static int frequency;

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

    UART0_init();
    ADC_init();
    measurements = calloc(12000000, sizeof(short));
    location = 0;

    while(1) {
            /*get adc value*/
            int val = adcReadVal();

            /*convert adc value to mV*/
            val = val / CONVERSION;
            measurements[location] = val;
            location++;

            /*read value into a character array in reverse order
             * each character formatted into ascii
             */
            char values[20];
            int counter;
            for (counter = 0; val; counter++) {
                values[counter] = (val % 10) + '0';
                val = val / 10;
            }

            /*echo characters in uart*/
            while (counter > 3) {
                uartPrint(values[--counter]);
            }
            uartPrint('.');
            while (--counter >= 0) {
                uartPrint(values[counter]);
            }
            uartPrint(' ');
            uartPrint('V');
            uartPrint('\r');
            uartPrint('\n');

            for(counter = 0; counter < 100000; counter++);
        }

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
    int counter = location;
    while (counter--)
    {
        sum += measurements[counter]*measurements[counter];
    }
    trueRMS = sqrt( 1 / location * sum);
    calcRMS = trueRMS - DCoffset;
}

void DC()
{
    int sum = 0;
    int counter = location;
    while (counter--)
    {
        sum += measurements[counter];
    }
    dc = sum / location;
}
