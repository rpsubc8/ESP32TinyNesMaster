//Port Jens Nyberg emulator to TTGO VGA32 by ackerman:
// Load Roms
// MODE320x240
// Mode 64 colors RGB 6 bits
// Tiny VGA library bitluni 0.3.3 (include in project)
// gbConfig options configuration compile
// Sonido Activar use_lib_sound_ricoh2A03
// Botones:
//  Tecla X - A
//  Tecla Z - B
//  Tecla C - SELECT
//  Tecla V - START
//  Tecla Cursores - arriba, abajo, izquierda, derecha

#include "gbConfig.h"
#include <Arduino.h>
#include "PS2Kbd.h"
#ifdef use_lib_sound_ricoh2A03
 #include "fabgl.h" //Para fabgl
 #include "fabutils.h" //Para fabgl
 //#include "WiFiGeneric.h" //Quitar para dependencia al compilar
#endif 
#include "gbGlobals.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include "ppu.h"
#include "nes.h"
#include "cpu.h"
#include "rom.h"
#include "backend.h"
//#include "MartianVGA.h"
//#include "def/Font.h"
#include "hardware.h"
#include "driver/timer.h"
#include "soc/timer_group_struct.h"
#include "dataFlash/gbrom.h"
#include "PS2KeyCode.h"
#include "osd.h"
#include "sdl.h"
#include "vga_6bit.h"





#ifdef use_lib_measure_time
 unsigned int jj_ini_medir_cpu,jj_end_medir_cpu;
 unsigned int gb_max_cpu_ticks,gb_min_cpu_ticks,gb_cur_cpu_ticks;
 unsigned int gb_cpu_timer_before=0,gb_cpu_timer_cur=0;
#endif 

 #ifdef use_lib_vga8colors
  //DAC 3 bits 8 colores
  // 3 bit pins  
 static const unsigned char pin_config[] = {  
  PIN_RED_HIGH,
  PIN_GREEN_HIGH,  
  PIN_BLUE_HIGH,
  255,
  255,
  255,
  PIN_HSYNC,
  PIN_VSYNC
 };
 #else
  //DAC 6 bits 64 colores
  static const unsigned char pin_config[] = {
   PIN_RED_LOW,
   PIN_RED_HIGH,
   PIN_GREEN_LOW,
   PIN_GREEN_HIGH,
   PIN_BLUE_LOW,
   PIN_BLUE_HIGH,
   PIN_HSYNC,
   PIN_VSYNC
  };
 #endif

 unsigned char gb_sync_bits;

#ifdef use_lib_bank_cache
 unsigned char *gb_prg_array[8];
#endif
 
unsigned char gb_load_new_rom=1;
unsigned char gb_salir=0;
int gb_currentTime,gb_vgaTime,gb_keyboardTime; //Tiempo ms vga
int gb_jj_cur_cycle=0;
int gb_jj_ini_cycle=0;
int gb_jj_cur_fps=0;
int gb_jj_ini_fps=0;
int gb_jj_time_ini_fps=0;
unsigned char gb_process_frame=0;

#ifdef use_lib_rom_memory
 unsigned char gb_id_cur_rom= 0;
#endif 

struct nes_header header;
//JJunsigned char memory[65536];
//unsigned char * memory;
unsigned char memory[65536];
int pad1_state[8];
int pad1_readcount = 0;
int running = 1;
char *romfn;
int OS_MIRROR = 0;
int SRAM;
int MIRRORING;

unsigned char ppu_memory[16384];
unsigned int ppu_addr = 0x2000;
unsigned int ppu_lastdata = 0;
unsigned int ppu_register_t = 0;
unsigned int ppu_register_v = 0;
unsigned int ppu_register_x = 0;
unsigned int ppu_register_w = 0;
unsigned int ppu_sprite_addr = 0x00;
#ifdef use_lib_resizeram_ppu
 unsigned char ppu_sprite_memory[256];
 //unsigned char ppu_sprite_cache[264][33];
 //unsigned char ppu_background_cache[264][33];
#else
 unsigned char ppu_sprite_memory[256];
 unsigned char ppu_sprite_cache[256 + 8][256 + 8];
 unsigned char ppu_background_cache[256 + 8][256 + 8];
