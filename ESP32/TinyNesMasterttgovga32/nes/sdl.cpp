#include "gbConfig.h"
#include "gbGlobals.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include <SDL/SDL.h>
#include "nes.h"
#include "osd.h"
#include "PS2KeyCode.h"
#include "keys.h"
#include "sdl.h"
#include "dataFlash/gbrom.h"

//JJ SDL_Surface *screen;
//JJ SDL_Event event;

//JJ SDL_Color gb_colors[256]; //paleta sdl 256 colores

unsigned char gb_cache_paleta_id[64]={
 0x2A,0x20,0x20,0x21,0x12,0x03,0x02,0x02,0x01,0x04,0x04,0x04,0x14,0x00,0x00,0x00,
 0x3F,0x34,0x34,0x32,0x23,0x13,0x03,0x03,0x07,0x08,0x08,0x18,0x38,0x00,0x00,0x00,
 0x3F,0x3C,0x39,0x3B,0x37,0x27,0x0B,0x0B,0x0B,0x0E,0x0C,0x2C,0x3C,0x15,0x00,0x00,
 0x3F,0x3E,0x3E,0x3B,0x3B,0x2B,0x2F,0x2F,0x2F,0x2F,0x2E,0x3E,0x3E,0x3F,0x00,0x00
};


//static struct
//{
// char r;
// char g;
// char b;
//} palette[64] = {
//    {0x80, 0x80, 0x80}, {0x00, 0x3D, 0xA6}, {0x00, 0x12, 0xB0}, {0x44, 0x00, 0x96},
//    {0xA1, 0x00, 0x5E}, {0xC7, 0x00, 0x28}, {0xBA, 0x06, 0x00}, {0x8C, 0x17, 0x00},
//    {0x5C, 0x2F, 0x00}, {0x10, 0x45, 0x00}, {0x05, 0x4A, 0x00}, {0x00, 0x47, 0x2E},
//    {0x00, 0x41, 0x66}, {0x00, 0x00, 0x00}, {0x05, 0x05, 0x05}, {0x05, 0x05, 0x05},
//    {0xC7, 0xC7, 0xC7}, {0x00, 0x77, 0xFF}, {0x21, 0x55, 0xFF}, {0x82, 0x37, 0xFA},
//    {0xEB, 0x2F, 0xB5}, {0xFF, 0x29, 0x50}, {0xFF, 0x22, 0x00}, {0xD6, 0x32, 0x00},
//    {0xC4, 0x62, 0x00}, {0x35, 0x80, 0x00}, {0x05, 0x8F, 0x00}, {0x00, 0x8A, 0x55},
//    {0x00, 0x99, 0xCC}, {0x21, 0x21, 0x21}, {0x09, 0x09, 0x09}, {0x09, 0x09, 0x09},
//    {0xFF, 0xFF, 0xFF}, {0x0F, 0xD7, 0xFF}, {0x69, 0xA2, 0xFF}, {0xD4, 0x80, 0xFF},
//    {0xFF, 0x45, 0xF3}, {0xFF, 0x61, 0x8B}, {0xFF, 0x88, 0x33}, {0xFF, 0x9C, 0x12},
//    {0xFA, 0xBC, 0x20}, {0x9F, 0xE3, 0x0E}, {0x2B, 0xF0, 0x35}, {0x0C, 0xF0, 0xA4},
//    {0x05, 0xFB, 0xFF}, {0x5E, 0x5E, 0x5E}, {0x0D, 0x0D, 0x0D}, {0x0D, 0x0D, 0x0D},
//    {0xFF, 0xFF, 0xFF}, {0xA6, 0xFC, 0xFF}, {0xB3, 0xEC, 0xFF}, {0xDA, 0xAB, 0xEB},
//    {0xFF, 0xA8, 0xF9}, {0xFF, 0xAB, 0xB3}, {0xFF, 0xD2, 0xB0}, {0xFF, 0xEF, 0xA6},
//    {0xFF, 0xF7, 0x9C}, {0xD7, 0xE8, 0x95}, {0xA6, 0xED, 0xAF}, {0xA2, 0xF2, 0xDA},
//    {0x99, 0xFF, 0xFC}, {0xDD, 0xDD, 0xDD}, {0x11, 0x11, 0x11}, {0x11, 0x11, 0x11}
//};


//static unsigned int prev_offset=0;
void backend_read_from_memory(unsigned int offset, unsigned int count, void *buffer)
{
 //if (prev_offset!= offset)
 //{
 // prev_offset = offset;
 // printf("backend_read SRC:%08X DST:%08X bytes:%d\n",offset,buffer,count);
 // fflush(stdout);      
  memcpy(buffer,&gb_list_rom_data[gb_id_cur_rom][offset],count);   
 //} 
}

//********************************************************************
void SDLClear()
{ 
 for (int y=0; y<240; y++){
  for (int x=0; x<320; x++){
   jj_fast_drawpixel(x,y,63);
  }
 }    
}

