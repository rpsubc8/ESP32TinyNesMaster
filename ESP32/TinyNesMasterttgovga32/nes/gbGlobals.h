#ifndef _GB_GLOBALS_H
 #define _GB_GLOBALS_H
 #include "gbConfig.h"
 #include <stdio.h>

struct nes_header
{
    char identity[4];
    char prgromsize;
    char chrromsize;
    char flags6;
    char flags7;
    char prgramsize;
    char flags9;
    char flags10;
    char zero[5];
};
//JJ} header;

extern struct nes_header header;

 
 //extern struct nes_header header;
 //JJ extern unsigned char memory[65536];
 //extern unsigned char * memory;
 extern unsigned char memory[65536];
 extern int pad1_state[8];
 extern int pad1_readcount;
 extern int running;
 extern char *romfn;
 extern int OS_MIRROR;
 extern int SRAM;
 extern int MIRRORING;
 
 extern unsigned char ppu_memory[16384];
 
 extern unsigned int ppu_addr;
 extern unsigned int ppu_lastdata;
 extern unsigned int ppu_register_t;
 extern unsigned int ppu_register_v;
 extern unsigned int ppu_register_x;
 extern unsigned int ppu_register_w;
 extern unsigned int ppu_sprite_addr;
 #ifdef use_lib_resizeram_ppu
  extern unsigned char ppu_sprite_memory[256];
  //extern unsigned char ppu_sprite_cache[264][33];//(256+8)DIV 8
  //extern unsigned char ppu_background_cache[264][33];//(256+8)DIV 8
 #else
  extern unsigned char ppu_sprite_memory[256];
  extern unsigned char ppu_sprite_cache[256 + 8][256 + 8];
  extern unsigned char ppu_background_cache[256 + 8][256 + 8];
 #endif 
 
 
 
 extern unsigned char gb_show_osd_main_menu;
 //extern SDL_Surface *screen;
 //extern SDL_Event event;
 extern unsigned char gb_load_new_rom;
 extern unsigned char gb_salir;
 
 
 extern int mmc3_irq_enable;
 extern int mmc3_irq_counter; 

 #ifdef use_lib_rom_memory
  extern unsigned char gb_id_cur_rom; //rom actual
 #endif












 extern volatile unsigned char keymap[256];
 extern volatile unsigned char oldKeymap[256];

 extern unsigned char gb_current_ms_poll_sound; //milisegundos muestreo
 extern unsigned char gb_current_ms_poll_keyboard; //milisegundos muestreo teclado
 extern unsigned char gb_current_frame_crt_skip; //el actual salto de frame
 extern unsigned char gb_current_delay_emulate_ms; //la espera en cada iteracion
 extern unsigned char gb_sdl_blit;
 extern unsigned char gb_screen_xOffset;


 extern unsigned char **gb_buffer_vga;
 extern unsigned int **gb_buffer_vga32;


 extern unsigned char gb_color_vga[128];
 extern unsigned char gb_cache_paleta_id[64];

 #ifdef use_lib_bank_cache
  //8 bancos prg de 16384 bytes
  extern unsigned char *gb_prg_array[8];
 #endif 


 #ifdef use_lib_sound_ricoh2A03
  extern unsigned char gb_silence_all_channels;
  extern unsigned char gbShiftLeftVolumen;
  extern unsigned char gb_mute_sound; 
  extern unsigned char gbVolMixer_now[3];
  extern unsigned short int gbFrecMixer_now[3];
 #endif

 extern unsigned char gb_auto_delay_cpu; //Auto ajustar velocidad CPU 20 ms

 extern unsigned char prev_bank_prg;

#endif
 
