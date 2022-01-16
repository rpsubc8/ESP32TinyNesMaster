#ifndef _CPU_H
 #define _CPU_H
#include "gbConfig.h"

//JJ int cpu_irq(int cycles, unsigned char *memory);
int cpu_irq(int cycles);
//JJ int cpu_nmi(int cycles, unsigned char *memory);
int cpu_nmi(int cycles);
//JJ void cpu_reset(unsigned char *memory);
void cpu_reset(void);
//JJ int cpu_execute(int cycles, unsigned char *memory);
int cpu_execute(int cycles);

#endif
