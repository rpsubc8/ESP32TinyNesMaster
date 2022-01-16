#include "gbConfig.h"
#include "gbGlobals.h"
#include "backend.h"
#include "string.h"
#include "ppu.h"



/*unsigned char ppu_memory[16384];
unsigned int ppu_addr = 0x2000;
unsigned int ppu_lastdata = 0;
unsigned int ppu_register_t = 0;
unsigned int ppu_register_v = 0;
unsigned int ppu_register_x = 0;
unsigned int ppu_register_w = 0;
unsigned char ppu_background_cache[256 + 8][256 + 8];
unsigned int ppu_sprite_addr = 0x00;
unsigned char ppu_sprite_memory[256];
unsigned char ppu_sprite_cache[256 + 8][256 + 8];*/


inline void jj_inline_fast_drawpixel(unsigned char x, unsigned char y, unsigned char nescolor)
{
 //if (y<240){
  gb_buffer_vga[y][(x+gb_screen_xOffset)^2] = gb_color_vga[nescolor];
 //}
}


#ifdef use_lib_resizeram_ppu
 unsigned char Get_ppu_bg_cache(int i,int scanline)
 {
  //unsigned int auxOffset = (i>>3)+(i%8);
  //if (ppu_background_cache[scanline][auxOffset])
  // return 1;
  //else
  // return 0; 
  return 1;
 }
 
 unsigned char Get_ppu_sprite_cache(int i,int scanline)
 {
  //unsigned int auxOffset = (i>>3)+(i%8);
  //if (ppu_sprite_cache[scanline][auxOffset])
  // return 1;
  //else
  // return 0;           
  return 1;
 }
 
 void Set_ppu_bg_cache(int i, int scanline, int valor)
 {
 }
#endif 

//JJ static unsigned char ppu_memread(unsigned int address)
//unsigned char ppu_memread(unsigned int address)
unsigned char ppu_memread(unsigned short int address)
{
 if (address == PPUSTATUS)
 {
  unsigned int old = memory[PPUSTATUS];
  memory[PPUSTATUS] &= 0x7F;
  ppu_register_w = 0;
  return (old & 0xE0) | (ppu_lastdata & 0x1F);
 }

 if (address == PPUDATA)
 {
  unsigned int old = ppu_lastdata;
  ppu_lastdata = ppu_addr;
  ppu_addr += (PPUCTRL_INCREMENT) ? 32 : 1;
  return ppu_memory[old];
 }

 return memory[address];
}

//JJ static unsigned char ppu_memwrite(unsigned int address, unsigned char data)
//unsigned char ppu_memwrite(unsigned int address, unsigned char data)
unsigned char ppu_memwrite(unsigned short int address, unsigned char data)
{
 ppu_lastdata = data;
 if (address == PPUCTRL)
 {
  ppu_register_t &= 0xF3FF;
  ppu_register_t |= (data & 0x03) << 10;
  return data;
 }

 if (address == PPUOAMADDR)
 {
  ppu_sprite_addr = data;
  return data;
 }

 if (address == PPUOAMDATA)
 {
  ppu_sprite_memory[ppu_sprite_addr] = data;
  ppu_sprite_addr++;
  return data;
 }

 if (address == PPUSCROLL)
 {
  if (ppu_register_w)
  {
   ppu_register_t &= 0x8C1F;
   ppu_register_t |= (data & 0x07) << 12;
   ppu_register_t |= (data & 0xF8) << 2;
   ppu_register_w = 0;
  }
  else
  {
   ppu_register_t &= 0xFFE0;
   ppu_register_t |= (data & 0xF8) >> 3;
   ppu_register_x = data & 0x07;
   ppu_register_w = 1;
  }
  return data;
 }

 if (address == PPUADDR)
 {
  if (ppu_register_w)
        {

            ppu_addr |= data;
            ppu_register_t &= 0xFF00;
            ppu_register_t |= data;
            ppu_register_v = ppu_register_t;
            ppu_register_w = 0;

        }

        else
        {

            ppu_addr = (data << 8);
            ppu_register_t &= 0x80FF;
            ppu_register_t |= (data & 0x3F) << 8;
            ppu_register_w = 1;

        }

        return data;

    }

    if (address == PPUDATA)
    {

        if (PPUSTATUS_VRAMWRITEFLAG)
            return data;

        ppu_memory[ppu_addr] = data;

        if ((ppu_addr > 0x1999) && (ppu_addr < 0x3000))
        {

            if (OS_MIRROR == 1)
            {

                ppu_memory[ppu_addr + 0x400] = data;
                ppu_memory[ppu_addr + 0x800] = data;
                ppu_memory[ppu_addr + 0x1200] = data;

            }

            else
            {

                if (MIRRORING == 0)
                    ppu_memory[ppu_addr + 0x400] = data;
                else
                    ppu_memory[ppu_addr + 0x800] = data;

            }

        }

        if (ppu_addr == 0x3f10)
            ppu_memory[0x3f00] = data;

        ppu_addr += (PPUCTRL_INCREMENT) ? 32 : 1;

        return data;

    }

    if (address == 0x4014)
    {

        unsigned int i;

        for (i = 0; i < 256; i++)
            ppu_sprite_memory[i] = memory[0x100 * data + i];

        return data;

    }

    return data;

}


