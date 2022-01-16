#ifndef _GBROM_H
 #define _GBROM_H

 #include <stddef.h>
 #include "rom/gbromAlfonzoMelee.h"
 #include "rom/gbromChaseHB.h"
 #include "rom/gbromCMC80s.h"
 #include "rom/gbromemunesplfami.h"
 #include "rom/gbromFallingHB.h"
 //#include "rom/gbromFamilyPicros.h"
 //#include "rom/gbromFFFv10HB.h"
 //#include "rom/gbromFilthyKitche.h"
 #include "rom/gbromfiredemo.h"
 #include "rom/gbromFlappyBirdHB.h"
 #include "rom/gbromFunction.h"
 //#include "rom/gbromGetItHB.h"
 #include "rom/gbromGiants.h"
 //#include "rom/gbromGruniozerca2.h"
 //#include "rom/gbromGuardian5HB.h"
 #include "rom/gbromInherentSmil.h"
 #include "rom/gbromInvadersHB.h"
 #include "rom/gbromJamminHoneyH.h"
 #include "rom/gbromLightShields.h"
 #include "rom/gbromLoveStoryHB.h"
 //#include "rom/gbromLuckyPenguin.h"
 //#include "rom/gbromLunarLimitHB.h"
 #include "rom/gbromMachineCaveH.h"
 #include "rom/gbromMarbleRunHB.h"
 #include "rom/gbromMeloJellos2H.h"
 #include "rom/gbromMilioNESy.h"
 #include "rom/gbrommojontwinsje.h"
 #include "rom/gbrommojontwinsmi.h"
 #include "rom/gbromMultiDudeHB.h"
 #include "rom/gbromNESertGolfin.h"
 //#include "rom/gbromNESRotoZoomH.h"
 //#include "rom/gbromNimNomv11.h"
 #include "rom/gbromPanesianPowe.h"

 //#define max_list_rom 43
 #define max_list_rom 23


 //rom
 //Titulos
 static const char * gb_list_rom_title[max_list_rom]={
  "AlfonzoMelee",
  "ChaseHB",
  "CMC80s",
  "emunesplfami",
  "FallingHB",
  //"FamilyPicros",
  //"FFFv10HB",
  //"FilthyKitche",
  "firedemo",
  "FlappyBirdHB",
  "Function",
  //"GetItHB",
  "Giants",
  //"Gruniozerca2",
  //"Guardian5HB",
  "InherentSmil",
  "InvadersHB",
  "JamminHoneyH",
  "LightShields",
  "LoveStoryHB",
  //"LuckyPenguin",
  //"LunarLimitHB",
  "MachineCaveH",
  "MarbleRunHB",
  "MeloJellos2H",
  "MilioNESy",
  "mojontwinsje",
  "mojontwinsmi",
  "MultiDudeHB",
  "NESertGolfin",
  //"NESRotoZoomH",
  //"NimNomv11",
  "PanesianPowe"
 };

 //Datos rom
 static const unsigned char * gb_list_rom_data[max_list_rom]={
  gb_rom_AlfonzoMelee,
  gb_rom_ChaseHB,
  gb_rom_CMC80s,
  gb_rom_emunesplfami,
  gb_rom_FallingHB,
  //gb_rom_FamilyPicros,
  //gb_rom_FFFv10HB,
  //gb_rom_FilthyKitche,
  gb_rom_firedemo,
  gb_rom_FlappyBirdHB,
  gb_rom_Function,
  //gb_rom_GetItHB,
  gb_rom_Giants,
  //gb_rom_Gruniozerca2,
  //gb_rom_Guardian5HB,
  gb_rom_InherentSmil,
  gb_rom_InvadersHB,
  gb_rom_JamminHoneyH,
  gb_rom_LightShields,
  gb_rom_LoveStoryHB,
  //gb_rom_LuckyPenguin,
  //gb_rom_LunarLimitHB,
  gb_rom_MachineCaveH,
  gb_rom_MarbleRunHB,
  gb_rom_MeloJellos2H,
  gb_rom_MilioNESy,
  gb_rom_mojontwinsje,
  gb_rom_mojontwinsmi,
  gb_rom_MultiDudeHB,
  gb_rom_NESertGolfin,
  //gb_rom_NESRotoZoomH,
  //gb_rom_NimNomv11,
  gb_rom_PanesianPowe
 };

/*
 //Tamanio en bytes
 static const int gb_list_rom_size[max_list_rom]={
  65552,
  24592,
  49168,
  40976,
  40976,
  40976,
  65552,
  65552,
  40976,
  24592,
  16400,
  24592,
  24592,
  65552,
  49168,
  65552,
  32784,
  65552,
  24592,
  32784,
  65552,
  40976,
  40976,
  40976,
  40976,
  32784,
  40976,
  65552,
  40976,
  32784,
  40976,
  40976,
  131088
 };
 */

#endif








