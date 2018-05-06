#ifndef LCD_H
#define LCD_H

void init_pins_LCD();

void Startup_LCD();

void Clear_LCD();

void Home_LCD();

void Write_char_LCD(char c);

void HelloWorld();

void Write_string_LCD(char word[256]);

#endif
