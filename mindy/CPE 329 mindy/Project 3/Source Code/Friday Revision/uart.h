#ifndef UART_H_
#define UART_H_

/* Sets up UART0
 */
void UART0_init();

/* Returns a positive number if a new number has been read
 * since last reset, 0 otherwise
 */

void uartPrint(char c);

#endif /* UART_H_ */
