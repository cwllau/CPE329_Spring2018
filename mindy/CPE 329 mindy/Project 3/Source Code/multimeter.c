#include "multimeter.h"
#include "terminal.h"
#include "msp.h"
#include <math.h>

#define MAXMEASUREMENTS 30000 /*adjust number of measurements taken*/
#define FREQUENCYMULTIPLIER 5000 /*adjust for frequency error*/
#define MAXFREQUENCIES 10

/*Private functions*/
void readPeak();
void readVoltage();

/*Private variables*/
static short values[MAXMEASUREMENTS];
static int location;
static short myMid;
static short lastReading;
static int lastTime;
static short myMin;
static short myMax;

/*Multimeter setup*/
void multimeter_init() {
    int counter;

    lastTime = 0;
    lastReading = 4000;
    location = 0;
    for (counter = 0; counter < MAXMEASUREMENTS; counter++)
        values[counter] = 0;
}

/*Reads a value and time for multimeter*/
void readVol(short val) {
    /*Add value to array of past values*/
    values[location++] = val;

    /*Avoid array overflow*/
    if (location >= MAXMEASUREMENTS) {
        location = 0;

        /*Perform calculations on values*/
        readVoltage();
        readPeak();

    }
}


/*Finds the peak to peak of the array*/
void readPeak() {
    updatePeak(myMax - myMin);
}

/*Finds the RMS values of the array*/
void readVoltage() {
    unsigned long long sum = 0;
    int counter;
    myMin = 4000;
    myMax = 0;
    unsigned long long DCsum = 0;
    /*Sum square of all values in array*/
    for (counter = 0; counter < MAXMEASUREMENTS; counter++)
    {
        sum = sum + (values[counter]*values[counter]);
        DCsum += values[counter];
        /*Check for minimum and maximum values*/
        if (values[counter] < myMin)
            myMin = values[counter];
        if (values[counter] > myMax)
            myMax = values[counter];
    }

    myMid = (myMin + myMax) / 2;
    /*calculate rms*/
    sum = sum / counter;
    unsigned long long temp = sqrt(sum);

    /*update values in terminal*/
    updateDC(DCsum / counter);

    updateOffset(DCsum / counter);
    updateTrueRMS(temp);
    updateCalcRMS(temp - (DCsum / counter));
}

/*Finds the frequency of the waveform*/
void readFrequency(short val, int time) {
    if (lastReading < myMid && val > myMid) {
        int newTime = time - lastTime;
        if (newTime < 0)
            newTime += 65384;
        updateFrequency(FREQUENCYMULTIPLIER / newTime);
        lastTime = time;
    }
    lastReading = val;
}
