#include "Memory.h"
#include "Cpu.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

/**************************************************************************************************
 * Global Variables
 *************************************************************************************************/

/**************************************************************************************************
 * Function Definitions
 *************************************************************************************************/
uint8_t cpu_read_mem(uint16_t addr)
{
    mem_t* mem = mem_get_ptr();

    if (addr >= 0x0000 && addr <= 0x00FF){ return mem->zero_page[addr]; } 
    else if (addr >= 0x0100 && addr <= 0x01FF){ return mem->stack[addr - 0x100]; }
    else if (addr >= 0x0200 && addr <= 0x7FFF){ return mem->ram[addr - 0x200]; }
    else if (addr >= 0x8000 && addr <= 0xFFF9){ return mem->rom[addr - 0x8000]; }
    else if (addr >= 0xFFFA && addr <= 0xFFFB){ return mem->nmi_vector[addr - 0xFFFA]; }
    else if (addr >= 0xFFFC && addr <= 0xFFFD){ return mem->reset_vector[addr - 0xFFFC]; }
    else{ return mem->irq_brk_vector[addr - 0xFFFE]; }
}

void cpu_write_mem(uint16_t addr, uint8_t val)
{
    mem_t* mem = mem_get_ptr();

    if (addr >= 0x0000 && addr <= 0x00FF){ mem->zero_page[addr] = val; } 
    else if (addr >= 0x0100 && addr <= 0x01FF){ mem->stack[addr - 0x100] = val; }
    else if (addr >= 0x0200 && addr <= 0x7FFF){ mem->ram[addr - 0x200] = val; }
    else if (addr >= 0x8000 && addr <= 0xFFF9){ mem->rom[addr - 0x8000] = val; }
    else if (addr >= 0xFFFA && addr <= 0xFFFB){ mem->nmi_vector[addr - 0xFFFA] = val; }
    else if (addr >= 0xFFFC && addr <= 0xFFFD){ mem->reset_vector[addr - 0xFFFC] = val; }
    else{ mem->irq_brk_vector[addr - 0xFFFE] = val; }
}