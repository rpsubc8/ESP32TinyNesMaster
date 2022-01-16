#include <stdlib.h>
#include "gbConfig.h"
#include "gbGlobals.h"
#include "instructions.h"
#include "nes.h"
#include "ppu.h"
#include "backend.h"
#include "apu.h"

struct mapper
{
 void (*reset)();
 void (*access)(unsigned int address, unsigned char data);
};
extern struct mapper *mapper;

static unsigned char zero_flag;
static unsigned char sign_flag;
static unsigned char overflow_flag;
static unsigned char break_flag;
static unsigned char decimal_flag;
static unsigned char interrupt_flag;
static unsigned char carry_flag;
static unsigned int addr;
static unsigned int program_counter;
static unsigned int tmp, tmp2, tmp3;
static unsigned char accumulator;
static unsigned char stack_pointer;
static unsigned char status_register;
static unsigned char x_reg;
static unsigned char y_reg;

static int cycle_count; //Para optmizar CPU


#ifdef use_lib_cpu_ADC_IM
 inline void ADC_IM(int auxCYCLES)
 {
  addr = memory[program_counter];
  tmp = accumulator + addr + (carry_flag ? 1 : 0);
  overflow_flag = (~(accumulator ^ addr)) & (accumulator ^ addr) & 0x80;
  carry_flag = tmp > 0xFF;
  accumulator = tmp & 0xFF;
  sign_flag = accumulator & 0x80;
  zero_flag = !(accumulator);
  program_counter++;
  cycle_count -= auxCYCLES;
 }
#endif

#ifdef use_lib_cpu_ADC_ZP
 inline void ADC_ZP(int auxCYCLES)
 {
  addr = ram_read(memory[program_counter]);
  tmp = accumulator + addr + (carry_flag ? 1 : 0);
  overflow_flag = (~(accumulator ^ addr)) & (accumulator ^ addr) & 0x80;
  carry_flag = tmp > 0xFF;
  accumulator = tmp & 0xFF;
  sign_flag = accumulator & 0x80;
  zero_flag = !(accumulator);
  program_counter++;
  cycle_count -= auxCYCLES;    
 }
#endif

//*****************************************************************
//Optimizacion
static void ram_write(unsigned short int address, unsigned char data)
{
 if (address < 0x2000)
 {
  memory[address] = data;
  memory[address + 2048] = data;
  memory[address + 4096] = data;
  memory[address + 6144] = data;
  return;
 }
 
 if ((address >= 0x2000 && address < 0x4000) || address == 0x4014)
 {
  memory[address] = ppu_memwrite(address, data);
  return;
 }
 
 if ((address >= 0x4000 && address < 0x4014) || address == 0x4015 || address == 0x4017)
 {
  memory[address] = apu_memwrite(address, data);
  //memory[address] = memory[address]; //optimizado
  return;
 }
 
 if (address == 0x4016)
 {
  memory[address] = 0x40;
  return;
 }
 
 if (address >= 0x6000 && address < 0x8000)
 {
  if (SRAM == 1){
   backend_writesavefile("game.sav", memory);
  }
  memory[address] = data;
  return;
 }
 
 if (mapper->access){
  mapper->access(address, data);
 }
 memory[address] = data;
}

//*****************************************************************
//Optimizacion
static inline unsigned short int input_memread(unsigned short int address)
{
 address = pad1_state[pad1_readcount];
 if (pad1_readcount == 7){
  pad1_readcount = 0;
 } 
 else{
  pad1_readcount++;
 }
 return address;
}

//*****************************************************************
//Optimizacion
static unsigned char ram_read(unsigned short int address)
{
 if ((address >= 0x2000 && address < 0x4000) || address == 0x4014)
  return ppu_memread(address);
 if ((address >= 0x4000 && address < 0x4014) || address == 0x4015 || address == 0x4017)
 {
  //JJ return apu_memread(address);
  return memory[address]; //optimizado
 }
 if (address == 0x4016)
  return input_memread(address);
 return memory[address];
}

//JJ static void update_status_register()
static inline void update_status_register()
{
 status_register = ((sign_flag ? 0x80 : 0) | (zero_flag ? 0x02 : 0) | (carry_flag ? 0x01 : 0) | (interrupt_flag ? 0x04 : 0) | (decimal_flag ? 0x08 : 0) | (overflow_flag ? 0x40 : 0) | (break_flag ? 0x10 : 0) | 0x20);
}

//JJ int cpu_irq(int cycles, unsigned char *memory)
int cpu_irq(int cycles)
{

    PUSH_ST((program_counter & 0xff00) >> 8);
    PUSH_ST(program_counter & 0xff);
    PUSH_ST(GET_SR());

    break_flag = 0;
    interrupt_flag = 1;
    program_counter = (memory[0xffff] << 8) | memory[0xfffe];

    return cycles -= 7;

}

