#include "gbConfig.h"
#include "nes.h"
#include "osd.h"
#include "dataFlash/gbrom.h"
#include "gbGlobals.h"
#include "PS2Kbd.h"
#include "gb_sdl_font8x8.h"
#include "sdl.h"


unsigned char gb_run_rom=0;
unsigned char gb_id_run_rom=0;
unsigned int gb_timer_run_rom_ini=0;

//#define BLACK   0
//#define BLUE    4
//#define RED     1
//#define MAGENTA 5
//#define GREEN   2
//#define CYAN    6
//#define YELLOW  3
//#define WHITE   15

#ifdef COLOR_3B           //       BGR 
 #define BLACK   0x08      // 0000 1000
 #define BLUE    0x0C      // 0000 1100
 #define RED     0x09      // 0000 1001
 #define MAGENTA 0x0D      // 0000 1101
 #define GREEN   0x0A      // 0000 1010
 #define CYAN    0x0E      // 0000 1110
 #define YELLOW  0x0B      // 0000 1011
 #define WHITE   0x0F      // 0000 1111
#endif


#ifdef use_lib_vga8colors
 #define ID_COLOR_BLACK 0 //Negro
 #define ID_COLOR_WHITE 12 //Blanco
 #define ID_COLOR_MAGENTA 6 //Magenta
#else
 #define ID_COLOR_BLACK 63 //Negro
 #define ID_COLOR_WHITE 61 //Blanco
 #define ID_COLOR_MAGENTA 23 //Magenta
#endif 

//extern int gb_screen_xIni;
//extern int gb_screen_yIni;
//extern unsigned char gb_cache_zxcolor[8];


unsigned char gb_show_osd_main_menu=0;

//extern SDL_Surface * gb_screen;
//extern SDL_Event gb_event;





#define max_gb_osd_screen 1
const char * gb_osd_screen[max_gb_osd_screen]={
 "Pixels Left"//,
 //"Pixels Top",
 //"Color 8",
 //"Mono Blue 8",
 //"Mono Green 8",
 //"Mono Red 8",
 //"Mono Grey 8"
};

#define max_gb_osd_screen_values 5
const char * gb_osd_screen_values[max_gb_osd_screen_values]={
 "0",
 "2",
 "4", 
 "8", 
 "16"
};


#define max_gb_main_menu 6
const char * gb_main_menu[max_gb_main_menu]={ 
 "Load ROM", 
 "Speed",
 "Screen Adjust", 
 "Sound",
 "Reset",
 "Return"
};


#define max_gb_romloadrun_menu 3
const char * gb_romloadrun_menu[max_gb_romloadrun_menu]={
 "Load ROM",
 "Run ROM",
 "Return"
};


#define max_gb_osd_sound_menu 2
const char * gb_osd_sound_menu[max_gb_osd_sound_menu]={
 "Sound Enabled",
 "Sound Volume"
};

#define max_gb_speed_sound_menu 52
const char * gb_speed_sound_menu[max_gb_speed_sound_menu]={
 "0 (FAST)","1","2","3","4","5","6","7",
 "8","9","10","11","12","13","14","15",
 "16","17","18","19","20","21","22","23",
 "24","25","26","27","28","29","30","31",
 "32","33","34","35","36","37","38","39",
 "40","41","42","43","44","45","46","47",
 "48","49","50","AUTO"
};

#define max_osd_sound_vol_values 5
const char * gb_osd_sound_vol_values[max_osd_sound_vol_values]={
 "100%",
 "75%",
 "50%",
 "25%",
 "5%"
};


#define max_gb_value_binary_menu 2
const char * gb_value_binary_menu[max_gb_value_binary_menu]={
 "0 (OFF)",
 "1 (ON)"
};


#define max_gb_speed_videoaudio_options_menu 4
const char * gb_speed_videoaudio_options_menu[max_gb_speed_videoaudio_options_menu]={
 "Audio poll",
 "CPU delay",
 "Skip Frame",
 "Keyboard poll" 
};


