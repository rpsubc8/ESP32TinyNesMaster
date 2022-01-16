#ifndef _ROM_H
 #define _ROM_H
 
#include "gbConfig.h"
#include "gbGlobals.h"
enum flags6
{

    FLAGS6_HMIRROR                      = (1 << 0),
    FLAGS6_SRAM                         = (1 << 1),
    FLAGS6_TRAINER                      = (1 << 2)

};

enum flags7
{

    FLAGS7_UNISYSTEM                    = (1 << 0),
    FLAGS7_PLAYCHOICE                   = (1 << 1),
    FLAGS7_FORMAT                       = (2 << 2)

};

enum flags9
{

    FLAGS9_PAL                          = (1 << 0)

};

enum flags10
{

    FLAGS10_PAL                         = (1 << 1),
    FLAGS10_SRAM                        = (1 << 4),
    FLAGS10_BUSCONFLICT                 = (1 << 5)

};

//JJ struct nes_header
//JJ {
//JJ 
//JJ     char identity[4];
//JJ     char prgromsize;
//JJ     char chrromsize;
//JJ     char flags6;
//JJ     char flags7;
//JJ     char prgramsize;
//JJ     char flags9;
//JJ     char flags10;
//JJ     char zero[5];
//JJ 
//JJ };

#ifdef use_lib_rom_memory
 int rom_load_from_memory(unsigned char id, struct nes_header *header, unsigned char *ram_mem, unsigned char *ppu_mem);
#endif 
 
//int rom_load(char *romfn, struct nes_header *header, unsigned char *ram_mem, unsigned char *ppu_mem);

#endif