#endif 

struct mapper
{

    void (*reset)();
    void (*access)(unsigned int address, unsigned char data);

};

//#include "ppu.h"
#include "apu.h"
#include "mmc1.h"
#include "unrom.h"
#include "cnrom.h"
#include "mmc3.h"

struct mapper mappers[256] = {
    {0 , 0},
    {mmc1_reset, mmc1_access},
    {unrom_reset, unrom_access},
    {cnrom_reset, cnrom_access},
    {mmc3_reset, mmc3_access}
};

struct mapper *mapper = &mappers[0];





//VGA6Bit vga;

unsigned char gb_run_emulacion = 1; //Ejecuta la emulacion
unsigned char gb_current_ms_poll_sound = gb_ms_sound;
unsigned char gb_screen_xOffset=0;
unsigned char gb_current_delay_emulate_ms= (gb_auto_delay_cpu==1)?1:gb_delay_emulate_ms;
unsigned char gb_auto_delay_cpu= use_lib_delay_tick_cpu_auto;
static unsigned long gb_time_ini_espera;
//static unsigned long gb_currentTime;
//static unsigned long gb_sdl_time_sound_before;
//static unsigned long gb_keyboardTime;
static unsigned long gb_time_ini_beep;
unsigned char gb_current_sel_rom=0;



//unsigned char gb_current_delay_emulate_ms= gb_delay_emulate_ms;
unsigned char gb_current_ms_poll_keyboard = gb_ms_keyboard;
unsigned char gb_delay_t=0;
unsigned char gb_delay_sound=0;

static unsigned char gbVolMixer=0; //Beep
static unsigned char gbCont=0;


static unsigned int gb_stats_time_cur= 0;
static unsigned int gb_stats_time_min= 500000;
static unsigned int gb_stats_time_max= 0;

//int frame = 0;
unsigned char **gb_buffer_vga;
unsigned int **gb_buffer_vga32;
//static unsigned long gb_vgaTime;


#ifdef use_lib_sound_ricoh2A03
 
 SoundGenerator soundGenerator;
 SineWaveformGenerator gb_sineArray[3];
 
 unsigned char gbVolMixer_before[3]={0,0,0};
 unsigned short int gbFrecMixer_before[3]={0,0,0};
 unsigned char gbVolMixer_now[3]={0,0,0};
 unsigned short int gbFrecMixer_now[3]={0,0,0}; 
 unsigned char gb_silence_all_channels=1; 
 unsigned char gbShiftLeftVolumen=0; //Maximo volumen shift left 2
 unsigned char gb_mute_sound=0;
 
 static unsigned int gb_sdl_time_sound_before;
  
 void sound_cycleFabgl(void);
 //void jj_mixpsg(void);
 void SilenceAllChannels(void);
#endif


#ifdef use_lib_sound_ricoh2A03
 void SilenceAllChannels()
 {  
  for (unsigned char i=0;i<3;i++)  
  {
   gb_sineArray[i].setFrequency(0);
   gb_sineArray[i].setVolume(0);
   gbVolMixer_before[i] = gbVolMixer_now[i] = 0;
   gbFrecMixer_before[i] = gbFrecMixer_now[i] = 0;
  }  
 }

 inline void sound_cycleFabgl()
 {  
  //AY8912
  for (unsigned char i=0;i<3;i++)
  {
   if (gbVolMixer_now[i] != gbVolMixer_before[i])
   {
    //gb_sineArray[i].setVolume((gbVolMixer_now[i]<<2));    
    switch (gbShiftLeftVolumen)
    {
      case 0: gb_sineArray[i].setVolume((gbVolMixer_now[i]<<2)); break;
      //case 0: gb_sineArray[i].setVolume((gbVolMixer_now[i]<<4)); break;
      case 1: gb_sineArray[i].setVolume((gbVolMixer_now[i]<<1)); break;
      case 2: gb_sineArray[i].setVolume((gbVolMixer_now[i])); break;
      case 3: gb_sineArray[i].setVolume((gbVolMixer_now[i]>>1)); break;
      case 4: gb_sineArray[i].setVolume((gbVolMixer_now[i]>>2)); break;
      default: gb_sineArray[i].setVolume((gbVolMixer_now[i]<<2)); break;
    }
    
    gbVolMixer_before[i] = gbVolMixer_now[i];
   }
   if (gbFrecMixer_now[i] != gbFrecMixer_before[i])
   {
    gb_sineArray[i].setFrequency(gbFrecMixer_now[i]);
    gbFrecMixer_before[i] = gbFrecMixer_now[i];
   }
  }
 } 
