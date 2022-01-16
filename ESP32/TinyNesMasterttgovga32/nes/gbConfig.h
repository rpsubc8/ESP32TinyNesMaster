#ifndef _GB_CONFIG_H
 #define _GB_CONFIG_H
 
 //Corrige teclados que no se inicializan(solucion dcrespo3D)
 //#define FIX_PERIBOARD_NOT_INITING

 //Activar si queremos sonido
 //#define use_lib_sound_ricoh2A03
 
 #ifdef use_lib_sound_ricoh2A03
  #define use_lib_sound_ay8912
 #endif
 

 #define use_lib_tinybitluni_fast

 //Medir tiempo de frame
 #define use_lib_measure_time

 //#ifdef use_lib_bank_cache
 
 //Modo DAC3 bits 8 colores
 //#define use_lib_vga8colors

 //#define use_lib_colour_8bit
 #define use_lib_fast_sdl
 #define use_lib_resizeram_ppu
 #define use_lib_rom_memory


 //Optimizaciones CPU
 //#define use_lib_cpu_ADC_IM
 //#define use_lib_cpu_ADC_ZP

 //Video mode
 //#define use_lib_400x300 

 //Activar Logs
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



 //Leer teclado cada x milisegundos
 //#define gb_ms_keyboard 10
 


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
