#include "uart.h"
#include "terminal.h"
#include "format.h"

/*Private variable*/
static int location;

/*print terminal frame*/
void terminal_init() {
    location = 0;

    /*print until end of frame*/
    while (frame[location] != '\0') {
        uartPrint(frame[location++]);
    }
    location = 0;
}

/*Update offset for next terminal push*/
void updateOffset(int offset) {
    /*go to offset position in update*/
    int counter = OFFSET;

    /*set values in update*/
    update[counter] = offset / 100 + '0';
    counter += 2;
    update[counter++] = (offset / 10) % 10 + '0';
    update[counter] = (offset / 1) % 10 + '0';
}

/*Update peak to peak for next terminal push*/
void updatePeak(int peakToPeak) {
    /*go to peak to peak position in update*/
    int counter = PEAK;

    /*set values in update*/
    update[counter] = peakToPeak / 100 + '0';
    counter += 2;
    update[counter++] = (peakToPeak / 10) % 10 + '0';
    update[counter] = (peakToPeak / 1) % 10 + '0';
}

/*update True RMS for next terminal push*/
void updateTrueRMS(int trueRMS) {
    /*go to True RMS location in update*/
    int counter = TRUERMS;

    /*set values in update*/
    update[counter] = trueRMS / 100 + '0';
    counter += 2;
    update[counter++] = (trueRMS / 10) % 10 + '0';
    update[counter] = (trueRMS / 1) % 10 + '0';
}

/*Update Calc RMS for next terminal push*/
void updateCalcRMS(int calcRMS) {
    /*go to Calc RMS location in update*/
    int counter = CALCRMS;

    /*set values in update*/
    update[counter] = calcRMS / 100 + '0';
    counter += 2;
    update[counter++] = (calcRMS / 10) % 10 + '0';
    update[counter] = (calcRMS / 1) % 10 + '0';

    /*print bar graph in update*/
    int numPoints = calcRMS / 8.3;
    for (counter = RMSBAR; counter < RMSBAR + numPoints; counter++) {
        update[counter] = '*';
    }
    /*clear any stars from last update*/
    for ( ; counter < RMSBAR + 53; counter++) {
        update[counter] = ' ';
    }
}

/*Update DC for next terminal push*/
void updateDC(int dc) {
    /*go to dc position in update*/
    int counter = DC;

    /*set values in update*/
    update[counter] = dc / 100 + '0';
    counter += 2;
    update[counter++] = (dc / 10) % 10 + '0';
    update[counter] = (dc / 1) % 10 + '0';

    /*print bar graph in update*/
    int numPoints = dc / 8.3;
    for (counter = DCBAR; counter < DCBAR + numPoints; counter++) {
        update[counter] = '*';
    }
    /*clear any stars from last update*/
    for ( ; counter < DCBAR + 53; counter++) {
        update[counter] = ' ';
    }
}

/*update frequency for next terminal push*/
void updateFrequency(int frequency) {
    /*go to frequency position in update*/
    int counter = FREQUENCY;

    /*set values in update*/
    update[counter++] = (frequency / 1000) % 10 + '0';
    update[counter++] = (frequency / 100) % 10 + '0';
    update[counter++] = (frequency / 10) % 10 + '0';
    update[counter] = frequency % 10 + '0';
}

/*push update to terminal*/
void printVals() {
    /*go to beginning of update if end reached*/
    if (update[location] == '\0')
        location = 0;

    /*send next char in update to UART*/
    uartPrint(update[location++]);
}