#endif 




    int pal_speed = 1773447;
    int pal_start_int = 341;
    int pal_vblank_int = pal_speed / 50;
    int pal_vblank_timeout = (313 - 240) * pal_vblank_int / 313;
    int pal_scanline_refresh = pal_vblank_int / 313;
    int width = 256;
    int height = 240;
    int rcb;



//Funciones
void Setup(void);
//void SDL_keys_poll(void);




//Lectura teclado
//void SDL_keys_poll()
//{  
//  //oCIA1->TestTeclado(59,(keymap[PS2_KC_2])); 
//  //row 0
//  //oCIA1->TestTeclado(0,(keymap[KEY_BACKSPACE]));;	
//
//  if (keymap[PS2_KC_F12] == 0)
//  {
//   ESP.restart();
//  }                    
//}

unsigned char gb_color_vga[128];


//**************************************************
void PreparaColorVGA()
{
 //for (unsigned char i=0;i<64;i++)
 //{  
 // //gb_color_vga[i] = (i & vga.RGBAXMask)| vga.SBits;
 // gb_color_vga[i] =  (gb_cache_paleta_id[i] & vga.RGBAXMask)| vga.SBits;  
 //}   

 #ifdef use_lib_vga8colors
  for (unsigned char i=0;i<128;i++){   
   gb_color_vga[i]= (gb_cache_paleta_id[i] & 0x07) | gb_sync_bits;
  }
 #else
  //Hay juegos que pasan de 64 colores, pongo mascara limite 64
  for (unsigned char i=0;i<128;i++){//DAC 6 bits 64 colores
   gb_color_vga[i]= (gb_cache_paleta_id[i] & 0x3F) | gb_sync_bits;
  }
 #endif 
}


#ifdef use_lib_bank_cache
 //*********************************
 void CreateMemoryPRG()
 {
  for (unsigned int i=0;i<6;i++)
  {
   gb_prg_array[i]= (unsigned char *)malloc(16384);
   #ifdef use_lib_log_serial
    Serial.printf("Memoria:%d\n",i);
   #endif 
  }
 }

 //*********************************
 void LoadCacheBankPRG()
 {
  unsigned int auxOffs;
  for (unsigned int i=0;i<6;i++)
  {
   auxOffs= 16 + ((unsigned int)i<<14);
   memcpy(gb_prg_array[i],&gb_list_rom_data[gb_id_cur_rom][auxOffs],16384); 
  }
 }
#endif 


