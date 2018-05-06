#include "msp.h"
#include "delay.h"

static void set_DCO(int freq) {
    CS->KEY = CS_KEY_VAL;

    CS->CTL0 = 0;

    if (freq == FREQ_1_5_MHz)
       CS->CTL0 = CS_CTL0_DCORSEL_0;
    else if (freq == FREQ_3_MHz)
       CS->CTL0 = CS_CTL0_DCORSEL_1;
    else if (freq == FREQ_6_MHz)
       CS->CTL0 = CS_CTL0_DCORSEL_2;
    else if (freq == FREQ_12_MHz)
       CS->CTL0 = CS_CTL0_DCORSEL_3;
    else if (freq == FREQ_24_MHz)
       CS->CTL0 = CS_CTL0_DCORSEL_4;
    else if (freq == FREQ_48_MHz) {
       // setting Vcore to level 1 for 48 MHz operation
       while(PCM->CTL1 & PCM_CTL1_PMR_BUSY);
       PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR_1;
       while(PCM->CTL1 & PCM_CTL1_PMR_BUSY);

       // configure flash wait-state to 1 for both banks 0 & 1
       FLCTL->BANK0_RDCTL = (FLCTL->BANK0_RDCTL &
           ~(FLCTL_BANK0_RDCTL_WAIT_MASK)) | FLCTL_BANK0_RDCTL_WAIT_1;

       FLCTL->BANK1_RDCTL = (FLCTL->BANK1_RDCTL &
                  ~(FLCTL_BANK1_RDCTL_WAIT_MASK)) | FLCTL_BANK1_RDCTL_WAIT_1;

       CS->CTL0 = CS_CTL0_DCORSEL_5;
    }

    CS->CTL1 &= ~(CS_CTL1_SELM_MASK | CS_CTL1_DIVM_MASK) | CS_CTL1_SELM_3;

    CS->KEY = 0;
}

// Delay milliseconds function
void delay_ms(int ms, int freq) {
   int i, j;

   set_DCO(freq);

   for (i = 0; i < ms; i++)
      for (j = .001 * freq / 10; j > 0; j--);   // delay 1 ms (approx)
}

// delay microseconds function
void delay_us(int us, int freq) {
   int i, j;

   set_DCO(freq);

   for (i = 0; i < us; i++)
      for (j = .00001 * freq / 10; j > 0; j--);    // delay 1 us (approx)
}
