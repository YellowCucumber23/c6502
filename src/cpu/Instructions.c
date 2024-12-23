#include "Memory.h"
#include "Instructions.h"
#include "Cpu.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

/**************************************************************************************************
 * Global Variables
 *************************************************************************************************/
mem_t* p_mem;
cpu_t* p_cpu;
uint16_t abs_addr;
uint16_t rel_addr;
uint8_t fetched_instr;
uint32_t instr_cycles;

instruction_t instruction_map[16][16] = 
{   /*0X00 - 0X0F*/
    {DEF_INSTR("BRK", &BRK, &IMM, 7), DEF_INSTR("ORA", &ORA, &IND, 6),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("ORA", &ORA, &ZPG, 3),
     DEF_INSTR("ASL", &ASL, &ZPG, 5), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("PHP", &PHP, &IMP, 3), DEF_INSTR("ORA", &ORA, &IMM, 2),
     DEF_INSTR("ASL", &ASL, &IMP, 2), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("ORA", &ORA, &ABS, 4),
     DEF_INSTR("ASL", &ASL, &ABS, 6), DEF_INSTR("NUL", &NUL, &IMM, 0)},
    /*0X10 - 0X1F*/
    {DEF_INSTR("BPL", &BPL, &REL, 2), DEF_INSTR("ORA", &ORA, &IZY, 5),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("ORA", &ORA, &ZPX, 4),
     DEF_INSTR("ASL", &ASL, &ZPX, 4), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("CLC", &CLC, &IMP, 2), DEF_INSTR("ORA", &ORA, &ABY, 4),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("ORA", &ORA, &ABX, 4),
     DEF_INSTR("ASL", &ASL, &ABX, 7), DEF_INSTR("NUL", &NUL, &IMM, 0),},
    /*0X20 - 0X2F*/
    {DEF_INSTR("JSR", &JSR, &ABS, 6), DEF_INSTR("AND", &AND, &IZX, 0),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("BIT", &BIT, &ZPG, 3), DEF_INSTR("AND", &AND, &ZPG, 3),
     DEF_INSTR("ROL", &ROL, &ZPG, 5), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("PLP", &PLP, &IMP, 4), DEF_INSTR("AND", &AND, &IMM, 2),
     DEF_INSTR("ROL", &ROL, &IMP, 2), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("BIT", &BIT, &ABS, 4), DEF_INSTR("AND", &AND, &ABS, 4),
     DEF_INSTR("ROL", &ROL, &ABS, 6), DEF_INSTR("NUL", &NUL, &IMM, 0),},
    /*0X30 - 0X3F*/
    {DEF_INSTR("BMI", &BMI, &REL, 2), DEF_INSTR("AND", &AND, &IZY, 5),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("AND", &AND, &ZPX, 4),
     DEF_INSTR("ROL", &ROL, &ZPX, 6), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("SEC", &SEC, &IMP, 2), DEF_INSTR("AND", &AND, &ABY, 4),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("AND", &AND, &ABX, 4),
     DEF_INSTR("ROL", &ROL, &ABX, 7), DEF_INSTR("NUL", &NUL, &IMM, 0),},
    /*0X40 - 0X4F*/
    {DEF_INSTR("RTI", &RTI, &IMP, 6), DEF_INSTR("EOR", &EOR, &IZX, 6),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("EOR", &EOR, &ZPG, 3),
     DEF_INSTR("LSR", &LSR, &ZPG, 5), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("PHA", &PHA, &IMP, 3), DEF_INSTR("EOR", &EOR, &IMM, 2),
     DEF_INSTR("LSR", &LSR, &IMP, 2), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("JMP", &JMP, &ABS, 3), DEF_INSTR("EOR", &EOR, &ABS, 4),
     DEF_INSTR("LSR", &LSR, &ABS, 6), DEF_INSTR("NUL", &NUL, &IMM, 0),},
    /*0X50 - 0X5F*/
    {DEF_INSTR("BVC", &BVC, &REL, 2), DEF_INSTR("EOR", &EOR, &IZY, 5),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("EOR", &EOR, &ZPX, 4),
     DEF_INSTR("LSR", &LSR, &ZPX, 6), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("CLI", &CLI, &IMP, 2), DEF_INSTR("EOR", &EOR, &ABY, 4),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("EOR", &EOR, &ABX, 4),
     DEF_INSTR("LSR", &LSR, &ABX, 7), DEF_INSTR("NUL", &NUL, &IMM, 0),},
    /*0X60 - 0X6F*/
    {DEF_INSTR("RTS", &RTS, &IMP, 6), DEF_INSTR("ADC", &ADC, &IND, 6),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("ADC", &ADC, &ZPG, 3),
     DEF_INSTR("ROR", &ROR, &ZPG, 5), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("PLA", &PLA, &IMP, 4), DEF_INSTR("ADC", &ADC, &IMM, 2),
     DEF_INSTR("ROR", &ROR, &IMP, 2), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("JMP", &JMP, &IND, 5), DEF_INSTR("ADC", &ADC, &ABS, 4),
     DEF_INSTR("ROR", &ROR, &ABS, 6), DEF_INSTR("NUL", &NUL, &IMM, 0),},
    /*0X70 - 0X7F*/
    {DEF_INSTR("BVS", &BVS, &REL, 2), DEF_INSTR("ADC", &ADC, &IZY, 5),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("ADC", &ADC, &ZPX, 4),
     DEF_INSTR("ROR", &ROR, &ZPX, 6), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("SEI", &SEI, &IMP, 2), DEF_INSTR("ADC", &ADC, &ABY, 4),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("ADC", &ADC, &ABX, 4),
     DEF_INSTR("ROR", &ROR, &ABX, 7), DEF_INSTR("NUL", &NUL, &IMM, 0),},
    /*0X80 - 0X8F*/
    {DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("STA", &STA, &IZX, 6),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("STY", &STY, &ZPG, 3), DEF_INSTR("STA", &STA, &ZPG, 3),
     DEF_INSTR("STX", &STX, &ZPG, 3), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("DEY", &DEY, &IMP, 2), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("TXA", &TXA, &IMP, 2), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("STY", &STY, &ABS, 4), DEF_INSTR("STA", &STA, &ABS, 0),
     DEF_INSTR("STX", &STX, &ABS, 4), DEF_INSTR("NUL", &NUL, &IMM, 0),},
    /*0X90 - 0X9F*/
    {DEF_INSTR("BCC", &BCC, &REL, 2), DEF_INSTR("STA", &STA, &IZY, 6),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("STY", &STY, &ZPX, 4), DEF_INSTR("STA", &STA, &ZPX, 4),
     DEF_INSTR("STX", &STX, &ZPY, 4), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("TYA", &TYA, &IMP, 2), DEF_INSTR("STA", &STA, &ABY, 5),
     DEF_INSTR("TXS", &TXS, &IMP, 2), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("STA", &STA, &ABX, 5),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("NUL", &NUL, &IMM, 0),},
    /*0XA0 - 0XAF*/
    {DEF_INSTR("LDY", &LDY, &IMM, 2), DEF_INSTR("LDA", &LDA, &IZX, 6),
     DEF_INSTR("LDX", &LDX, &IMM, 3), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("LDY", &LDY, &ZPG, 3), DEF_INSTR("LDA", &LDA, &ZPG, 3),
     DEF_INSTR("LDX", &LDX, &ZPG, 3), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("TAY", &TAY, &IMP, 2), DEF_INSTR("LDA", &LDA, &IMM, 2),
     DEF_INSTR("TAX", &TAX, &IMP, 2), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("LDY", &LDY, &ABS, 4), DEF_INSTR("LDA", &LDA, &ABS, 4),
     DEF_INSTR("LDX", &LDA, &ABS, 4), DEF_INSTR("NUL", &NUL, &IMM, 0),},
    /*0XB0 - 0XBF*/
    {DEF_INSTR("BCS", &BCS, &REL, 2), DEF_INSTR("LDA", &LDA, &IZY, 5),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("LDY", &LDY, &ZPX, 4), DEF_INSTR("LDA", &LDA, &ZPX, 4),
     DEF_INSTR("LDX", &LDX, &ZPY, 4), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("CLV", &CLV, &IMP, 2), DEF_INSTR("LDA", &LDA, &ABY, 4),
     DEF_INSTR("TSX", &TSX, &IMP, 2), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("LDY", &LDY, &ABX, 4), DEF_INSTR("LDA", &LDA, &ABX, 4),
     DEF_INSTR("LDX", &LDY, &ABY, 4), DEF_INSTR("NUL", &NUL, &IMM, 0),},
    /*0XC0 - 0XCF*/
    {DEF_INSTR("CPY", &NUL, &IMM, 0), DEF_INSTR("CMP", &NUL, &IMM, 0),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("CPY", &NUL, &IMM, 0), DEF_INSTR("CMP", &NUL, &IMM, 0),
     DEF_INSTR("DEC", &NUL, &IMM, 0), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("INY", &NUL, &IMM, 0), DEF_INSTR("CMP", &NUL, &IMM, 0),
     DEF_INSTR("DEX", &NUL, &IMM, 0), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("CPY", &NUL, &IMM, 0), DEF_INSTR("CMP", &NUL, &IMM, 0),
     DEF_INSTR("DEC", &NUL, &IMM, 0), DEF_INSTR("NUL", &NUL, &IMM, 0),},
    /*0XD0 - 0XDF*/
    {DEF_INSTR("BNE", &BNE, &REL, 2), DEF_INSTR("CMP", &CMP, &IZY, 5),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("CMP", &CMP, &ZPX, 4),
     DEF_INSTR("DEC", &DEC, &ZPX, 6), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("CLD", &CLD, &IMP, 2), DEF_INSTR("CMP", &CMP, &ABY, 4),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("CMP", &CMP, &ABX, 4),
     DEF_INSTR("DEC", &DEC, &ABX, 7), DEF_INSTR("NUL", &NUL, &IMM, 0),},
    /*0XE0 - 0XEF*/
    {DEF_INSTR("CPX", &CPX, &IMM, 2), DEF_INSTR("SBC", &SBC, &IZX, 6),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("CPX", &CPX, &ZPG, 3), DEF_INSTR("SBC", &SBC, &ZPG, 3),
     DEF_INSTR("INC", &INC, &ZPG, 5), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("INX", &INX, &IMP, 2), DEF_INSTR("SBC", &SBC, &IMM, 2),
     DEF_INSTR("NOP", &NOP, &IMP, 2), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("CPX", &CPX, &ABS, 4), DEF_INSTR("SBC", &SBC, &ABX, 4),
     DEF_INSTR("INC", &INC, &ABS, 6), DEF_INSTR("NUL", &NUL, &IMM, 0),},
    /*0XF0 - 0XFF*/
    {DEF_INSTR("BEQ", &BEQ, &REL, 2), DEF_INSTR("SBC", &SBC, &IZY, 5),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("SBC", &SBC, &ZPX, 4),
     DEF_INSTR("INC", &INC, &ZPX, 6), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("SED", &SED, &IMP, 2), DEF_INSTR("SBC", &SBC, &ABY, 4),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("NUL", &NUL, &IMM, 0),
     DEF_INSTR("NUL", &NUL, &IMM, 0), DEF_INSTR("SBC", &SBC, &ABX, 4),
     DEF_INSTR("INC", &INC, &ABX, 7), DEF_INSTR("NUL", &NUL, &IMM, 0),}
};
/**************************************************************************************************
 * Helper Function Definitions
 *************************************************************************************************/

