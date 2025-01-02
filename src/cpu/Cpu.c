#include "Memory.h"
#include "Cpu.h"
#include "Instructions.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

/**************************************************************************************************
 * Global Variables
 *************************************************************************************************/
mem_t* p_mem = NULL;
cpu_t cpu;

uint32_t cycles = 0;

/**************************************************************************************************
 * Function Definitions
 *************************************************************************************************/

void cpu_reset(void)
{

    cpu.pc = cpu_read_mem(0xFFFC) | cpu_read_mem(0xFFFD) << 8;

    cpu.sp = 0x1FF;
    cpu.reg_a = 0;
    cpu.reg_x = 0;
    cpu.reg_y = 0;
    cpu.status = 0;
}

void cpu_init(void)
{
    p_mem = mem_get_ptr();
    cpu_write_mem(0xFFFC, 0x00);
    cpu_write_mem(0xFFFD, 0x80);
    cpu_reset();
    instruction_init();
}

cpu_t* cpu_get_ptr(void)
{
    return &cpu;
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

uint8_t cpu_fetch(uint16_t addr)
{
    uint8_t data = cpu_read_mem(addr);
    if (addr == cpu.pc){cpu.pc++;}
    return data;
}

void cpu_print(void)
{
    printf("CPU State:\n");
    printf("Program Counter (PC): 0x%04X\n", cpu.pc);
    printf("Stack Pointer (SP): 0x%04X\n", cpu.sp);
    printf("Accumulator Register (A): 0x%02X\n", cpu.reg_a);
    printf("Index Register X: 0x%02X\n", cpu.reg_x);
    printf("Index Register Y: 0x%02X\n", cpu.reg_y);
    printf("Status Register: 0x%02X\n", cpu.status);
}

void cpu_exec(void)
{
    uint8_t fetched;
    do {
        printf("(loop) cycles: %d\n", cycles);
        // executing in a take
        if (cycles == 0) {
            fetched = cpu_read_mem(cpu.pc);

            printf("(cpu_exec) fetched: 0x%X\n", fetched);
            instruction_exec(fetched, &cycles);
        }
        cycles--;
    } while (cycles != 0);
}