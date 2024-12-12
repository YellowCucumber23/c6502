#include "Memory.h"
#include "Cpu.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

/**************************************************************************************************
 * Global Variables
 *************************************************************************************************/
mem_t* p_mem = NULL;

/**************************************************************************************************
 * Function Definitions
 *************************************************************************************************/

void cpu_init(void)
{
    p_mem = mem_get_ptr();
}

uint8_t cpu_read_mem(uint16_t addr)
{
    if (addr >= 0x0000 && addr <= 0x00FF){ return p_mem->zero_page[addr]; } 
    else if (addr >= 0x0100 && addr <= 0x01FF){ return p_mem->stack[addr - 0x100]; }
    else if (addr >= 0x0200 && addr <= 0x7FFF){ return p_mem->ram[addr - 0x200]; }
    else if (addr >= 0x8000 && addr <= 0xFFF9){ return p_mem->rom[addr - 0x8000]; }
    else if (addr >= 0xFFFA && addr <= 0xFFFB){ return p_mem->nmi_vector[addr - 0xFFFA]; }
    else if (addr >= 0xFFFC && addr <= 0xFFFD){ return p_mem->reset_vector[addr - 0xFFFC]; }
    else{ return p_mem->irq_brk_vector[addr - 0xFFFE]; }
}

void cpu_write_mem(uint16_t addr, uint8_t val)
{

    if (addr >= 0x0000 && addr <= 0x00FF){ p_mem->zero_page[addr] = val; } 
    else if (addr >= 0x0100 && addr <= 0x01FF){ p_mem->stack[addr - 0x100] = val; }
    else if (addr >= 0x0200 && addr <= 0x7FFF){ p_mem->ram[addr - 0x200] = val; }
    else if (addr >= 0x8000 && addr <= 0xFFF9){ p_mem->rom[addr - 0x8000] = val; }
    else if (addr >= 0xFFFA && addr <= 0xFFFB){ p_mem->nmi_vector[addr - 0xFFFA] = val; }
    else if (addr >= 0xFFFC && addr <= 0xFFFD){ p_mem->reset_vector[addr - 0xFFFC] = val; }
    else{ p_mem->irq_brk_vector[addr - 0xFFFE] = val; }
}