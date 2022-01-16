#ifndef _MMC3_H
 #define _MMC3_H
 
 #include "gbConfig.h"
 #include "gbGlobals.h"
 
 void mmc3_reset();
 void mmc3_access(unsigned int address, unsigned char data);
 
#endif
 
