#include "gbConfig.h"
#include "gbGlobals.h"
#include "backend.h"
#include "cnrom.h"
static void cnrom_switchchr(int bank)
{
 int size = 8192;
 //JJ    backend_read(romfn, 16 + (16384 * header.prgromsize) + (bank * size), size, ppu_memory);
 backend_read_from_memory(16 + (16384 * header.prgromsize) + (bank * size), size, ppu_memory);
}

//JJ static void cnrom_access(unsigned int address, unsigned char data)
void cnrom_access(unsigned int address, unsigned char data)
{

    if (address > 0x7fff && address < 0x10000)
        cnrom_switchchr(data & (header.chrromsize - 1));

}

//JJ static void cnrom_reset()
void cnrom_reset()
{

}