/*

#ifndef _GBROM_H
 #define _GBROM_H
 
 //#include "rom/romArkanoid.h" //OK
 //#include "rom/romContra.h" //OK bancos se ven mal algunos sprites
 //#include "rom/romMariobro.h" //OK
 //#include "rom/romSupermariobros3.h" //KO mal algunos sprites
 
 //#include "rom/gbromfullscreen.h" //KO
 //#include "rom/gbromLMNA.h" //KO
 //#include "rom/gbromny2011.h" //KO
 //#include "rom/gbromSayoonara.h" //KO
 //#include "rom/gbromStarsSEfix2.h"
 //#include "rom/gbromXMAS2001.h" 
 
 //#include "rom/gbromAuge.h" //Hace cosas raras id colores mas de 64
 
 #include "rom/gbromChaseHB.h" //OK
 #include "rom/gbromCMC80s.h" //OK
 #include "rom/gbromemunesplfami.h" //OK
 #include "rom/gbromfiredemo.h" //OK 
 #include "rom/gbromGiants.h" //OK
 #include "rom/gbromMilioNESy.h" //OK
 #include "rom/gbrommojontwinsmi.h" //OK
   
 //#define max_list_rom 4
 //#define max_list_rom 7
 #define max_list_rom 7
 
 static const char * gb_list_rom_title[max_list_rom]={
  //"Auge",
  //"Arkanoid",
  //"fullscreen",
  //"LMNA",
  //"ny2011",
  //"Sayoonara",
  //"StarsSEfix2",  
  //"XMAS2001",
    
  "ChaseHB",
  "CMC80s",
  "emunesplfami",
  "firedemo",
  "Giants",
  "MilioNESy",  
  "mojontwinsmi"
  
  //"Contra.nes",  
  //"Mario",
  //"SMB3"
  
  //"BombSweeper.nes",
  //"Bubble.nes",
  //"Mariobro.nes",
  //"Supermariobros3.nes",
  //"Tennis.nes"
  
 };
  
 static const unsigned char * gb_list_rom_data[max_list_rom]={
  //gb_rom_Auge,
  //gb_rom_arkanoid,  
  //gb_rom_fullscreen,
  //gb_rom_LMNA,
  //gb_rom_ny2011,
  //gb_rom_Sayoonara,
  //gb_rom_StarsSEfix2,
  //gb_rom_XMAS2001,  
    
  gb_rom_ChaseHB,
  gb_rom_CMC80s,
  gb_rom_emunesplfami,
  gb_rom_firedemo,  
  gb_rom_Giants,
  gb_rom_MilioNESy,
  gb_rom_mojontwinsmi
  
  //gb_rom_contra,  
  //gb_rom_mariobro,
  //gb_rom_supermariobros3  
  
  //gb_rom_arkanoid,
  //gb_rom_arkanoid,
  //gb_rom_arkanoid,
  //gb_rom_arkanoid,
  //gb_rom_arkanoid         
  
 }; 
 
#endif
 
*/




/*
#ifndef _GBROM_H
 #define _GBROM_H
 
 //#include "rom/romContra.h"
 //#include "rom/romArkanoid.h"
 //#include "rom/gbromAuge.h"
 //#include "rom/gbromChaseHB.h"
 //#include "rom/gbromCMC80s.h"
 //#include "rom/gbromemunesplfami.h"
 //#include "rom/gbromfiredemo.h"
 //#include "rom/gbromGiants.h"
 //#include "rom/gbromMilioNESy.h"
 //#include "rom/gbrommojontwinsmi.h"


 #include "rom/gbromChaseHB.h" //OK
 #include "rom/gbromCMC80s.h" //OK
 #include "rom/gbromemunesplfami.h" //OK
 #include "rom/gbromfiredemo.h" //OK 
 #include "rom/gbromGiants.h" //OK
 #include "rom/gbromMilioNESy.h" //OK
 #include "rom/gbrommojontwinsmi.h"//OK



 //#include "rom/romMariobro.h"
 //#include "rom/romSupermariobros3.h"
 
   
 //#define max_list_rom 4
 #define max_list_rom 7
 
 static const char * gb_list_rom_title[max_list_rom]={
  "ChaseHB",
  "CMC80s",
  "emunesplfami",
  "firedemo",
  "Giants",
  "MilioNESy",  
  "mojontwinsmi"


  //"Contra.nes",
  //"Arkanoid.nes",
  //"Auge",
  //"ChaseHB",
  //"CMC80s",
  //"emunesplfami",
  //"firedemo",
  //"Giants",
  //"MilioNESy",    
  //"mojontwinsmi"
  
  //,
  //"Mario",
  //"SMB3"
  //
 };
  
 static const unsigned char * gb_list_rom_data[max_list_rom]={
  //gb_rom_contra,
  gb_rom_ChaseHB,
  gb_rom_CMC80s,
  gb_rom_emunesplfami,
  gb_rom_firedemo,  
  gb_rom_Giants,
  gb_rom_MilioNESy,
  gb_rom_mojontwinsmi


//  gb_rom_arkanoid,
//  gb_rom_Auge,
//  gb_rom_ChaseHB,
//  gb_rom_CMC80s,
//  gb_rom_emunesplfami,
//  gb_rom_firedemo,
//  gb_rom_Giants,
//  gb_rom_MilioNESy,    
//  gb_rom_mojontwinsmi  

//  ,
//  gb_rom_mariobro,
//  gb_rom_supermariobros3
  
 }; 
 
#endif
 
*/