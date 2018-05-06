#include "msp.h"
#include "uart.h"
#include "adc.h"

#define CONVERSION 1.3

void main() {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;   // Stop watchdog timer

    /*set up UART*/
    UART0_init();
    ADC_init();

    while(1) {
        /*get adc value*/
        int val = adcReadVal();

        /*convert adc value to mV*/
        val = val / CONVERSION;

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
        while (--counter > 3) {
            uartPrint(values[counter]);
        }

        uartPrint('.');
        uartPrint(values[counter--]);
        uartPrint(values[counter--]);
        uartPrint(' ');
        uartPrint('V');
        uartPrint('\r');
        uartPrint('\n');

        for(counter = 0; counter < 100000; counter++);
    }
}