//********************************************************************
#ifdef use_lib_fast_sdl
 void jj_fast_drawpixel(int x, int y, unsigned char nescolor)
 {
  /*JJ
  if ((x >= screen->w) || (x < 0))
   return;
  if ((y >= screen->h) || (y < 0))
   return;
  if (!nescolor)
   return;

  Uint8 *bufp = (Uint8 *)screen->pixels + y * screen->w + x;
  *bufp = gb_cache_paleta_id[nescolor];
  */
  //vga.dot(x,y,gb_cache_paleta_id[nescolor]);
  
  //vga.dot(x,y,nescolor); //Funciona
  //if ((y<240)&&(x<256)&&(nescolor<64)){ //Funciona
  // vga.dotFast(x,y,gb_cache_paleta_id[nescolor]);  //Funciona 
  //}//Funciona

  //if ((y<240)&&(x<256)&&(nescolor<64)){  
  if ((y<240)&&(x<320)&&(nescolor<64)){
   gb_buffer_vga[y][x^2] = gb_color_vga[nescolor];   
  }
 }
#endif

//*******************************
void ConviertePaletaNESaSDL()
{
 int i,j;
 unsigned char auxId,aRed,aGreen,aBlue;
/* for (i=0;i<64;i++)
 {
  gb_colors[i].r= palette[i].r;
  gb_colors[i].g= palette[i].g;
  gb_colors[i].b= palette[i].b;
 }

 printf("Paleta Indices\n");
 for (i=0;i<64;i++)
 {//BBGGRR
  aRed = ((palette[i].r>>6)&0x03);
  aGreen = ((palette[i].g>>6)&0x03);
  aBlue = ((palette[i].b>>6)&0x03);
  auxId= (aBlue<<4)|(aGreen<<2)|aRed;
  //00111111
  //11111100
  //11110000
  printf("0x%02X,",auxId);
 }
 fflush(stdout);
 */
 
// for (i=0;i<64;i++)
// {
//  gb_colors[i].r= ((palette[i].r>>6)&0x03)*85;
//  gb_colors[i].g= ((palette[i].g>>6)&0x03)*85;
//  gb_colors[i].b= ((palette[i].b>>6)&0x03)*85;
// }
// 
// for (i=64;i<256;i++) 
//  gb_colors[i].r= gb_colors[i].g= gb_colors[i].b= 255;


 //Paleta real TTGO VGA32 64 colores
 /*JJ
 for (int j=0;j<4;j++)
 {
  for(int i=0;i<64;i++)
  {
   gb_colors[i+(j*64)].b=((i>>4)&0x03)*85;
   gb_colors[i+(j*64)].g=((i>>2)&0x03)*85;
   gb_colors[i+(j*64)].r=(i & 0x03)*85;
  }
 }
 */
 
 
 /*for (int i=0;i<256;i++)
 {//Adapto a 2 bits RRGGBB de color
  gb_colors[i].r= (((gb_colors[i].r>>6)&0x03)<<6);
  gb_colors[i].g= (((gb_colors[i].g>>6)&0x03)<<6);
  gb_colors[i].b= (((gb_colors[i].b>>6)&0x03)<<6);
 }*/
 printf("Tabla color ttgovga\n");
 fflush(stdout);
}

/*
unsigned int backend_read(char *path, unsigned int offset, unsigned int count, void *buffer)
{

    FILE *fp = fopen(path, "rb");

    if (!fp)
        return 0;

    fseek(fp, offset, SEEK_SET);

    count = fread(buffer, count, 1, fp);

    fclose(fp);

    return count;

}

unsigned int backend_write(char *path, unsigned int offset, unsigned int count, void *buffer)
{

    FILE *fp = fopen(path, "rb");

    if (!fp)
        return 0;

    fseek(fp, offset, SEEK_SET);

    count = fwrite(buffer, count, 1, fp);

    fclose(fp);

    return count;

}
*/

void backend_drawpixel(int x, int y, int nescolor)
{
    /*JJ
 if ((x >= screen->w) || (x < 0))
  return;
 if ((y >= screen->h) || (y < 0))
  return;
 if (!nescolor)
  return;

 #ifdef use_lib_colour_original32bits
  Uint32 *bufp = (Uint32 *)screen->pixels + y * screen->w + x;
  *bufp = SDL_MapRGB(screen->format, palette[nescolor].r, palette[nescolor].g, palette[nescolor].b);
 #else
  //Uint8 *bufp = (Uint8 *)screen->pixels + y * screen->w + x;
  ////*bufp = SDL_MapRGB(screen->format, palette[nescolor].r, palette[nescolor].g, palette[nescolor].b); 
  ////*bufp = nescolor;
  //*bufp = gb_cache_paleta_id[nescolor];
 #endif
 */
}


