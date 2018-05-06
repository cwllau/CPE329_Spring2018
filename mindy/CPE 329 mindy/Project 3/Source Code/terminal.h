#ifndef TERMINAL_H_
#define TERMINAL_H_

/*initialize terminal*/
void terminal_init();
/*update offset for next push to terminal*/
void updateOffset(int offset);
/*update peak to peak for next push to terminal*/
void updatePeak(int peakToPeak);
/*update True RMS for next push to terminal*/
void updateTrueRMS(int trueRMS);
/*update Calc RMS for next push to terminal*/
void updateCalcRMS(int calcRMS);
/*update DC readings for next push to terminal*/
void updateDC(int dc);
/*update frequency for next push to terminal*/
void updateFrequency(int frequency);
/*push next char from update to terminal*/
void printVals();

#endif /* TERMINAL_H_ */
