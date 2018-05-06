#include "uart.h"
#include "terminal.h"

void printVals(DCoffset, peakToPeak, trueRMS, calcRMS, dc, frequency) {
    int counter;
    int numPoints;

    //set to dc bar graph
    uartPrint(0x1B);
    uartPrint('[');
    uartPrint('4');
    uartPrint(';');
    uartPrint('1');
    uartPrint('6');
    uartPrint('H');

    //print dc bar graph
    numPoints = dc / 83;
    for (counter = 0; counter < numPoints; counter++) {
        uartPrint('*');
    }
    for ( ; counter < 53; counter++) {
        uartPrint(' ');
    }

    //set to rms bar graph
    uartPrint(0x1B);
    uartPrint('[');
    uartPrint('5');
    uartPrint(';');
    uartPrint('1');
    uartPrint('6');
    uartPrint('H');

    //print rms bar graph
    numPoints = calcRMS / 83;
    for (counter = 0; counter < numPoints; counter++) {
        uartPrint('*');
    }
    for ( ; counter < 53; counter++) {
        uartPrint(' ');
    }

    //set to TrueRMS
    uartPrint(0x1B);
    uartPrint('[');
    uartPrint('1');
    uartPrint('1');
    uartPrint(';');
    uartPrint('1');
    uartPrint('5');
    uartPrint('H');

    //print TrueRMS
    uartPrint(trueRMS / 1000 + '0');
    uartPrint('.');
    uartPrint(((trueRMS / 100) % 10) + '0');
    uartPrint(((trueRMS / 10) % 10) + '0');

    //set to Frequency
    uartPrint(0x1B);
    uartPrint('[');
    uartPrint('1');
    uartPrint('2');
    uartPrint(';');
    uartPrint('1');
    uartPrint('5');
    uartPrint('H');

    //print frequency
    uartPrint(frequency / 1000 + '0');
    uartPrint(((frequency / 100) % 10) + '0');
    uartPrint(((frequency / 10) % 10) + '0');
    uartPrint((frequency % 10) + '0');

    //set to peakpeak
    uartPrint(0x1B);
    uartPrint('[');
    uartPrint('1');
    uartPrint('3');
    uartPrint(';');
    uartPrint('1');
    uartPrint('5');
    uartPrint('H');

    //print peakpeak
    uartPrint(peakToPeak / 1000 + '0');
    uartPrint('.');
    uartPrint(((peakToPeak / 100) % 10) + '0');
    uartPrint(((peakToPeak / 10) % 10) + '0');

    //set to offset
    uartPrint(0x1B);
    uartPrint('[');
    uartPrint('1');
    uartPrint('4');
    uartPrint(';');
    uartPrint('1');
    uartPrint('5');
    uartPrint('H');

    //print offset
    uartPrint(DCoffset / 1000 + '0');
    uartPrint('.');
    uartPrint(((DCoffset / 100) % 10) + '0');
    uartPrint(((DCoffset / 10) % 10) + '0');

    //set to calcrms
    uartPrint(0x1B);
    uartPrint('[');
    uartPrint('1');
    uartPrint('5');
    uartPrint(';');
    uartPrint('1');
    uartPrint('5');
    uartPrint('H');

    //print calcrms
    uartPrint(calcRMS / 1000 + '0');
    uartPrint('.');
    uartPrint(((calcRMS / 100) % 10) + '0');
    uartPrint(((calcRMS / 10) % 10) + '0');

    //set to dc
    uartPrint(0x1B);
    uartPrint('[');
    uartPrint('7');
    uartPrint(';');
    uartPrint('1');
    uartPrint('5');
    uartPrint('H');

    //print dc
    uartPrint(dc / 1000 + '0');
    uartPrint('.');
    uartPrint(((dc / 100) % 10) + '0');
    uartPrint(((dc / 10) % 10) + '0');
}

void terminal_init() {
    int counter = 0;

    uartPrint(0x1B);
    uartPrint('[');
    uartPrint('?');
    uartPrint('2');
    uartPrint('5');
    uartPrint('l');

    uartPrint(0x1B);
    uartPrint('[');
    uartPrint('H');

    //Print line by line
    char *line =
            "|-------------------------------------------------------------------|";
    while (line[counter])
        uartPrint(line[counter++]);
    uartPrint('\r');
    uartPrint('\n');

    counter = 0;
    line =
            "| Voltage (V) |0   0.5   1.0   1.5   2.0   2.5   3.0   3.5   4.0    |";
    while (line[counter])
        uartPrint(line[counter++]);
    uartPrint('\r');
    uartPrint('\n');

    counter = 0;
    line =
            "|-------------|-----------------------------------------------------|";
    while (line[counter])
        uartPrint(line[counter++]);
    uartPrint('\r');
    uartPrint('\n');

    counter = 0;
    line =
            "| DC          |                                                     |";
    while (line[counter])
        uartPrint(line[counter++]);
    uartPrint('\r');
    uartPrint('\n');

    counter = 0;
    line =
            "| CalcRMS     |                                                     |";
    while (line[counter])
        uartPrint(line[counter++]);
    uartPrint('\r');
    uartPrint('\n');

    counter = 0;
    line =
            "|-------------------------------------------------------------------|";
    while (line[counter])
        uartPrint(line[counter++]);
    uartPrint('\r');
    uartPrint('\n');

    counter = 0;
    line =
            "| DC Value  : 0.00 V                                                |";
    while (line[counter])
        uartPrint(line[counter++]);
    uartPrint('\r');
    uartPrint('\n');

    counter = 0;
    line =
            "|-------------------------------------------------------------------|";
    while (line[counter])
        uartPrint(line[counter++]);
    uartPrint('\r');
    uartPrint('\n');
    counter = 0;

    counter = 0;
    line =
            "| AC Values                                                         |";
    while (line[counter])
        uartPrint(line[counter++]);
    uartPrint('\r');
    uartPrint('\n');

    counter = 0;
    line =
            "|-------------------------------------------------------------------|";
    while (line[counter])
        uartPrint(line[counter++]);
    uartPrint('\r');
    uartPrint('\n');

    counter = 0;
    line =
            "| True RMS  : 0.00 V                                                |";
    while (line[counter])
        uartPrint(line[counter++]);
    uartPrint('\r');
    uartPrint('\n');

    counter = 0;
    line =
            "| Frequency : 0000 Hz                                               |";
    while (line[counter])
        uartPrint(line[counter++]);
    uartPrint('\r');
    uartPrint('\n');

    counter = 0;
    line =
            "| Peak-Peak : 0.00 V                                                |";
    while (line[counter])
        uartPrint(line[counter++]);
    uartPrint('\r');
    uartPrint('\n');

    counter = 0;
    line =
            "| DC Offset : 0.00 V                                                |";
    while (line[counter])
        uartPrint(line[counter++]);
    uartPrint('\r');
    uartPrint('\n');

    counter = 0;
    line =
            "| CalcRMS   : 0.00 V                                                |";
    while (line[counter])
        uartPrint(line[counter++]);
    uartPrint('\r');
    uartPrint('\n');

    counter = 0;
    line =
            "|-------------------------------------------------------------------|";
    while (line[counter])
        uartPrint(line[counter++]);
}
