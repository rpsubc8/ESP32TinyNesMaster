#ifndef _APU_H
 #define _APU_H
 
 #include "gbConfig.h"
 #include "gbGlobals.h"
 
 unsigned char apu_memread(unsigned int address);
 unsigned char apu_memwrite(unsigned int address, unsigned char data);
 
#endif

 