void instruction_init(void)
{
    p_mem = mem_get_ptr();
    p_cpu = cpu_get_ptr();
}

void instruction_exec(uint8_t fetched, uint32_t* cycles)
{
    instruction_t instr = instruction_map[(fetched & 0xF0) >> 4][fetched & 0x0F];
    instr_cycles = *cycles;
    printf("%s\n",instr.opname);

}

/**************************************************************************************************
 * Address Modes Definitions
 *************************************************************************************************/
/**
 * @brief Set cpu mode to implicit
 */
void IMP(void)
{
    fetched_instr = p_cpu->reg_a;
}

/**
 * Data is taken from the byte following the opcode
 */
void IMM(void)
{
    abs_addr = p_cpu->pc++;
}

/**
 * An 8-bit address is provided within the zero page. This is like an absolute address, 
 * but since the argument is only one byte, the CPU does not have to spend an additional 
 * cycle to fetch high byte.
 */
void ZPG(void)
{
    
}

/**
 * @brief Set cpu mode to zero page, x
 */
void ZPX(void)
{

}

/**
 * @brief Set cpu mode to zero page, y
 */
void ZPY(void)
{

}

/**
 * @brief Set cpu mode to relative
 */
void REL(void)
{

}

/**
 * @brief Set cpu mode to absolute
 */
