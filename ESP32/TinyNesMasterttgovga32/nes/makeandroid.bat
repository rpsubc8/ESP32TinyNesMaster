@echo off
echo Convert ANDROID PROJECT DATA (WIN32)
echo Delete dataFlash (Borrar dataFlash)
pause

echo Process gbrom.h (Procesar gbrom.h)
fart dataFlash\gbrom.h "rom/gbrom" "gbrom"
ping -n 2 127.0.0.1>nul

echo Copy gbrom.h (Copiar gbrom.h)
copy/Y dataFlash\gbrom.h
ping -n 2 127.0.0.1>nul


echo Copy rom (Copiar rom)
copy/Y dataFlash\rom\*.h
ping -n 2 127.0.0.1>nul



echo Process mmc1.cpp (Procesar mmc1.cpp)
fart mmc1.cpp "dataFlash/gbrom.h" "gbrom.h"
ping -n 2 127.0.0.1>nul

echo Process mmc3.cpp (Procesar mmc3.cpp)
fart mmc3.cpp "dataFlash/gbrom.h" "gbrom.h"
ping -n 2 127.0.0.1>nul

echo Process nes.ino (Procesar nes.ino)
fart nes.ino "dataFlash/gbrom.h" "gbrom.h"
ping -n 2 127.0.0.1>nul

echo Process osd.cpp (Procesar osd.cpp)
fart osd.cpp "dataFlash/gbrom.h" "gbrom.h"
ping -n 2 127.0.0.1>nul

echo Process rom.cpp (Procesar rom.cpp)
fart rom.cpp "dataFlash/gbrom.h" "gbrom.h"
ping -n 2 127.0.0.1>nul

echo Process sdl.cpp (Procesar sdl.cpp)
fart sdl.cpp "dataFlash/gbrom.h" "gbrom.h"
ping -n 2 127.0.0.1>nul

echo Process unrom.cpp (Procesar unrom.cpp)
fart unrom.cpp "dataFlash/gbrom.h" "gbrom.h"
ping -n 2 127.0.0.1>nul


echo Delete dataFlash (Borrar dataFlash)
rmdir /S /Q dataFlash


pause