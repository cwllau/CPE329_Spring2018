#ifndef MULTIMETER_H_
#define MULTIMETER_H_

/*initialize multimeter*/
void multimeter_init();
/*adjust multimeter for most recently read value*/
void readVol(short val);
void readFrequency(short val, int time);

#endif /* MULTIMETER_H_ */