//#define max_gb_speed_menu 5
//const char * gb_speed_menu[max_gb_speed_menu]={
// "Normal",
// "2x",
// "4x",
// "8x",
// "16x"
//};


#define max_gb_osd_mouse_menu 3
const char * gb_osd_mouse_menu[max_gb_osd_mouse_menu]={
 "Mouse Detect",
 "Mouse Buttons",
 "Mouse Enabled"
};

#define max_gb_osd_mouse_buttons_menu 2
const char * gb_osd_mouse_buttons_menu[max_gb_osd_mouse_buttons_menu]={
 "right handed",
 "left handed"
};

#define max_gb_reset_menu 2
const char * gb_reset_menu[max_gb_reset_menu]={
 "Soft",
 "Hard"
};


//#ifdef use_lib_400x300
// #define gb_pos_x_menu 120
// #define gb_pos_y_menu 50 
//#else
// #define gb_pos_x_menu 90
// #define gb_pos_y_menu 50
//#endif

#define gb_pos_x_menu 90
#define gb_pos_y_menu 50

#define gb_osd_max_rows 10


//*************************************************************************************
void SDLprintCharOSD(char car,int x,int y,unsigned char color,unsigned char backcolor)
{ 
// unsigned char aux = gb_sdl_font_6x8[(car-64)];
 int auxId = car << 3; //*8
 unsigned char aux;
 unsigned char auxBit,auxColor;
 for (unsigned char j=0;j<8;j++)
 {
  aux = gb_sdl_font_8x8[auxId + j];
  for (int i=0;i<8;i++)
  {
   auxColor= ((aux>>i) & 0x01);
   //SDLputpixel(surface,x+(6-i),y+j,(auxColor==1)?color:backcolor);
   jj_fast_drawpixel(x+(6-i),y+j,(auxColor==1)?color:backcolor);
  }
 }
}

void SDLprintText(const char *cad,int x, int y, unsigned char color,unsigned char backcolor)
{
//SDL_Surface *surface,
// gb_sdl_font_6x8
 int auxLen= strlen(cad);
 if (auxLen>50)
  auxLen=50;
 for (int i=0;i<auxLen;i++)
 {
  SDLprintCharOSD(cad[i],x,y,color,backcolor);
  x+=7;
 }
}


void OSDMenuRowsDisplayScroll(const char **ptrValue,unsigned char currentId,unsigned char aMax)
{//Dibuja varias lineas
 for (int i=0;i<gb_osd_max_rows;i++)
  SDLprintText("                    ",gb_pos_x_menu,gb_pos_y_menu+8+(i<<3),ID_COLOR_BLACK,ID_COLOR_BLACK);
 
 for (int i=0;i<gb_osd_max_rows;i++)
 {
  if (currentId >= aMax)
   break;  
  SDLprintText(ptrValue[currentId],gb_pos_x_menu,gb_pos_y_menu+8+(i<<3),((i==0)?ID_COLOR_WHITE:ID_COLOR_WHITE),((i==0)?ID_COLOR_MAGENTA:ID_COLOR_BLACK));
  currentId++;
 }     

 
 //vga.setTextColor(WHITE,BLACK);
 //for (int i=0;i<gb_osd_max_rows;i++)
 //{
 // vga.setCursor(gb_pos_x_menu, gb_pos_y_menu+8+(i<<3));
 // vga.print("                    ");
 //}
 //
 //for (int i=0;i<gb_osd_max_rows;i++)
 //{
 // if (currentId >= aMax)
 //  break;
 // if (i == 0)
 //  vga.setTextColor(CYAN,BLUE);
 // else
 //  vga.setTextColor(WHITE,BLACK);
 // vga.setCursor(gb_pos_x_menu, gb_pos_y_menu+8+(i<<3));
 // vga.print(ptrValue[currentId]);
 // currentId++;
 //}     
}

