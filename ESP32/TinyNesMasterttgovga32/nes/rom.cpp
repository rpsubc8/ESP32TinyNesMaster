#include "gbConfig.h"
#include <stdlib.h>
#include <string.h>
#include "nes.h"
#include "cpu.h"
#include "rom.h"
#include "backend.h"
#ifdef use_lib_rom_memory
 #include "dataFlash/gbrom.h"
#endif 
#include "Arduino.h"

#ifdef use_lib_rom_memory
 int rom_load_from_memory(unsigned char id, struct nes_header *header, unsigned char *ram_mem, unsigned char *ppu_mem)
 {  
  #ifdef use_lib_log_serial
   Serial.printf("Rom memoria INI\n");
  #endif 
  char identity[4] = {'N', 'E', 'S', 0x1A};
  memcpy(header,gb_list_rom_data[id],16);
  if (memcmp(header->identity, identity, 4)){
   return 1;
  }
  memcpy(ram_mem + 0x8000,&gb_list_rom_data[id][16],16384);
  memcpy(ram_mem + 0xC000,&gb_list_rom_data[id][(16 + ((header->prgromsize - 1) * 16384))],16384);
  if (header->chrromsize){
   memcpy(ppu_mem,&gb_list_rom_data[id][(16 + (header->prgromsize * 16384))],8192);
  }
  #ifdef use_lib_log_serial
   Serial.printf("Rom memoria cargada\n");
  #endif 
  return 0;
 }
#endif

//**********************************************************************************
/*
int rom_load(char *romfn, struct nes_header *header, unsigned char *ram_mem, unsigned char *ppu_mem)
{

    char identity[4] = {'N', 'E', 'S', 0x1A};

    backend_read(romfn, 0, 16, header);

    if (memcmp(header->identity, identity, 4))
        return 1;

    backend_read(romfn, 16, 16384, ram_mem + 0x8000);
    backend_read(romfn, 16 + ((header->prgromsize - 1) * 16384), 16384, ram_mem + 0xC000);

    if (header->chrromsize)
        backend_read(romfn, 16 + (header->prgromsize * 16384), 8192, ppu_mem);

    return 0;

}
*/
