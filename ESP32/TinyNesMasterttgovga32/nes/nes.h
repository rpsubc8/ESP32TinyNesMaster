#ifndef _NES_H
 #define _NES_H

 #include "hardware.h"
 //#include "MartianVGA.h"

 // Declared vars
 //extern VGA6Bit vga;

 // Declared methods
 void do_keyboard();
 //unsigned char ram_read(unsigned int address);
 //void ram_write(unsigned int address,unsigned char data);
 void input_set(int key);
 void input_clear(int key);
 void halt(); 
#endif
