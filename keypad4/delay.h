#ifndef DELAY_H
#define DELAY_H

#define FREQ_1_5_MHz 1500000
#define FREQ_3_MHz 3000000
#define FREQ_6_MHz 6000000
#define FREQ_12_MHz 12000000
#define FREQ_24_MHz 24000000
#define FREQ_48_MHz 48000000

void delay_ms(int ms, int freq);

void delay_us(int ms, int freq);

#endif
