#ifndef UART_H_
#define UART_H_

/* Sets up UART0
 */
void UART0_init();

/* Returns a positive number if a new number has been read
 * since last reset, 0 otherwise
 */
int getFlag();

/* Returns last number read
 */
int getNumber();

/* Resets flag for new number read
 */
void resetFlag();

#endif /* UART_H_ */
