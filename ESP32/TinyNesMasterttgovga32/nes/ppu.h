#ifndef _PPU_H
 #define _PPU_H

 #include "gbConfig.h"
 #include "gbGlobals.h"

#define PPUCTRL                         0x2000
#define PPUMASK                         0x2001

#define PPUSTATUS                       0x2002
#define PPUOAMADDR                      0x2003
#define PPUOAMDATA                      0x2004
#define PPUSCROLL                       0x2005
#define PPUADDR                         0x2006
#define PPUDATA                         0x2007
#define PPUCTRL_NAMETABLE               (memory[PPUCTRL] & 0x00)
#define PPUCTRL_INCREMENT               (memory[PPUCTRL] & 0x04)
#define PPUCTRL_SPRITEHI                (memory[PPUCTRL] & 0x08)
#define PPUCTRL_BACKGROUNDHI            (memory[PPUCTRL] & 0x10)
#define PPUCTRL_SPRITE16                (memory[PPUCTRL] & 0x20)
#define PPUCTRL_VBLANKNMI               (memory[PPUCTRL] & 0x80)
#define PPUMASK_MONOCHROME              (memory[PPUMASK] & 0x01)
#define PPUMASK_BACKGROUNDCLIP          (memory[PPUMASK] & 0x02)
#define PPUMASK_SPRITECLIP              (memory[PPUMASK] & 0x04)
#define PPUMASK_BACKGROUNDSHOW          (memory[PPUMASK] & 0x08)
#define PPUMASK_SPRITESHOW              (memory[PPUMASK] & 0x10)
#define PPUSTATUS_VRAMWRITEFLAG         (memory[PPUSTATUS] & 0x10)
#define PPUSTATUS_SPRITEOVERFLOW        (memory[PPUSTATUS] & 0x20)
#define PPUSTATUS_SPRITE0HIT            (memory[PPUSTATUS] & 0x40)
#define PPUSTATUS_VBLANKON              (memory[PPUSTATUS] & 0x80)


 //JJ unsigned char ppu_memread(unsigned int address);
 unsigned char ppu_memread(unsigned short int address);
 //JJ unsigned char ppu_memwrite(unsigned int address, unsigned char data);
 unsigned char ppu_memwrite(unsigned short int address, unsigned char data);
 void ppu_checkspritehit(int width, int scanline);
 void ppu_renderbackground(int scanline);
 void ppu_rendersprites();

#endif
 