//JJ static void ppu_checkspritehit(int width, int scanline)
void ppu_checkspritehit(int width, int scanline)
{
 int i;
 unsigned int auxOffset;
 for (i = 0; i < width; i++)
 {
  #ifdef use_lib_resizeram_ppu   
   //if (Get_ppu_bg_cache(i,(scanline-1)) && Get_ppu_sprite_cache(i,(scanline-1)))
   //{
   // memory[PPUSTATUS] |= 0x40;
   // return;
   //}  
  #else     
   if (ppu_background_cache[i][scanline - 1] && ppu_sprite_cache[i][scanline - 1])
   {
    memory[PPUSTATUS] |= 0x40;
    return;
   }
  #endif 
 }
}

//JJ static void ppu_renderbackground(int scanline)
void ppu_renderbackground(int scanline)
{

    int tile_count;
    int i;
    int nt_addr;
    int at_addr;
    int x_scroll;
    int y_scroll;
    int attribs;
    unsigned char tile[8];

    ppu_register_v &= 0xfbe0;
    ppu_register_v |= (ppu_register_t & 0x041f);
    x_scroll = (ppu_register_v & 0x1f);
    y_scroll = (ppu_register_v & 0x03e0) >> 5;
    nt_addr = 0x2000 + (ppu_register_v & 0x0fff);
    at_addr = 0x2000 + (ppu_register_v & 0x0c00) + 0x03c0 + ((y_scroll & 0xfffc) << 1) + (x_scroll >> 2);

    if ((y_scroll & 0x0002) == 0)
    {

        if ((x_scroll & 0x0002) == 0)
            attribs = (ppu_memory[at_addr] & 0x03) << 2;
        else
            attribs = (ppu_memory[at_addr] & 0x0C);

    }

    else
    {

        if ((x_scroll & 0x0002) == 0)
            attribs = (ppu_memory[at_addr] & 0x30) >> 2;
        else
            attribs = (ppu_memory[at_addr] & 0xC0) >> 4;

    }

    for (tile_count = 0; tile_count < 33; tile_count++)
    {

        int ttc = tile_count << 3;
        int pt_addr = (ppu_memory[nt_addr] << 4) + ((ppu_register_v & 0x7000) >> 12);
        char a1, a2;

        if (PPUCTRL_BACKGROUNDHI)
            pt_addr += 0x1000;

        a1 = ppu_memory[pt_addr];
        a2 = ppu_memory[pt_addr + 8];

        for (i = 0; i < 8; i++)
        {

            tile[i] = 0;

            if (a1 & (0x80 >> i))
                tile[i] += 1;

            if (a2 & (0x80 >> i))
                tile[i] += 2;

            if (tile[i])
                tile[i] += attribs;

        }

        if ((tile_count == 0) && (ppu_register_x != 0))
        {
         for (i = 0; i < 8 - ppu_register_x; i++)
         {
          #ifdef use_lib_resizeram_ppu
           //ppu_background_cache[ttc + i][scanline] = tile[ppu_register_x + i];
           //Set_ppu_bg_cache((ttc + i),scanline, tile[ppu_register_x + i]);
          #else
           ppu_background_cache[ttc + i][scanline] = tile[ppu_register_x + i];
          #endif          
          if (PPUMASK_BACKGROUNDSHOW)
          {
           #ifdef use_lib_fast_sdl
            //jj_fast_drawpixel(ttc + i, scanline, ppu_memory[0x3f00 + (tile[ppu_register_x + i])]);
            jj_inline_fast_drawpixel(ttc + i, scanline, ppu_memory[0x3f00 + (tile[ppu_register_x + i])]);
           #else
            backend_drawpixel(ttc + i, scanline, ppu_memory[0x3f00 + (tile[ppu_register_x + i])]);
           #endif
          }
         }
        }

        else if ((tile_count == 32) && (ppu_register_x != 0))
        {

            for (i = 0; i < ppu_register_x; i++)
            {
             #ifdef use_lib_resizeram_ppu
              //Set_ppu_bg_cache((ttc + i - ppu_register_x),scanline,tile[i]);
             #else
              ppu_background_cache[ttc + i - ppu_register_x][scanline] = tile[i];
             #endif 
             if (PPUMASK_BACKGROUNDSHOW)
             {
              #ifdef use_lib_fast_sdl
               //jj_fast_drawpixel(ttc + i - ppu_register_x, scanline, ppu_memory[0x3f00 + (tile[i])]);
               jj_inline_fast_drawpixel(ttc + i - ppu_register_x, scanline, ppu_memory[0x3f00 + (tile[i])]);
              #else
               backend_drawpixel(ttc + i - ppu_register_x, scanline, ppu_memory[0x3f00 + (tile[i])]);
              #endif
             }
            }

        }

        else
        {

            for (i = 0; i < 8; i++)
            {
             #ifdef use_lib_resizeram_ppu
              //Set_ppu_bg_cache((ttc + i - ppu_register_x),scanline,tile[i]);
             #else
              ppu_background_cache[ttc + i - ppu_register_x][scanline] = tile[i];
             #endif

             if (PPUMASK_BACKGROUNDSHOW)
             {
              #ifdef use_lib_fast_sdl
               //jj_fast_drawpixel(ttc + i - ppu_register_x, scanline, ppu_memory[0x3f00 + (tile[i])]);
               jj_inline_fast_drawpixel(ttc + i - ppu_register_x, scanline, ppu_memory[0x3f00 + (tile[i])]);
              #else
               backend_drawpixel(ttc + i - ppu_register_x, scanline, ppu_memory[0x3f00 + (tile[i])]);
              #endif   
             }
            }

        }

        nt_addr++;
        x_scroll++;

        if ((x_scroll & 0x0001) == 0)
        {

            if ((x_scroll & 0x0003) == 0)
            {

                if ((x_scroll & 0x1F) == 0)
                {

                    nt_addr ^= 0x0400;
                    at_addr ^= 0x0400;
                    nt_addr -= 0x0020;
                    at_addr -= 0x0008;
                    x_scroll -= 0x0020;

                }

                at_addr++;

            }

            if ((y_scroll & 0x0002) == 0)
            {

                if ((x_scroll & 0x0002) == 0)
                    attribs = ((ppu_memory[at_addr]) & 0x03) << 2;
                else
                    attribs = ((ppu_memory[at_addr]) & 0x0C);

            }

            else
            {

                if ((x_scroll & 0x0002) == 0)
                    attribs = ((ppu_memory[at_addr]) & 0x30) >> 2;
                else
                    attribs = ((ppu_memory[at_addr]) & 0xC0) >> 4;

            }

        }

    }

    if ((ppu_register_v & 0x7000) == 0x7000)
    {

        ppu_register_v &= 0x8fff;

        if ((ppu_register_v & 0x03e0) == 0x03a0)
        {

            ppu_register_v ^= 0x0800;
            ppu_register_v &= 0xfc1f;

        }

        else
        {

            if ((ppu_register_v & 0x03e0) == 0x03e0)
                ppu_register_v &= 0xfc1f;
            else
                ppu_register_v += 0x0020;

        }

    }

    else
    {

        ppu_register_v += 0x1000;

    }

}

