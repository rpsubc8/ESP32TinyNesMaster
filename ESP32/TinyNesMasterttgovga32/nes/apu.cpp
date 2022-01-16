#include "gbConfig.h"
#include "gbGlobals.h"
#include "apu.h"
//JJ static unsigned char apu_memread(unsigned int address)
unsigned char apu_memread(unsigned int address)
{

    /*
    if (address == 0x4015)
    {

    }
    */

    return memory[address];

}

unsigned char gb_frec_canal1_low=0;
unsigned char gb_frec_canal1_high=0;
unsigned char gb_frec_canal2_low=0;
unsigned char gb_frec_canal2_high=0;
unsigned char gb_frec_canal3_low=0;
unsigned char gb_frec_canal3_high=0;

//JJ static unsigned char apu_memwrite(unsigned int address, unsigned char data)
unsigned char apu_memwrite(unsigned int address, unsigned char data)
{
#ifdef use_lib_sound_ricoh2A03    
 unsigned int aux,auxFrec;
 //memory[address] = data;
 //Canal 1
 if (address == 0x4000){     
  gbVolMixer_now[0]= ((data & 0x0F)==0)?0:15;
  return memory[address];
 }
 if (address == 0x4002){
  gb_frec_canal1_low = data;
  //printf("1 LOW:%d\n",gb_frec_canal1_low);
  return memory[address];
 }
 if (address == 0x4003){
  gb_frec_canal1_high = (data & 0x07);
  //printf("1 HIGH:%d\n",gb_frec_canal1_high);
  aux= gb_frec_canal1_high;
  aux= aux<<8;
  aux= aux | gb_frec_canal1_low;
  auxFrec= (unsigned int)(1.789773 * 1000000 / (16*(aux+1)));
  if (auxFrec>12000){
   auxFrec= 12000;
  }  
  gbFrecMixer_now[0]= (unsigned short int)auxFrec;
  //printf("Canal1 Dato:%d Frec:%d\n",aux,auxFrec);
  //fflush(stdout);
  return memory[address];
 }
 
 //Canal2
 if (address == 0x4004){
  //gbVolMixer_now[1]= (data & 0x0F)>>3;
  gbVolMixer_now[1]= ((data & 0x0F)==0)?0:15;
  return memory[address];
 }
 if (address == 0x4006){
  gb_frec_canal2_low = data;
  return memory[address];
 }
 if (address == 0x4007){
  gb_frec_canal2_high = data & 0x07;
  aux= gb_frec_canal2_high;
  aux= aux<<8;
  aux= aux | gb_frec_canal2_low;
  auxFrec= (unsigned int)(1.789773 * 1000000 / (16*(aux+1)));  
  if (auxFrec>12000){
   auxFrec= 12000;
  }  
  gbFrecMixer_now[1]= (unsigned short int)auxFrec;
  //printf("Canal2 Dato:%d Frec:%d\n",aux,auxFrec);
  //fflush(stdout); 
  return memory[address];
 } 
 
 
 //Canal3 Triangulo
 if (address == 0x4008){
  //gbVolMixer_now[2]= data & 0x0F;
  //gbVolMixer_now[2]= ((data & 0x01) == 0x01)? 2: 0;
  gbVolMixer_now[2]= ((data & 0x01) == 0x01)?15:0;
  return memory[address];
 }
 if (address == 0x400A){
  gb_frec_canal3_low = data;
  return memory[address];
 }
 if (address == 0x400B){
  gb_frec_canal3_high = data & 0x07;
  aux= gb_frec_canal3_high;
  aux= aux<<8;
  aux= aux | gb_frec_canal3_low;
  auxFrec= (unsigned int)(1.789773 * 1000000 / (16*(aux+1)));  
  if (auxFrec>12000){
   auxFrec= 12000;
  }  
  gbFrecMixer_now[2]= (unsigned short int)auxFrec;
  gbVolMixer_now[2]= (gbFrecMixer_now[2] < 100)? 0: 2;
  
  //printf("Canal3 Dato:%d Frec:%d\n",aux,auxFrec);
  //fflush(stdout);
  return memory[address];
 } 
 
 
 if (address == 0x4015)
 {//MIXER
  //printf("MIXER:%02X\n",data);
  //fflush(stdout);
  if ((data & 0x01) != 0x01)
   gbVolMixer_now[0]= 0; 
  if ((data & 0x02) != 0x02) 
   gbVolMixer_now[1]= 0; 
  if ((data & 0x04) != 0x04) 
   gbVolMixer_now[2]= 0;  
  return memory[address];
 }
#endif 
    /*
    if (address == 0x4000 || address == 0x4004)
    {

        unsigned char duty = data & 0xC0;
        unsigned char lcounter = data & 0x20;
        unsigned char cvolume = data & 0x10;
        unsigned char volume = data & 0x0F;

    }

    if (address == 0x4001 || address == 0x4005)
    {

        unsigned char enabled = data & 0x80;
        unsigned char period = data & 0x70;
        unsigned char negate = data & 0x08;
        unsigned char shift = data & 0x07;

    }

    if (address == 0x4002 || address == 0x4006)
    {

        unsigned char timerlo = data;

    }

    if (address == 0x4003 || address == 0x4007)
    {

        unsigned char lcounter = data & 0xF8;
        unsigned char timerhi = data & 0x07;

    }

    if (address == 0x4008)
    {

        unsigned char lcounterctrl = data & 0x80;
        unsigned char lcounterload = data & 0x7F;

    }

    if (address == 0x400A)
    {

        unsigned char timerlo = data;

    }

    if (address == 0x400B)
    {

        unsigned char lcounterload = data & 0xF7;
        unsigned char timerhi = data & 0x08;

    }

    if (address == 0x400C)
    {

        unsigned char lcounterhalt = data & 0x20;
        unsigned char cvolume = data & 0x10;
        unsigned char volume = data & 0x0F;

    }

    if (address == 0x400E)
    {

        unsigned char loopnoise = data & 0x80;
        unsigned char noiseperiod = data & 0x0F;

    }

    if (address == 0x400F)
    {

        unsigned char lcounterload = data & 0xF8;

    }

    if (address == 0x4010)
    {

        unsigned char irqenable = data & 0x80;
        unsigned char loop = data & 0x40;
        unsigned char frequency = data & 0x0F;

    }

    if (address == 0x4011)
    {

        unsigned char loadcounter = data & 0x7F;

    }

    if (address == 0x4012)
    {

        unsigned char sampleaddress = data;

    }

    if (address == 0x4013)
    {

        unsigned char samplelength = data;

    }

    if (address == 0x4015)
    {

        unsigned char enable = data & 0x10;
        unsigned char noise = data & 0x08;
        unsigned char triangle = data & 0x04;
        unsigned char pulse2 = data & 0x02;
        unsigned char pulse1 = data & 0x01;

    }

    if (address == 0x4017)
    {

        unsigned char mode = data & 0x80;
        unsigned char irqinhibit = data & 0x40;

    }
    */

    return memory[address];

}