//Maximo 256 elementos
unsigned char ShowTinyMenu(const char *cadTitle,const char **ptrValue,unsigned char aMax,short int aSel)
{
 unsigned char aReturn=0;
 unsigned char salir=0;
 SDLClear();
 SDLprintText("Port NES(Jens Nyberg)Ackerman",(gb_pos_x_menu-(32)),(gb_pos_y_menu-16),ID_COLOR_WHITE,ID_COLOR_BLACK);
 //for (int i=0;i<20;i++) 
 for (int i=0;i<14;i++) 
  SDLprintCharOSD(' ',gb_pos_x_menu+(i<<3),gb_pos_y_menu,ID_COLOR_BLACK,ID_COLOR_WHITE);
 SDLprintText(cadTitle,gb_pos_x_menu,gb_pos_y_menu,ID_COLOR_BLACK,ID_COLOR_WHITE);

 aReturn = (aSel!=-1)?aSel:0;
 OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);
 
 while (salir == 0)
 {             
  //case SDLK_UP:
  if (checkAndCleanKey(KEY_CURSOR_LEFT))
  {
   if (aReturn>10) aReturn-=10;
   OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);       
  }
  if (checkAndCleanKey(KEY_CURSOR_RIGHT))
  {
   if (aReturn<(aMax-10)) aReturn+=10;
   OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);       
  }  
  if (checkAndCleanKey(KEY_CURSOR_UP))
  {
   if (aReturn>0) aReturn--;
   OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);
  }
  if (checkAndCleanKey(KEY_CURSOR_DOWN))
  {
   if (aReturn < (aMax-1)) aReturn++;
   OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);
  }
  if (checkAndCleanKey(KEY_ENTER))
  {
   salir= 1;
  }
  //case SDLK_KP_ENTER: case SDLK_RETURN: salir= 1;break;
  if (checkAndCleanKey(KEY_ESC))
  {
   salir=1; aReturn= 255;    
  }
  //case SDLK_ESCAPE: salir=1; aReturn= 255; break;
  //default: break;    
 }
 SDLClear();
 gb_show_osd_main_menu= 0;
 return aReturn;