//JJ int cpu_nmi(int cycles, unsigned char *memory)
int cpu_nmi(int cycles)
{

    PUSH_ST((program_counter & 0xff00) >> 8);
    PUSH_ST(program_counter & 0xff);
    PUSH_ST(GET_SR());

    break_flag = 0;
    interrupt_flag = 1;
    program_counter = (memory[0xfffb] << 8) | memory[0xfffa];

    return cycles -= 7;

}

//JJ void cpu_reset(unsigned char *memory)
void cpu_reset()
{
 status_register = 0x20;
 zero_flag = 1;
 sign_flag = 0;
 overflow_flag = 0;
 break_flag = 0;
 decimal_flag = 0;
 interrupt_flag = 0;
 carry_flag = 0;
 stack_pointer = 0xff;
 program_counter = (memory[0xfffd] << 8) | memory[0xfffc];
 accumulator = x_reg = y_reg = 0;
}


//JJ int cpu_execute(int cycles, unsigned char *memory)
int cpu_execute(int cycles)
{

    //JJ int cycle_count = cycles;
    cycle_count = cycles;
    unsigned char opcode;

    do
    {

        update_status_register();        

        opcode = memory[program_counter++];

        switch (opcode)
        {

        case 0x69:
         #ifdef use_lib_cpu_ADC_IM
          ADC_IM(2); break;
         #else
          ADC_IM(2);
         #endif          
        case 0x65:
         #ifdef use_lib_cpu_ADC_ZP
          ADC_ZP(3); break;
         #else
          ADC_ZP(3);
         #endif 
        case 0x75: ADC_ZPIX(4);
        case 0x6D: ADC_A(4);
        case 0x7D: ADC_AIX(5);
        case 0x79: ADC_AIY(4);
        case 0x61: ADC_IDI(6);
        case 0x71: ADC_INI(5);
        case 0x29: AND_IM(2);
        case 0x25: AND_ZP(3);
        case 0x35: AND_ZPIX(4);
        case 0x2D: AND_A(4);
        case 0x3D: AND_AIX(5);
        case 0x39: AND_AIY(4);
        case 0x21: AND_IDI(6);
        case 0x31: AND_INI(5);
        case 0x0A: ARITH_SL_ACC(2);
        case 0x06: ARITH_SL_ZP(5);
        case 0x16: ARITH_SL_ZPIX(6);
        case 0x0E: ARITH_SL_A(6);
        case 0x1E: ARITH_SL_AIX(7);
        case 0x90: BRANCH_CC(2);
        case 0xB0: BRANCH_CS(2);
        case 0xF0: BRANCH_ZS(2);
        case 0x24: BIT_TEST_ZP(3);
        case 0x2C: BIT_TEST_A(4);
        case 0x30: BRANCH_RM(2);
        case 0xD0: BRANCH_ZR(2);
        case 0x10: BRANCH_RP(2);
        case 0x00: BREAK(7);
        case 0x50: BRANCH_OC(2);
        case 0x70: BRANCH_OS(4);
        case 0x18: CLEAR_CF(2);
        case 0xD8: CLEAR_DM(2);
        case 0x58: CLEAR_ID(2);
        case 0xB8: CLEAR_OF(2);
        case 0xC9: COMP_MEM_IM(accumulator,2);
        case 0xC5: COMP_MEM_ZP(accumulator,3);
        case 0xD5: COMP_MEM_ZPIX(accumulator,4);
        case 0xCD: COMP_MEM_A(accumulator,4);
        case 0xDD: COMP_MEM_AIX(accumulator,5);
        case 0xD9: COMP_MEM_AIY(accumulator,4);
        case 0xC1: COMP_MEM_IDI(accumulator,6);
        case 0xD1: COMP_MEM_INI(accumulator,6);
        case 0xE0: COMP_MEM_IM(x_reg,2);
        case 0xE4: COMP_MEM_ZP(x_reg,3);
        case 0xEC: COMP_MEM_A(x_reg,4);
        case 0xC0: COMP_MEM_IM(y_reg,2);
        case 0xC4: COMP_MEM_ZP(y_reg,3);
        case 0xCC: COMP_MEM_A(y_reg,4);
        case 0xC6: DECR_MEM_ZP(5);
        case 0xD6: DECR_MEM_ZPIX(6);
        case 0xCE: DECR_MEM_A(6);
        case 0xDE: DECR_MEM_AIX(7);
        case 0xCA: DECR(x_reg,2);
        case 0x88: DECR(y_reg,2);
        case 0x49: EXCL_OR_MEM_IM(2);
        case 0x45: EXCL_OR_MEM_ZP(3);
        case 0x55: EXCL_OR_MEM_ZPIX(4);
        case 0x4D: EXCL_OR_MEM_A(6);
        case 0x5D: EXCL_OR_MEM_AIX(5);
        case 0x59: EXCL_OR_MEM_AIY(4);
        case 0x41: EXCL_OR_MEM_IDI(6);
        case 0x51: EXCL_OR_MEM_INI(5);
        case 0xE6: INCR_MEM_ZP(5);
        case 0xF6: INCR_MEM_ZPIX(6);
        case 0xEE: INCR_MEM_A(6);
        case 0xFE: INCR_MEM_AIX(7);
        case 0xE8: INCR(x_reg,2);
        case 0xC8: INCR(y_reg,2);
        case 0x4c: JMP_A(3);
        case 0x6c: JMP_AI(5);
        case 0x20: JSR(6);
        case 0xA9: LOAD_IM(accumulator,2);
        case 0xA5: LOAD_ZP(accumulator,3);
        case 0xB5: LOAD_ZPIX(accumulator,4);
        case 0xAD: LOAD_A(accumulator,4);
        case 0xBD: LOAD_AIX(accumulator,4);
        case 0xB9: LOAD_AIY(accumulator,4);
        case 0xA1: LOAD_IDI(accumulator,6);
        case 0xB1: LOAD_INI(accumulator,5);
        case 0xA2: LOAD_IM(x_reg,2);
        case 0xA6: LOAD_ZP(x_reg,3);
        case 0xB6: LOAD_ZPIY(x_reg,4);
        case 0xAE: LOAD_A(x_reg,4);
        case 0xBE: LOAD_AIY(x_reg,4);
        case 0xA0: LOAD_IM(y_reg,2);
        case 0xA4: LOAD_ZP(y_reg,3);
        case 0xB4: LOAD_ZPIX(y_reg,4);
        case 0xAC: LOAD_A(y_reg,4);
        case 0xBC: LOAD_AIX(y_reg,4);
        case 0x4A: LOGIC_SHIFT_R_ACC(2);
        case 0x46: LOGIC_SHIFT_R_ZP(5);
        case 0x56: LOGIC_SHIFT_R_ZPIX(6);
        case 0x4E: LOGIC_SHIFT_R_A(6);
        case 0x5E: LOGIC_SHIFT_R_AIX(7);
        //JJ case 0xEA: NOP(2);
        case 0xEA: NOPFIX(2);
        case 0x09: OR_MEM_IM(2);
        case 0x05: OR_MEM_ZP(3);
        case 0x15: OR_MEM_ZPIX(4);
        case 0x0D: OR_MEM_A(4);
        case 0x1D: OR_MEM_AIX(5);
        case 0x19: OR_MEM_AIY(4);
        case 0x01: OR_MEM_IDI(6);
        case 0x11: OR_MEM_INI(5);
        case 0x48: PUSH_A(accumulator,3);
        case 0x08: PUSH_PS(3);
        case 0x68: PULL_A(accumulator,4);
        case 0x28: PULL_PS(4);
        case 0x2A: ROTATE_LEFT_ACC(2);
        case 0x26: ROTATE_LEFT_ZP(5);
        case 0x36: ROTATE_LEFT_ZPIX(6);
        case 0x2E: ROTATE_LEFT_A(6);
        case 0x3E: ROTATE_LEFT_AIX(7);
        case 0x6A: ROTATE_RIGHT_ACC(2);
        case 0x66: ROTATE_RIGHT_ZP(5);
        case 0x76: ROTATE_RIGHT_ZPIX(6);
        case 0x6E: ROTATE_RIGHT_A(6);
        case 0x7E: ROTATE_RIGHT_AIX(7);
        case 0x40: RET_INT(4);
        case 0x60: RET_SUB(4);
        case 0xE9: SUB_ACC_IM(2);
        case 0xE5: SUB_ACC_ZP(3);
        case 0xF5: SUB_ACC_ZPIX(4);
        case 0xED: SUB_ACC_A(4);
        case 0xFD: SUB_ACC_AIX(5);
        case 0xF9: SUB_ACC_AIY(4);
        case 0xE1: SUB_ACC_IDI(6);
        case 0xF1: SUB_ACC_INI(5);
        case 0x38: SET_C_FLAG(2);
        case 0xF8: SET_D_MODE(2);
        case 0x78: SET_INT_DIS(2);
        case 0x85: STORE_ZP(accumulator,3);
        case 0x95: STORE_ZPIX(accumulator,4);
        case 0x8D: STORE_A(accumulator,4);
        case 0x9D: STORE_AIX(accumulator,5);
        case 0x99: STORE_AIY(accumulator,5);
        case 0x81: STORE_IDI(accumulator,6);
        case 0x91: STORE_INI(accumulator,6);
        case 0x86: STORE_ZP(x_reg,3);
        case 0x96: STORE_ZPIY(x_reg,4);
        case 0x8E: STORE_A(x_reg,4);
        case 0x84: STORE_ZP(y_reg,3);
        case 0x94: STORE_ZPIX(y_reg,4);
        case 0x8C: STORE_A(y_reg,4);
        case 0xAA: TRANSFER_REG(accumulator,x_reg,2);
        case 0xA8: TRANSFER_REG(accumulator,y_reg,2);
        case 0xBA: TRANSFER_STACK_FROM(x_reg,2);
        case 0x8A: TRANSFER_REG(x_reg,accumulator,2);
        case 0x9A: TRANSFER_STACK_TO(x_reg,2);
        case 0x98: TRANSFER_REG(y_reg,accumulator,2);
        default:
            break;

        }

    } while (cycle_count > 0);

    return cycles - cycle_count;

}

