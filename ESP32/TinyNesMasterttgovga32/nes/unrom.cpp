#include "gbGlobals.h"
#include "backend.h"
#include "unrom.h"
#include "dataFlash/gbrom.h"
#include <string.h>

unsigned char prev_bank_prg=255;

static void unrom_switchprg(unsigned char bank)
{
 //Funciona backend_read_from_memory(16 + ((unsigned int)bank<<14), 16384, memory + 0x8000);
 #ifdef use_lib_bank_cache
  if (prev_bank_prg!= bank)
  {
   prev_bank_prg= bank;  
   if (bank<6){
    memcpy(memory + 0x8000,gb_prg_array[bank],16384);
   }
   else{
    backend_read_from_memory(16 + ((unsigned int)bank<<14), 16384, memory + 0x8000);     
   }
  }
 #else
  if (prev_bank_prg!= bank)
  {
   prev_bank_prg= bank;  
   backend_read_from_memory(16 + ((unsigned int)bank<<14), 16384, memory + 0x8000);   
  } 
 #endif


 //int size = 16384;
 ////JJ backend_read(romfn, 16 + (bank * size), size, memory + 0x8000);
 ////memcpy(memory + 0x8000,&gb_list_rom_data[gb_id_cur_rom][(16 + (bank * size))],size);
 //backend_read_from_memory(16 + (bank * size), size, memory + 0x8000);
}

//JJ static void unrom_access(unsigned int address, unsigned char data)
void unrom_access(unsigned int address, unsigned char data)
{
 if (address > 0x7fff && address < 0x10000){
  unrom_switchprg(data);
 }
}

//JJ static void unrom_reset()
void unrom_reset()
{

}

