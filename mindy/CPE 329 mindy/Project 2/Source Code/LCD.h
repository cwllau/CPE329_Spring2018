/*
 * LCD.h
 *
 * Drivers for the LCD
 *  Created on: Apr 17, 2017
 *      Author: Brendan
 */

#ifndef LCD_H_
#define LCD_H_
#define RS 1     /* P4.0 mask */
#define EN 4     /* P4.2 mask */

void delayMs(int n);
void LCD_nibble_write(unsigned char data, unsigned char control);
void LCD_command(unsigned char command);
void LCD_data(unsigned char data);
void LCD_init(void);


#endif /* LCD_H_ */
