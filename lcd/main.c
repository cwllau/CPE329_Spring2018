#include "msp.h"
#include "LCD.h"
#include "delay.h"

void main() {
    init_pins_LCD();

    Startup_LCD();

    Clear_LCD();

    Home_LCD();

    //Write_char_LCD('w');

    HelloWorld();

    //Write_string_LCD("hello");
}
