#include "msp.h"

#define MAX_VALUE 4096
#define THREE_V   1890
#define TWO_V     1280
#define ONE_V     650

#define TIMER_OFFSET 60000

volatile unsigned int Counter = 0;
volatile unsigned int TempDAC_Value = THREE_V;

void Drive_DAC(unsigned int level){
  unsigned int DAC_Word = 0;
  int i;

  DAC_Word = (0x1000) | (level & 0x0FFF);   // 0x1000 sets DAC for Write
                                            // to DAC, Gain = 2, /SHDN = 1
                                            // and put 12-bit level value
                                            // in low 12 bits.

  P4->OUT &= ~BIT1;                                   // Clear P4.1 (drive /CS low on DAC)
                                                      // Using a port output to do this for now

  EUSCI_B0->TXBUF = (unsigned char) (DAC_Word >> 8);  // Shift upper byte of DAC_Word
                                                      // 8-bits to right

  while (!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG));              // USCI_A0 TX buffer ready?

  EUSCI_B0->TXBUF = (unsigned char) (DAC_Word & 0x00FF);     // Transmit lower byte to DAC

  while (!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG));      // Poll the TX flag to wait for completion

  for(i = 200; i > 0; i--);                          // Delay 200 16 MHz SMCLK periods
                                                     //to ensure TX is complete by SIMO

  P4->OUT |= BIT1;                                   // Set P4.1   (drive /CS high on DAC)
}

void TA0_0_IRQHandler(void) {
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
    TIMER_A0->CCR[0] += TIMER_OFFSET;              // Add Offset to TACCR0

    Counter++;

    if (Counter == 4) {
        TempDAC_Value = TempDAC_Value == THREE_V ? ONE_V : THREE_V;
        Counter = 0;
    }

    Drive_DAC(TempDAC_Value);
}

int main() {
  WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;   // Stop watchdog timer

  // DCO = 24 MHz, SMCLK and MCLK = DCO
  CS->KEY = CS_KEY_VAL;
  CS->CTL0 = CS_CTL0_DCORSEL_4;   // DCO = 24 MHz
  CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
  CS->KEY = 0;

  // Configure port bits for SPI
  P4->DIR |= BIT1;               // Will use BIT4 to activate /CE on the DAC
  P1->SEL0 |= (BIT6 | BIT5);     // Configure P1.6 and P1.5 for UCB0SIMO and UCB0CLK
  P1->SEL1 &= ~(BIT6 | BIT5);    //p1.6 UCBOSIMO mcp4, p1.5 UCB0CLK mcp3

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
  TIMER_A0->CCR[0] = TIMER_OFFSET;



  TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | // SMCLK, continuous mode
                  TIMER_A_CTL_MC__CONTINUOUS;

  SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;    // Enable sleep on exit from ISR

  // Enable global interrupt
  __enable_irq();

  NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);

  while (1) {
      __sleep();

      __no_operation();
  }

}