//Setup principal
void setup()
{   
 #ifdef use_lib_log_serial
  Serial.begin(115200);         
  Serial.printf("HEAP BEGIN %d\n", ESP.getFreeHeap()); 
 #endif

 #ifdef use_lib_bank_cache
  CreateMemoryPRG();
 #endif 
 //memory = (unsigned char *)malloc(65536);

 #define RED_PINS_6B 21, 22
 #define GRE_PINS_6B 18, 19
 #define BLU_PINS_6B  4,  5
 const int redPins[] = {RED_PINS_6B};
 const int grePins[] = {GRE_PINS_6B};
 const int bluPins[] = {BLU_PINS_6B};
 //vga.init(vga.MODE320x240.custom(256,240), redPins, grePins, bluPins, HSYNC_PIN, VSYNC_PIN);
 //funcion vga.init(vga.MODE320x240, redPins, grePins, bluPins, HSYNC_PIN, VSYNC_PIN);
 vga_init(pin_config,VgaMode_vga_mode_320x240,false); //Llamada en C   
 
 //vga.setFont(Font6x8);
 //vga.clear(BLACK);
 //vga.fillRect(0,0,256,240,0x00);
 //gb_buffer_vga= vga.backBuffer;
 gb_sync_bits= vga_get_sync_bits();
 gb_buffer_vga = vga_get_framebuffer();
 gb_buffer_vga32=(unsigned int **)gb_buffer_vga; 
 PreparaColorVGA();
 //PreparaColorVGA();
 SDLClear();
 
 #ifdef use_lib_log_serial
  Serial.printf("VGA %d\n", ESP.getFreeHeap()); 
 #endif

 kb_begin();


 gb_keyboardTime = gb_vgaTime= gb_currentTime = gb_time_ini_beep = millis();
 #ifdef use_lib_sound_ricoh2A03
  gb_sdl_time_sound_before= gb_currentTime;
 #endif

 #ifdef use_lib_sound_ricoh2A03
  gb_sdl_time_sound_before = gb_currentTime;
  for (unsigned char i=0;i<3;i++)
  {
   soundGenerator.attach(&gb_sineArray[i]);
   gb_sineArray[i].enable(true);
   gb_sineArray[i].setFrequency(0);
  }
  soundGenerator.play(true);
 #endif

 #ifdef use_lib_log_serial  
  Serial.printf("END SETUP %d\n", ESP.getFreeHeap()); 
 #endif 

 gb_vgaTime = gb_jj_time_ini_fps= gb_keyboardTime= millis();//SDL_GetTicks();
 #ifdef use_lib_measure_time
  gb_cpu_timer_before= gb_cpu_timer_cur= gb_keyboardTime = millis();
 #endif

}



//static unsigned int gb_jj_cur_cycle=0;
//static unsigned int gb_jj_ini_cycle=0;
//static unsigned int gb_jj_cur_fps=0;
//static unsigned int gb_jj_ini_fps=0;
//static unsigned int gb_jj_time_ini_fps=0;




void input_set(int key)
{

    pad1_state[key] = 0x01;

}

void input_clear(int key)
{

    pad1_state[key] = 0x40;

}

void input_reset()
{
 input_clear(0);
 input_clear(1);
 input_clear(2);
 input_clear(3);
 input_clear(4);
 input_clear(5);
 input_clear(6);
 input_clear(7);
}


