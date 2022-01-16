#include "gbConfig.h"
#include "gbGlobals.h"
#include "backend.h"
#include "mmc1.h"
#include <string.h>
#include "dataFlash/gbrom.h"
static int mmc1_prgrom_area_switch;
static int mmc1_prgrom_bank_switch;
static int mmc1_chrrom_bank_switch;
static int mmc1_reg0_data = 0;
static int mmc1_reg1_data = 0;
static int mmc1_reg2_data = 0;
static int mmc1_reg3_data = 0;
static int mmc1_reg0_bitcount = 0;
static int mmc1_reg1_bitcount = 0;
static int mmc1_reg2_bitcount = 0;
static int mmc1_reg3_bitcount = 0;

static void mmc1_switchprg(int bank, int pagesize, int area)
{
 unsigned int address = (pagesize && !area) ? 0xc000 : 0x8000;
 int size = (pagesize) ? 16384 : 32768;

 //JJ backend_read(romfn, 16 + (bank * size), size, memory + address);
 //memcpy(memory + address,&gb_list_rom_data[gb_id_cur_rom][(16 + (bank * size))],size);
 backend_read_from_memory(16 + (bank * size), size, memory + address);
}

static void mmc1_switchchr(int bank, int pagesize, int area)
{
 unsigned int address = (pagesize && area) ? 0x1000 : 0x0000;
 int size = (pagesize) ? 4096 : 8192;

 //JJ backend_read(romfn, 16 + (16384 * header.prgromsize) + (bank * size), size, ppu_memory + address);
 //memcpy(ppu_memory + address,&gb_list_rom_data[gb_id_cur_rom][(16 + (16384 * header.prgromsize) + (bank * size))],size);
 backend_read_from_memory(16 + (16384 * header.prgromsize) + (bank * size), size, ppu_memory + address);
}

//JJ static void mmc1_access(unsigned int address, unsigned char data)
void mmc1_access(unsigned int address, unsigned char data)
{

    if (address > 0x7fff && address < 0xa000)
    {

        if (data & 0x80)
        {

            mmc1_reg0_data = 0;
            mmc1_reg0_bitcount = 0;

        }

        else
        {

            mmc1_reg0_data |= data;
            mmc1_reg0_bitcount++;

        }

        if (mmc1_reg0_bitcount == 5)
        {

            if (mmc1_reg0_data & 0x01)
                MIRRORING = 0;
            else
                MIRRORING = 1;

            if (mmc1_reg0_data & 0x02)
                OS_MIRROR = 1;
            else
                OS_MIRROR = 0;

            if (mmc1_reg0_data & 0x04)
                mmc1_prgrom_area_switch = 1;
            else
                mmc1_prgrom_area_switch = 0;

            if (mmc1_reg0_data & 0x08)
                mmc1_prgrom_bank_switch = 1;
            else
                mmc1_prgrom_bank_switch = 0;

            if (mmc1_reg0_data & 0x10)
                mmc1_chrrom_bank_switch = 1;
            else
                mmc1_chrrom_bank_switch = 0;

            mmc1_reg0_data = 0;
            mmc1_reg0_bitcount = 0;

        }

    }

    if (address > 0x9fff && address < 0xc000)
    {

        if (data & 0x80)
        {

            mmc1_reg1_data = 0;
            mmc1_reg1_bitcount = 0;

        }

        else
        {

            mmc1_reg1_bitcount++;
            mmc1_reg1_data |= (data & 0x01) << mmc1_reg1_bitcount;

        }

        if (mmc1_reg1_bitcount == 5)
        {

            if (mmc1_reg1_data != 0x00)
                mmc1_switchchr(mmc1_reg1_data >> 1, mmc1_chrrom_bank_switch, 0);

            mmc1_reg1_data = 0;
            mmc1_reg1_bitcount = 0;

        }

    }

    if (address > 0xbfff && address < 0xe000)
    {

        if (data & 0x80)
        {

            mmc1_reg2_data = 0;
            mmc1_reg2_bitcount = 0;

        }

        else
        {

            mmc1_reg2_bitcount++;
            mmc1_reg2_data |= (data & 0x01) << mmc1_reg2_bitcount;

        }

        if (mmc1_reg2_bitcount == 5)
        {

            if (mmc1_reg2_data != 0x00)
                mmc1_switchchr(mmc1_reg2_data >> 1, mmc1_chrrom_bank_switch, 1);

            mmc1_reg2_data = 0;
            mmc1_reg2_bitcount = 0;

        }

    }

    if (address > 0xdfff && address < 0x10000)
    {

        if (data & 0x80)
        {

            mmc1_reg3_data = 0;
            mmc1_reg3_bitcount = 0;

        }

        else
        {

            mmc1_reg3_bitcount++;
            mmc1_reg3_data |= (data & 0x01) << mmc1_reg3_bitcount;

        }

        if (mmc1_reg3_bitcount == 5)
        {

            mmc1_switchprg(mmc1_reg3_data >> 1, mmc1_prgrom_bank_switch, mmc1_prgrom_area_switch);

            mmc1_reg3_data = 0;
            mmc1_reg3_bitcount = 0;

        }

    }

}

//JJ static void mmc1_reset()
void mmc1_reset()
{

}