void ABS(void)
{

}

/**
 * @brief Set cpu mode to absolute, x
 */
void ABX(void)
{

}

/**
 * @brief Set cpu mode to implicit, y
 */
void ABY(void)
{

}

/**
 * @brief Set cpu mode to indirect
 */
void IND(void)
{

}

/**
 * @brief Set cpu mode to indirect, x
 */
void IZX(void)
{

}

/**
 * @brief Set cpu mode to indirect, y
 */
void IZY(void)
{

}


/**************************************************************************************************
 * OPCode Definitions
 *************************************************************************************************/

uint8_t NUL(void){return 0;}
uint8_t ADC(void){return 0;}
uint8_t AND(void){return 0;}
uint8_t ASL(void){return 0;}
uint8_t BCC(void){return 0;}
uint8_t BCS(void){return 0;}
uint8_t BEQ(void){return 0;}
uint8_t BIT(void){return 0;}
uint8_t BMI(void){return 0;}
uint8_t BNE(void){return 0;}
uint8_t BPL(void){return 0;}
uint8_t BRK(void){return 0;}
uint8_t BVC(void){return 0;}
uint8_t BVS(void){return 0;}
uint8_t CLC(void){return 0;}

uint8_t CLD(void){return 0;}
uint8_t CLI(void){return 0;}
uint8_t CLV(void){return 0;}
uint8_t CMP(void){return 0;}
uint8_t CPX(void){return 0;}
uint8_t CPY(void){return 0;}
uint8_t DEC(void){return 0;}
uint8_t DEX(void){return 0;}
uint8_t DEY(void){return 0;}
uint8_t EOR(void){return 0;}
uint8_t INC(void){return 0;}
uint8_t INX(void){return 0;}
uint8_t INY(void){return 0;}
uint8_t JMP(void){return 0;}

uint8_t JSR(void){return 0;}
uint8_t LDA(void){return 0;}
uint8_t LDX(void){return 0;}
uint8_t LDY(void){return 0;}
uint8_t LSR(void){return 0;}
uint8_t NOP(void){return 0;}
uint8_t ORA(void){return 0;}
uint8_t PHA(void){return 0;}
uint8_t PHP(void){return 0;}
uint8_t PLA(void){return 0;}
uint8_t PLP(void){return 0;}
uint8_t ROL(void){return 0;}
uint8_t ROR(void){return 0;}
uint8_t RTI(void){return 0;}

uint8_t RTS(void){return 0;}
uint8_t SBC(void){return 0;}
uint8_t SEC(void){return 0;}
uint8_t SED(void){return 0;}
uint8_t SEI(void){return 0;}
uint8_t STA(void){return 0;}
uint8_t STX(void){return 0;}
uint8_t STY(void){return 0;}
uint8_t TAX(void){return 0;}
uint8_t TAY(void){return 0;}
uint8_t TSX(void){return 0;}
uint8_t TXA(void){return 0;}
uint8_t TXS(void){return 0;}
uint8_t TYA(void){return 0;}