/*
 unsigned char aReturn=0;
 unsigned char salir=0;
 #ifdef use_lib_400x300
  vga.fillRect(0,0,400,300,BLACK);
  vga.fillRect(0,0,400,300,BLACK);//Repeat Fix visual defect
 #else
  vga.fillRect(0,0,320,200,BLACK);
  vga.fillRect(0,0,320,200,BLACK);//Repeat Fix visual defect    
 #endif
 vga.setTextColor(WHITE,BLACK);
 vga.setCursor((gb_pos_x_menu-(32)), gb_pos_y_menu-16);
 vga.print("Port Cpcem(Tom Walker) by Ackerman");

 vga.setTextColor(BLACK,WHITE); 
 for (int i=0;i<12;i++)
 {  
  vga.setCursor((gb_pos_x_menu+(i*6)), gb_pos_y_menu);
  vga.print(" ");
 }
  
 vga.setCursor(gb_pos_x_menu,gb_pos_y_menu);
 vga.print(cadTitle);  
 
 //for (int i=0;i<aMax;i++)
 //{     
 // //SDLprintText(gb_osd_sdl_surface,ptrValue[i],gb_pos_x_menu,gb_pos_y_menu+8+(i<<3),((i==0)?CYAN:WHITE),((i==0)?BLUE:BLACK),1);  
 // vga.setTextColor(((i==0)?CYAN:WHITE),((i==0)?BLUE:BLACK));
 // vga.setCursor(gb_pos_x_menu,(gb_pos_y_menu+8+(i<<3)));
 // vga.print(ptrValue[i]);
 //}
 
 OSDMenuRowsDisplayScroll(ptrValue,0,aMax);
 
 while (salir == 0)
 {
  //SDL_PollEvent(gb_osd_sdl_event);
  //if(SDL_WaitEvent(gb_osd_sdl_event))
  {
   //if(gb_osd_sdl_event->type == SDL_KEYDOWN)
   {
    //switch(gb_osd_sdl_event->key.keysym.sym)
    {
     //case SDLK_UP:
     if (checkAndCleanKey(KEY_CURSOR_UP))
     {
      //vga.setTextColor(WHITE,BLACK);
      //vga.setCursor(gb_pos_x_menu,gb_pos_y_menu+((aReturn+1)<<3));
      //vga.print(ptrValue[aReturn]);
      ////SDLprintText(gb_osd_sdl_surface,ptrValue[aReturn],gb_pos_x_menu,gb_pos_y_menu+((aReturn+1)<<3),WHITE,BLACK,1);
      if (aReturn>0) aReturn--;
      OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);
      //vga.setTextColor(CYAN,BLUE);
      //vga.setCursor(gb_pos_x_menu,gb_pos_y_menu+((aReturn+1)<<3));
      //vga.print(ptrValue[aReturn]);
      ////SDLprintText(gb_osd_sdl_surface,ptrValue[aReturn],gb_pos_x_menu,gb_pos_y_menu+((aReturn+1)<<3),CYAN,BLUE,1);
      ////break;
     }
     if (checkAndCleanKey(KEY_CURSOR_DOWN))
     {
     //case SDLK_DOWN: 
      //SDLprintText(gb_osd_sdl_surface,ptrValue[aReturn],gb_pos_x_menu,gb_pos_y_menu+((aReturn+1)<<3),WHITE,BLACK,1);
      //vga.setTextColor(WHITE,BLACK);
      //vga.setCursor(gb_pos_x_menu,gb_pos_y_menu+((aReturn+1)<<3));
      //vga.print(ptrValue[aReturn]);
      if (aReturn < (aMax-1)) aReturn++;
      OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);
      //vga.setTextColor(CYAN,BLUE);
      //vga.setCursor(gb_pos_x_menu,gb_pos_y_menu+((aReturn+1)<<3));
      //vga.print(ptrValue[aReturn]);
      ////SDLprintText(gb_osd_sdl_surface,ptrValue[aReturn],gb_pos_x_menu,gb_pos_y_menu+((aReturn+1)<<3),CYAN,BLUE,1);
      ////break;
     }
     if (checkAndCleanKey(KEY_ENTER))
     {
      salir= 1;
     }
     //case SDLK_KP_ENTER: case SDLK_RETURN: salir= 1;break;
     if (checkAndCleanKey(KEY_ESC))
     {
      salir=1; aReturn= 255;    
     }
     //case SDLK_ESCAPE: salir=1; aReturn= 255; break;
     //default: break;
    }
    //SDL_Flip(gb_osd_sdl_surface);
    //SDL_PollEvent(gb_osd_sdl_event);
   }  
  }  
 } 
 gb_show_osd_main_menu= 0;
 return aReturn;
 */
}

//****************************************************
unsigned char ShowTinyLoadROMMenu()
{
 return 0;
/*  
 unsigned char aSelLoadROM,aSelIdROM,aSelLowHigh;     
 unsigned char aReturn=255;
 aSelLoadROM= ShowTinyMenu("Load ROM",gb_list_romextra_title,max_list_romextra);
 if (aSelLoadROM != 255)
 {
  aSelLowHigh= ShowTinyMenu("LOW OR HIGH ROM",gb_romlowhigh_menu,max_gb_romlowhigh_menu);   
  if (aSelLowHigh == 0)
  {//Low
   lorom= (unsigned char*)gb_list_romextra_data[aSelLoadROM];
   pc=0; //reset
   aReturn= aSelLoadROM;
  }
  else
  {//High
   aSelIdROM= ShowTinyMenu("BANK ROM HIGH",gb_osd_rom_id_values,max_gb_osd_rom_id_values);                   
   if (aSelIdROM!=255)
   {
    hirom[aSelIdROM]= (unsigned char*)gb_list_romextra_data[aSelLoadROM];
    pc=0; //reset
    aReturn= aSelLoadROM;
   }
  }  
 }
 return aReturn;    
 */
}

