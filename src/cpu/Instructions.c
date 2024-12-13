#include "Memory.h"
#include "Instructions.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

/**************************************************************************************************
 * Global Variables
 *************************************************************************************************/
mem_t mem;

instruction_t instruction_map[16][16] = 
{   /*0X00 - 0X0F*/
    {DEF_INSTR("BRK", &BRK, &IMM, 7), DEF_INSTR("ORA", &ORA, &INX, 6),
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
    {DEF_INSTR("RTS", &RTS, &IMP, 6), DEF_INSTR("ADC", &ADC, &INX, 6),
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
 * Function Definitions
 *************************************************************************************************/