void backend_update()
{
/*JJ
    SDL_Flip(screen);
    */

}

void backend_lock()
{
 /*JJ
 if (SDL_MUSTLOCK(screen))
 {
  if (SDL_LockSurface(screen) < 0)
   return;
 }
 */
}

void backend_unlock()
{
 /*JJ
 if (SDL_MUSTLOCK(screen))
  SDL_UnlockSurface(screen);
 */
}

void backend_init(int w, int h)
{
 /*JJ
 if (SDL_Init(SDL_INIT_VIDEO) < 0)
  exit(1);

 atexit(SDL_Quit);

 #ifdef use_lib_colour_original32bits
  screen = SDL_SetVideoMode(w, h, 32, SDL_SWSURFACE | SDL_DOUBLEBUF);
 #else
  //screen = SDL_SetVideoMode(w, h, 8, SDL_SWSURFACE | SDL_DOUBLEBUF);
  screen = SDL_SetVideoMode(w, h, 8, SDL_SWSURFACE | SDL_DOUBLEBUF);
  ConviertePaletaNESaSDL();
  SDL_SetPalette(screen,SDL_LOGPAL|SDL_PHYSPAL, gb_colors, 0, 256);
 #endif
 SDLAssignSDLSurfaceOSD(screen,&event);//Asignamos surface
 SDL_WM_SetCaption ("SDL Port ESP32 NES", NULL);
 //screen = SDL_SetVideoMode(w, h,8, SDL_SWSURFACE | SDL_DOUBLEBUF);

 if (screen == NULL)
  exit(1);
 */
}


void SDL_keys_poll()
{//BOTON A
 if (keymap[PS2_KC_X] == 0) input_set(0);
 else input_clear(0);
 //BOTON B
 if (keymap[PS2_KC_Z] == 0) input_set(1);
 else input_clear(1);
 //BOTON SELECT
 if (keymap[PS2_KC_C] == 0) input_set(2);
 else input_clear(2);
 //BOTON START
 if (keymap[PS2_KC_V] == 0) input_set(3);
 else input_clear(3);  
 //BOTON ARRIBA
 if (keymap[KEY_CURSOR_UP] == 0) input_set(4);
 else input_clear(4);
 //BOTON ABAJO
 if (keymap[KEY_CURSOR_DOWN] == 0) input_set(5);
 else input_clear(5);
 //BOTON IZQUIERDA
 if (keymap[KEY_CURSOR_LEFT] == 0) input_set(6);
 else input_clear(6); 
 //BOTON DERECHA
 if (keymap[KEY_CURSOR_RIGHT] == 0) input_set(7);
 else input_clear(7); 

}


void backend_event()
{
 SDL_keys_poll();
 do_tinyOSD();

/*JJ
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_KEYDOWN)
        {

            switch (event.key.keysym.sym)
            {
                   
            case SDLK_F10:                   
                 gb_salir = 1;
                 break;
            case SDLK_F1:
                 gb_show_osd_main_menu=1;
                 printf("Tecla F1 Menu OSD\n");
                 fflush(stdout);
                 do_tinyOSD();
                 break;

            case SDLK_x:
                input_set(0);

                break;

            case SDLK_z:
                input_set(1);

                break;

            case SDLK_LSHIFT:
                input_set(2);

                break;

            case SDLK_LCTRL:
                input_set(3);

                break;

            case SDLK_UP:
                input_set(4);

                break;

            case SDLK_DOWN:
                input_set(5);

                break;

            case SDLK_LEFT:
                input_set(6);

                break;

            case SDLK_RIGHT:
                input_set(7);

                break;

            case SDLK_ESCAPE:
                gb_salir = 1;
                halt();

                break;

            default:
                break;

            }

        }

        if (event.type == SDL_KEYUP)
        {

            switch (event.key.keysym.sym)
            {

            case SDLK_x:
                input_clear(0);

                break;

            case SDLK_z:
                input_clear(1);

                break;

            case SDLK_LSHIFT:
                input_clear(2);

                break;

            case SDLK_LCTRL:
                input_clear(3);

                break;

            case SDLK_UP:
                input_clear(4);

                break;

            case SDLK_DOWN:
                input_clear(5);

                break;

            case SDLK_LEFT:
                input_clear(6);

                break;

            case SDLK_RIGHT:
                input_clear(7);

                break;

            default:
                break;

            }

        }

    }
*/
}

unsigned int backend_getticks()
{

    //JJ return SDL_GetTicks();
    return 0;

}

void backend_delay(unsigned int ms)
{
 //JJ SDL_Delay(ms);
}

void backend_readsavefile(char *name, unsigned char *memory)
{

//JJ    backend_read(name, 0, 8192, memory + 0x6000);

}

void backend_writesavefile(char *name, unsigned char *memory)
{

//JJ    backend_write(name, 0, 8192, memory + 0x6000);

}