//***********************************************************
void ShowTinyRunROMMenu(unsigned char idRom)
{
  /*
 unsigned char aSelLoadROM;   
 //0xAC8A(6128)	0xACA4(464)	0x100 BASIC input area for lines (as typed in and not tokenised) or for INPUT
 //aSelRunROM= ShowTinyMenu("Run ROM",gb_list_rom_title,max_list_rom);
 //aSelLoadROM= ShowTinyMenu("Run ROM",gb_list_rom_title,max_list_rom);    
 aSelLoadROM= idRom;
 printf("ShowTinyRunROMMenu:%d\n",idRom);
 fflush(stdout);
 if (aSelLoadROM != 255)
 {
  unsigned off= (model==2)? 0xAC8A:0xACA4;
  memset(&ram[off],0,0x100); //Borramos el buffer texto BASIC
  //unsigned off= 0xB514;
  ram[off++]= 0x7C; //caracter |
  int longitud= strlen(gb_list_romextra_title[aSelLoadROM]);
  for (int i=0;i<longitud;i++)
  {
   ram[off++]= gb_list_romextra_title[aSelLoadROM][i];
  }
 }     
 */
}
//***********************************************************
void ShowTinyROMMenu()
{
 unsigned char aSelNum;     
 aSelNum = ShowTinyMenu("ROM",gb_list_rom_title,max_list_rom,gb_id_cur_rom);

 romfn= (char *)gb_list_rom_title[aSelNum];
 gb_load_new_rom = 1;
 gb_id_cur_rom = aSelNum;
 running= 0;

/*  
 unsigned char aSelNum,aSelLoadROM,aSelRunROM,aSelIdROM,aSelLowHigh;
 aSelNum = ShowTinyMenu("Load OR Run ROM",gb_romloadrun_menu,max_gb_romloadrun_menu);
 switch (aSelNum)
 {
  case 0: ShowTinyLoadROMMenu(); break;
  case 1:
   aSelRunROM= ShowTinyLoadROMMenu();
   if (aSelRunROM != 255)
   {
    //ShowTinyRunROMMenu(aSelRunROM);
    gb_run_rom= 1;
    gb_id_run_rom= aSelRunROM;
    gb_timer_run_rom_ini= millis();    
   }
   break;
 }
 */
}


//Menu resetear
void ShowTinyResetMenu()
{
 unsigned char aSelNum;
 aSelNum= ShowTinyMenu("Reset",gb_reset_menu,max_gb_reset_menu,-1);
 if (aSelNum != 255)
 {  
  if (aSelNum == 1){
   ESP.restart();
  }
  else{
   gb_load_new_rom= 1;
  }
 }
}

//Menu TAPE
/*void ShowTinyTAPEMenu()
{
 unsigned char aSelNum;
 aSelNum = ShowTinyMenu("48K TAPE",gb_list_tapes_48k_title,max_list_tape_48);
 load_tape2Flash(aSelNum);
}

//Play sound tape
void ShowTinySelectTAPEMenu()
{
 unsigned char aSelNum;
 aSelNum = ShowTinyMenu("48K TAPE",gb_list_tapes_48k_title,max_list_tape_48);
 Z80EmuSelectTape(aSelNum);
// gb_play_tape_current = aSelNum;
// gb_play_tape_on_now = 1;
}

//Menu SCREEN
void ShowTinySCRMenu()
{
 unsigned char aSelNum;
 aSelNum = ShowTinyMenu("48K SCREEN",gb_list_scr_48k_title,max_list_scr_48);
 load_scr2Flash(aSelNum);     
}
*/

