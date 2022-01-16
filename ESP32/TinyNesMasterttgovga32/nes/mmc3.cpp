#include "gbGlobals.h"
#include "backend.h"
#include "mmc3.h"
#include <string.h>
#include "dataFlash/gbrom.h"
static unsigned char mmc3_cmd;
static int mmc3_prg_bank0 = 0;
static int mmc3_prg_bank1 = 0;
static int mmc3_prg_page = 0;
static int mmc3_chr_xor = 0;
//JJ static int mmc3_irq_counter = 0;
int mmc3_irq_counter = 0;
static int mmc3_irq_latch;
//JJ static int mmc3_irq_enable = 0;
int mmc3_irq_enable=0;

static void mmc3_switchprg(unsigned int address, int bank)
{
 int size = 8192;
 //JJ backend_read(romfn, 16 + (bank * size), size, memory + address);
 //memcpy(memory + address,&gb_list_rom_data[gb_id_cur_rom][(16 + (bank * size))],size);
 backend_read_from_memory(16 + (bank * size), size, memory + address);
}

static void mmc3_switchchr(unsigned int address, int bank, int pagecount)
{
 int size = 1024;
 //JJ backend_read(romfn, 16 + (16384 * header.prgromsize) + (bank * size), size * pagecount, ppu_memory + address);
 //memcpy(ppu_memory + address,&gb_list_rom_data[gb_id_cur_rom][((16384 * header.prgromsize) + (bank * size))],size * pagecount);
 backend_read_from_memory(16 + (16384 * header.prgromsize) + (bank * size), size * pagecount, ppu_memory + address);
}

//JJ static void mmc3_access(unsigned int address, unsigned char data)
void mmc3_access(unsigned int address, unsigned char data)
{

    switch (address)
    {

    case 0x8000:
        mmc3_cmd = data;

        if (data & 0x40)
        {

            if (mmc3_prg_page != 1)
            {

                mmc3_switchprg(0x8000, (header.prgromsize * 2) - 2);
                mmc3_switchprg(0xc000, mmc3_prg_bank0);

                mmc3_prg_page = 1;

            }

        }

        else
        {

            if (mmc3_prg_page != 0)
            {

                mmc3_switchprg(0xc000, (header.prgromsize * 2) - 2);
                mmc3_switchprg(0x8000, mmc3_prg_bank0);

                mmc3_prg_page = 0;

            }

        }

        mmc3_chr_xor = (data & 0x80) ? 1 : 0;

        break;

    case 0x8001:
        switch (mmc3_cmd & 0x07)
        {

        case 0:
            if (mmc3_chr_xor == 0)
                mmc3_switchchr(0x0000, data, 2);
            else
                mmc3_switchchr(0x1000, data, 2);

            break;

        case 1:
            if (mmc3_chr_xor == 0)
                mmc3_switchchr(0x0800, data, 2);
            else
                mmc3_switchchr(0x1800, data, 2);

            break;

        case 2:
            if (mmc3_chr_xor == 0)
                mmc3_switchchr(0x1000, data, 1);
            else
                mmc3_switchchr(0x0000, data, 1);

            break;

        case 3:
            if (mmc3_chr_xor == 0)
                mmc3_switchchr(0x1400, data, 1);
            else
                mmc3_switchchr(0x400, data, 1);

            break;

        case 4:
            if (mmc3_chr_xor == 0)
                mmc3_switchchr(0x1800, data, 1);
            else
                mmc3_switchchr(0x800, data, 1);

            break;

        case 5:
            if (mmc3_chr_xor == 0)
                mmc3_switchchr(0x1c00, data, 1);
            else
                mmc3_switchchr(0x0c00, data, 1);

            break;

        case 6:
            mmc3_prg_bank0 = data;

            if (mmc3_cmd & 0x40)
                mmc3_switchprg(0xc000, data);
            else
                mmc3_switchprg(0x8000, data);

            break;

        case 7:
            mmc3_prg_bank1 = data;
            mmc3_switchprg(0xa000, data);

            break;

        }

        break;

    case 0xa000:
        if (data & 0x01)
            MIRRORING = 1;
        else
            MIRRORING = 0;

        break;

    case 0xa001:
        if (data)
            SRAM = 1;

        break;

    case 0xc000:
        if (mmc3_irq_enable == 0)
            mmc3_irq_counter = data;

        break;

    case 0xc001:
        mmc3_irq_latch = data;

        break;

    case 0xe000:
        mmc3_irq_counter = mmc3_irq_latch;
        mmc3_irq_enable = 0;

        break;

    case 0xe001:
        mmc3_irq_enable = 1;

        break;

    }

}

//JJ static void mmc3_reset()
void mmc3_reset()
{
 //JJ backend_read(romfn, 16, 8192, memory + 0xa000);
 //memcpy(memory + 0xa000,&gb_list_rom_data[gb_id_cur_rom][16],8192);
 backend_read_from_memory(16, 8192, memory + 0xa000);
}