//*************************************************************************************
static void run(int width, int height, int start_int, int vblank_int, int vblank_timeout, int scanline_refresh)
{
 unsigned int jj_ini_cpu,jj_end_cpu,gb_cur_cpu_ticks; 

    int counter = 0;
    int scanline = 0;
    unsigned int currenttime;
    unsigned int starttime;
    unsigned int jj_endtime;
    int deltatime;
    
    //gb_vgaTime = gb_jj_time_ini_fps= gb_keyboardTime= millis();//SDL_GetTicks();

    //while (running)
    {

        starttime = millis();// backend_getticks();
        jj_ini_cpu = micros();
        jj_ini_medir_cpu= jj_ini_cpu;

        //JJ cpu_execute(start_int, memory);
        cpu_execute(start_int);

        memory[PPUSTATUS] |= 0x80;

        //JJ counter += cpu_execute(12, memory);
        counter += cpu_execute(12);

        if (PPUCTRL_VBLANKNMI){
         //JJ counter += cpu_nmi(counter, memory);
         counter += cpu_nmi(counter);
        }

        //JJ counter += cpu_execute(vblank_timeout, memory);
        counter += cpu_execute(vblank_timeout);
        memory[PPUSTATUS] &= 0x3F;

        ppu_register_v = ppu_register_t;

        jj_end_medir_cpu = micros();
        gb_cur_cpu_ticks= (jj_end_medir_cpu-jj_ini_medir_cpu);
        if (gb_cur_cpu_ticks>gb_max_cpu_ticks)
         gb_max_cpu_ticks= gb_cur_cpu_ticks;
   if (gb_cur_cpu_ticks<gb_min_cpu_ticks)   
    gb_min_cpu_ticks= gb_cur_cpu_ticks;   

   gb_cpu_timer_cur= millis();
   if ((gb_cpu_timer_cur-gb_cpu_timer_before)<1000)
   {   
   }
   else
   {
    gb_cpu_timer_before= gb_cpu_timer_cur;
    //Imprimo CPU
    #ifdef use_lib_measure_time
     Serial.printf("c:%u m:%u mx:%u\n",gb_cur_cpu_ticks,gb_min_cpu_ticks,gb_max_cpu_ticks);   
    #endif
    
    //Reseteo CPU a 1 segundo
    gb_min_cpu_ticks= 1000000;
    gb_max_cpu_ticks= 0;
    gb_cur_cpu_ticks= 0;   
  }      


        //JJ backend_lock();
        
       //if ((gb_jj_cur_fps & 0x03) == 0)
       //gb_process_frame=1;// Fuerzo procesar video
       //if ((gb_process_frame==1) && ((gb_jj_cur_fps & 0x01)==1))
       //if (gb_process_frame==1)
       //if ((gb_jj_cur_fps & 0x0F)==0x0F)
       { 
        
        for (scanline = 0; scanline < height; scanline++)
        {
         if (!PPUSTATUS_SPRITE0HIT)
         {
          ppu_checkspritehit(width, scanline);
         }
         ppu_renderbackground(scanline);
         //JJ counter += cpu_execute(scanline_refresh, memory);
         counter += cpu_execute(scanline_refresh);
         if (mmc3_irq_enable == 1)
         {
          if (scanline == mmc3_irq_counter)
          {
           //JJ cpu_irq(counter, memory);
           cpu_irq(counter);
           mmc3_irq_counter--;
          }
         }
        }//fin for scanline
       
        ppu_rendersprites();
       }
       
       //jj_endtime = SDL_GetTicks()-starttime;
       //printf("Frame ms:%d\n",jj_endtime);
       //fflush(stdout);
       
        //JJ backend_unlock();
        //JJ backend_update(); //Siempre se ejecuta sin esperar VGA tiempo
        
        jj_end_cpu = micros();
        gb_cur_cpu_ticks = jj_end_cpu-jj_ini_cpu;
        if (gb_auto_delay_cpu == 1)
        {
         if (gb_cur_cpu_ticks< 20000)
         {
          delayMicroseconds((20000-gb_cur_cpu_ticks));
         }
        }

        gb_jj_cur_fps++;
        gb_currentTime= millis();// SDL_GetTicks();
//        if ((gb_currentTime-gb_vgaTime) >= 50)
//        {//15 fps son 66 ms
//         gb_vgaTime= gb_currentTime;
//         gb_process_frame= 1;
//         //SDL_Flip(screen);
//         //SDL_Delay(20);
//        }
//        else
//        {
//         gb_process_frame= 0;
//        }
        
        //printf("%d\n",(gb_currentTime-gb_jj_time_ini_fps));
        if ((gb_currentTime-gb_jj_time_ini_fps)>=1000)
        {
         gb_jj_time_ini_fps=gb_currentTime;
         int aux_fps= gb_jj_cur_fps-gb_jj_ini_fps;
         gb_jj_ini_fps = gb_jj_cur_fps;
         #ifdef use_lib_measure_time
          Serial.printf("fps %d\n",aux_fps);
         #endif 
         //fflush(stdout);
        }        

//        currenttime = millis();
        deltatime = 16 - (currenttime - starttime);
//
//        #ifdef use_lib_delay
//         if (deltatime > 0)
//         {
//          //JJ backend_delay(deltatime);         
//          SDL_Delay(deltatime);
//          //printf("delay\n");
//          //fflush(stdout);            
//         }
//        #endif
        

        //JJ backend_event(); //Leemos siempre sin polling de tiempo
        if ((gb_currentTime-gb_keyboardTime) >= 10)
        {
         gb_keyboardTime= gb_currentTime;
         backend_event();
        }
        

    }

}