//Menu velocidad emulador
void ShowTinySpeedMenu()
{  
 unsigned char aSelNum,aSelNumSpeed;
 aSelNum = ShowTinyMenu("SPEED CPU AUDIO",gb_speed_videoaudio_options_menu,max_gb_speed_videoaudio_options_menu,-1);
 if (aSelNum == 255)
  return;
 switch (aSelNum)
 {
  case 0: aSelNumSpeed= ShowTinyMenu("AUDIO Poll ms",gb_speed_sound_menu,max_gb_speed_sound_menu,-1);
   if (aSelNumSpeed == 255)
    return;
   gb_current_ms_poll_sound= aSelNumSpeed;
   break;
  case 1: aSelNumSpeed= ShowTinyMenu("CPU DELAY ms",gb_speed_sound_menu,max_gb_speed_sound_menu,-1);
   if (aSelNumSpeed == 255)
    return;
   if (aSelNumSpeed == 51){
    gb_auto_delay_cpu= 1;  
    gb_current_delay_emulate_ms = 1; //Revisar condicion execute main cpu
   }
   else{
    gb_auto_delay_cpu= 0;
    gb_current_delay_emulate_ms = aSelNumSpeed;
   }
   break;
  case 2: 
   aSelNumSpeed = ShowTinyMenu("Skip Frame",gb_value_binary_menu,max_gb_value_binary_menu,-1);
   switch (aSelNumSpeed)
   {
    case 0: //gb_current_frame_crt_skip = 0; 
     break;
    case 1: //gb_current_frame_crt_skip = 1; 
     break;
    default: break;
   }
   break;
  case 3: aSelNumSpeed= ShowTinyMenu("Keyboard Poll ms",gb_speed_sound_menu,max_gb_speed_sound_menu,-1);
   if (aSelNumSpeed == 255)
    return;
   gb_current_ms_poll_keyboard= aSelNumSpeed;
   break;
  default: break;
 } 
}

//Activar desactivar sonido
void ShowTinySoundMenu()
{  
 unsigned char aSelNum,aSelSoundON,aSelVol;
 aSelNum = ShowTinyMenu("SOUND MENU",gb_osd_sound_menu,max_gb_osd_sound_menu,-1); 
 switch (aSelNum)
 {
  case 0:
   aSelSoundON = ShowTinyMenu("SOUND ENABLED",gb_value_binary_menu,max_gb_value_binary_menu,-1); 
   #ifdef use_lib_sound_ricoh2A03
    switch (aSelSoundON)
    {
     case 0: gb_mute_sound=1; break;
     case 1: gb_mute_sound=0; break;
     default: break;
    }
   #endif
   break;
  case 1:
   aSelVol = ShowTinyMenu("SOUND VOLUME",gb_osd_sound_vol_values,max_osd_sound_vol_values,-1);
   #ifdef use_lib_sound_ricoh2A03
    gbShiftLeftVolumen= aSelVol;
   #endif 
   break;
  default: break;
 }
}




//Ajustar pantalla
void ShowTinyScreenAdjustMenu()
{
 unsigned char aSelNum, auxCol; 
 aSelNum= ShowTinyMenu("Screen Adjust",gb_osd_screen,max_gb_osd_screen,-1);
 if (auxCol == 255)
  return;
 auxCol= ShowTinyMenu("Pixels",gb_osd_screen_values,max_gb_osd_screen_values,-1);
 if (auxCol != 255)
 {
  auxCol = auxCol<<1; //x2
  gb_screen_xOffset = auxCol; 
 }  
}



//*******************************************
void SDLActivarOSDMainMenu()
{     
 gb_show_osd_main_menu= 1;   
}

//Very small tiny osd
void do_tinyOSD() 
{
 unsigned char aSelNum;
 if (checkAndCleanKey(KEY_F1))
 {
  gb_show_osd_main_menu= 1;
  #ifdef use_lib_sound_ricoh2A03  
   gb_silence_all_channels = 1;
  #endif    
  return;
 }

 if (gb_show_osd_main_menu == 1)
 {
  aSelNum = ShowTinyMenu("MAIN MENU",gb_main_menu,max_gb_main_menu,-1);
  switch (aSelNum)
  {   
   case 0: ShowTinyROMMenu(); break;
   case 1: ShowTinySpeedMenu(); break;
   case 2: ShowTinyScreenAdjustMenu(); break;   
   case 3: ShowTinySoundMenu(); break;
   case 4: ShowTinyResetMenu(); break; 
    //resetz80();
    //resetcrtc();
    //SDL_Delay(2);
    //break;
//   case 5: ShowTinySpeedMenu(); break;   
//   case 9: OSD_Test64Colores(); break;
//   case 10: OSD_Test8Colores(); break;*/
   default: break;
  }
 }
 #ifdef use_lib_sound_ricoh2A03  
  //gb_silence_all_channels = 0; 
  gb_silence_all_channels = gb_mute_sound;
 #endif 
}

