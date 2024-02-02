#ifndef _GB_CONFIG_H
 #define _GB_CONFIG_H
 
 //Video mode selection (select one of the 3 modes)
 //320x240 640x480 31.4 Khz 60 Hz freq:25175000 pixel_clock:12587500
 #define use_lib_vga320x240x60hz_bitluni
 //QVGA 320x240 60Hz freq:12600000 funciona
 //#define use_lib_vga320x240x60hz_fabgl
 //320x240 640x480 31.4 Khz 60 Hz freq:25175000 pixel_clock:12587500 FIX PLL LOCK ESP32
 //#define use_lib_vga320x240x60hz_bitluni_apll_fix

 
 
 //Not use double numbers (IEEE not fpu) calculate vga frequency
 //#define use_lib_fix_double_precision

 //Debug i2s bitluni vga frequency
 //#define use_lib_debug_i2s



 #ifdef use_lib_vga320x240x60hz_bitluni
  #define use_lib_vga320x240
 #else
  #ifdef use_lib_vga320x240x60hz_fabgl
   #define use_lib_vga320x240
   #ifndef use_lib_fix_double_precision
    #define use_lib_fix_double_precision
   #endif
  #else
   #ifdef use_lib_vga320x240x60hz_bitluni_apll_fix
    #define use_lib_vga320x240
    #define use_lib_apll_fix
   #endif 
  #endif 
 #endif




 //Corrige teclados que no se inicializan(solucion dcrespo3D)
 //#define FIX_PERIBOARD_NOT_INITING

 //Activar si queremos sonido (Activate if we want sound)
 //#define use_lib_sound_ricoh2A03
 
 #ifdef use_lib_sound_ricoh2A03
  #define use_lib_sound_ay8912
 #endif
 

 #define use_lib_tinybitluni_fast

 //Medir tiempo de frame (Measuring frame time)
 //#define use_lib_measure_time

 //cache banks (consumes ram)
 //#define use_lib_bank_cache
 
 //Modo DAC3 bits 8 colores (DAC3-bit 8-colour mode)
 //#define use_lib_vga8colors

 //#define use_lib_colour_8bit
 #define use_lib_fast_sdl
 #define use_lib_resizeram_ppu
 #define use_lib_rom_memory


 //Optimizaciones CPU
 //#define use_lib_cpu_ADC_IM
 //#define use_lib_cpu_ADC_ZP

 

 //Activar Logs (Activate Logs)
 #define use_lib_log_serial

 

 #define usb_lib_wait_emu 
 #define usb_lib_fast_GetCyclesForOpcode
 #define usb_lib_fast_MemoryReadByte
 #define usb_lib_fast_MemoryWriteByte
 

 #ifdef use_lib_measure_time
  #ifndef use_lib_log_serial
   #define use_lib_log_serial 
  #endif 
 #endif



 //Read keyborad poll x millis
 #define gb_ms_keyboard 10
 //Read sound poll x millis
 #define gb_ms_sound 1
 //Skip x frames
 #define gb_frame_crt_skip 0
 //milisegundos espera en cada frame 
 #define use_lib_delay_tick_cpu_auto 1 
 //Delay millis iteration emulate
 #define gb_delay_emulate_ms 0



 //#ifdef define use_lib_200x150
 // #define gb_add_offset_x 0
 // #define gb_add_offset_y 0
 //#else
 // #define gb_add_offset_x 60 
 // #define gb_add_offset_y 40
 //#endif
#endif