//Loop main
void loop() 
{ 
  /*
 gb_currentTime = millis();
 if ((gb_currentTime-gb_keyboardTime) >= gb_current_ms_poll_keyboard)
 {  
  gb_keyboardTime = gb_currentTime;
  SDL_keys_poll();
 }
*/


 if (gb_load_new_rom == 1)
 {
  gb_load_new_rom=0;
  running= 1;

    //JJ romfn = argv[1];
   #ifdef use_lib_log_serial 
    Serial.printf("ROM %d\n",gb_id_cur_rom);    
   #endif 
    
    rom_load_from_memory(gb_id_cur_rom, &header, memory, ppu_memory);
    prev_bank_prg = 255; //Reseteamos previo banco
    #ifdef use_lib_bank_cache
     LoadCacheBankPRG();
    #endif 
    
    rcb = (header.flags6 - ((header.flags6 >> 4) << 4));
    MIRRORING = (rcb & 1) ? 1 : 0;
    SRAM = (rcb & 2) ? 1 : 0;

    mapper = &mappers[(header.flags6 >> 4) | (header.flags7 & 0xF0)];

    if (mapper->reset)
        mapper->reset();

    //if (SRAM == 1)
    //    backend_readsavefile("game.sav", memory);

    //JJ backend_init(width, height);
    //JJ cpu_reset(memory);
    cpu_reset();
    input_reset();
    run(width, height, pal_start_int, pal_vblank_int, pal_vblank_timeout, pal_scanline_refresh);
 }
 else
 {
  //#ifdef use_lib_measure_time     
  // jj_ini_medir_cpu = micros();
  //#endif      
  
  run(width, height, pal_start_int, pal_vblank_int, pal_vblank_timeout, pal_scanline_refresh);

 #ifdef use_lib_sound_ricoh2A03
  if (gb_silence_all_channels == 1)
   SilenceAllChannels();
  else
  {
   gb_currentTime = millis();
   if ((gb_currentTime-gb_sdl_time_sound_before) >= gb_current_ms_poll_sound)
   {
    gb_sdl_time_sound_before= gb_currentTime;
    //jj_mixpsg();
    sound_cycleFabgl();
   }
  }
 #endif  

  //#ifdef use_lib_measure_time
  // jj_end_medir_cpu = micros();
  //
  // gb_cur_cpu_ticks= (jj_end_medir_cpu-jj_ini_medir_cpu);
  // if (gb_cur_cpu_ticks>gb_max_cpu_ticks)
  //  gb_max_cpu_ticks= gb_cur_cpu_ticks;
  // if (gb_cur_cpu_ticks<gb_min_cpu_ticks)   
  //  gb_min_cpu_ticks= gb_cur_cpu_ticks;   

  // gb_cpu_timer_cur= millis();
  // if ((gb_cpu_timer_cur-gb_cpu_timer_before)<1000)
  // {   
  // }
  // else
  // {
  //  gb_cpu_timer_before= gb_cpu_timer_cur;
  //  //Imprimo CPU
  //  Serial.printf("c:%u m:%u mx:%u\n",gb_cur_cpu_ticks,gb_min_cpu_ticks,gb_max_cpu_ticks);   
  //  
  //  //Reseteo CPU a 1 segundo
  //  gb_min_cpu_ticks= 1000000;
  //  gb_max_cpu_ticks= 0;
  //  gb_cur_cpu_ticks= 0;   
  //}      
  //#endif 

 }
 


 /*
 //gb_currentTime = millis();
 gb_currentTime = millis();
 if ((gb_currentTime-gb_vgaTime) >= 20)
 {//50 fps
  gb_vgaTime= gb_currentTime;
  jj_video_Update();
  //oVIC->Update(); 
 }
 */


/*
 gb_currentTime = millis();
 if ((gb_currentTime-gb_jj_time_ini_fps)>=1000)
 {
  gb_jj_time_ini_fps=gb_currentTime;
  unsigned int aux_fps= gb_jj_cur_fps-gb_jj_ini_fps;
  gb_jj_ini_fps = gb_jj_cur_fps;  
  Serial.printf ("fps:%d %d m:%d mx:%d\n",aux_fps,gb_stats_time_cur,gb_stats_time_min,gb_stats_time_max);
  gb_stats_time_min= 500000;
  gb_stats_time_max= 0;  
 }
 */

 //oCPU.RunOpcode();
 //if (frame == 100000) 
 //{
//	oVIC->Update();
//	//JJ oHostIO.Yield();
//	frame = 0;
// }
// frame++;
// oCPU.RunOpcode();
}