static void ppu_rendersprite(int y, int x, int pattern_number, int attribs, int spr_nr)
{

    int disp_spr_back = attribs & 0x20;
    int flip_spr_hor = attribs & 0x40;
    int flip_spr_ver = attribs & 0x80;
    int i, imax;
    int j, jmax;
    int sprite_start;
    int sprite_pattern_table;
    unsigned char bit1[8][16];
    unsigned char bit2[8][16];
    unsigned char sprite[8][16];

    sprite_pattern_table = (!PPUCTRL_SPRITE16 && PPUCTRL_SPRITEHI) ? 0x1000 : 0x0000;
    sprite_start = sprite_pattern_table + ((pattern_number << 3) << 1);
    imax = 8;
    jmax = (PPUCTRL_SPRITE16) ? 16 : 8;

    if ((!flip_spr_hor) && (!flip_spr_ver))
    {

        for (i = imax - 1; i >= 0; i--)
        {

            for (j = 0; j < jmax; j++)
            {

                bit1[(imax - 1) - i][j] = ((ppu_memory[sprite_start + j] >> i) & 0x01) ? 1 : 0;
                bit2[(imax - 1) - i][j] = ((ppu_memory[sprite_start + 8 + j] >> i) & 0x01) ? 1 : 0;

            }

        }

    }

    else if ((flip_spr_hor) && (!flip_spr_ver))
    {

        for (i = 0; i < imax; i++)
        {

            for (j = 0; j < jmax; j++)
            {

                bit1[i][j] = ((ppu_memory[sprite_start + j] >> i) & 0x01) ? 1 : 0;
                bit2[i][j] = ((ppu_memory[sprite_start + 8 + j] >> i) & 0x01) ? 1 : 0;

            }

        }

    }

    else if ((!flip_spr_hor) && (flip_spr_ver))
    {

        for (i = imax - 1; i >= 0; i--)
        {

            for (j = jmax - 1; j >= 0; j--)
            {

                bit1[(imax - 1) - i][(jmax - 1) - j] = ((ppu_memory[sprite_start + j] >> i) & 0x01) ? 1 : 0;
                bit2[(imax - 1) - i][(jmax - 1) - j] = ((ppu_memory[sprite_start + 8 + j] >> i) & 0x01) ? 1 : 0;

            }

        }

    }

    else if ((flip_spr_hor) && (flip_spr_ver))
    {

        for (i = 0; i < imax; i++)
        {

            for (j = (jmax - 1); j >= 0; j--)
            {

                bit1[i][(jmax - 1) - j] = ((ppu_memory[sprite_start + j] >> i) & 0x01) ? 1 : 0;
                bit2[i][(jmax - 1) - j] = ((ppu_memory[sprite_start + 8 + j] >> i) & 0x01) ? 1 : 0;

            }

        }

    }

    for (i = 0; i < imax; i++)
    {

        for (j = 0; j < jmax; j++)
        {

            if ((bit1[i][j] == 0) && (bit2[i][j] == 0))
                sprite[i][j] = 0;
            else if ((bit1[i][j] == 1) && (bit2[i][j] == 0))
                sprite[i][j] = 1 + ((attribs & 0x03) << 0x02);
            else if ((bit1[i][j] == 0) && (bit2[i][j] == 1))
                sprite[i][j] = 2 + ((attribs & 0x03) << 0x02);
            else if ((bit1[i][j] == 1) && (bit2[i][j] == 1))
                sprite[i][j] = 3 + ((attribs & 0x03) << 0x02);

            if (spr_nr == 0)
            {
             #ifdef use_lib_resizeram_ppu            
             #else
              ppu_sprite_cache[x + i][y + j] = sprite[i][j];
             #endif 
            }

            if (sprite[i][j] != 0)
            {

                if (!disp_spr_back)
                {

                    if (PPUMASK_SPRITESHOW)
                    {
                     #ifdef use_lib_fast_sdl
                      //jj_fast_drawpixel(x + i, y + j, ppu_memory[0x3f10 + (sprite[i][j])]);
                      if ((y + j)<240)
                      {//Se sale de 240
                       jj_inline_fast_drawpixel(x + i, y + j, ppu_memory[0x3f10 + (sprite[i][j])]);
                      }
                     #else
                      backend_drawpixel(x + i, y + j, ppu_memory[0x3f10 + (sprite[i][j])]);
                     #endif 
                    }

                }

                else
                {
                 #ifdef use_lib_resizeram_ppu
                 #else   
                  if (ppu_background_cache[x + i][y + j] == 0)
                  {
                   if (PPUMASK_SPRITESHOW)
                   {
                    #ifdef use_lib_fast_sdl
                     jj_fast_drawpixel(x + i, y + j, ppu_memory[0x3f10 + (sprite[i][j])]);
                    #else
                     backend_drawpixel(x + i, y + j, ppu_memory[0x3f10 + (sprite[i][j])]);
                    #endif   
                   }
                  }
                 #endif
                }

            }

        }

    }

}

//JJ static void ppu_rendersprites()
void ppu_rendersprites()
{
 int i = 0;

 #ifdef use_lib_resizeram_ppu
 #else
  memset(ppu_sprite_cache, 0x00, sizeof (ppu_sprite_cache));
 #endif   

 for (i = 63; i >= 0; i--)
 {
  ppu_rendersprite(ppu_sprite_memory[i * 4], ppu_sprite_memory[i * 4 + 3], ppu_sprite_memory[i * 4 + 1], ppu_sprite_memory[i * 4 + 2], i);
 }
}

