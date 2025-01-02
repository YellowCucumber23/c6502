/*
 * Filename: Instructions.h
 * Description: 6502 Instruction header file
 * Author: Adrian Ho
 */

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

/**************************************************************************************************
 * Include Files 
 *************************************************************************************************/
#include <stdint.h>
#include <stddef.h>

/**************************************************************************************************
 * Definitions
 *************************************************************************************************/
/* Macro to populate an instruction struct */
#define DEF_INSTR(NAME, OP_FUNC, MODE_FUNC, CYCLES) \
    (instruction_t){ .opname = NAME, .op = OP_FUNC, .mode = MODE_FUNC, .clock_cycle = CYCLES }

/* Bit shift values for the resepective flags */
/* https://www.nesdev.org/wiki/Status_flags#The_B_flag */
#define C   0       /* Carry flag */
#define Z   1       /* Zero flag */
#define I   2       /* Interrupt disable */
#define D   3       /* Decimal mode */
#define B   4       /* B flag */
#define V   6       /* Overflow */
#define N   7       /* Negative */

/**************************************************************************************************
 * Type Definitions
 *************************************************************************************************/
typedef struct instruction_t {
    char* opname;               /* Operation name */
    uint8_t (*op)(void);        /* Pointer to operation function */
    void (*mode)(void);         /* Pointer to addressing mode function */
    uint8_t clock_cycle;        /* Number of clock cycles */
} instruction_t;

extern instruction_t instruction_map[16][16];

/**************************************************************************************************
 * Helper Function Prototypes
 *************************************************************************************************/

/**
 * @brief Initialize the memoery and cpu pointers for the instructions
 */
void instruction_init(void);

/**
 * @brief Execute the fetched instruction from the cpu
 * @param[in] fetched The instruction fetched
 * @param[in] cycles The current number of clock cycles
 */
void instruction_exec(uint8_t fetched, uint32_t* cycles);

/**************************************************************************************************
 * Addressing Mode Function Prototypes
 *************************************************************************************************/
// https://wiki.cdot.senecapolytechnic.ca/wiki/6502_Addressing_Modes#X.2C_Indirect 
/**
 * @brief Set cpu mode to implicit
 * @return the number of additional clock cycles
 */
uint8_t IMP(void);

/**
 * @brief Set cpu mode to immediate
 * @return the number of additional clock cycles
 */
uint8_t IMM(void);

/**
 * @brief Set cpu mode to zero page
 * @return the number of additional clock cycles
 */
uint8_t ZPG(void);

/**
 * @brief Set cpu mode to zero page, x
 * @return the number of additional clock cycles
 */
uint8_t ZPX(void);

/**
 * @brief Set cpu mode to zero page, y
 * @return the number of additional clock cycles
 */
uint8_t ZPY(void);

/**
 * @brief Set cpu mode to relative
 * @return the number of additional clock cycles
 */
uint8_t REL(void);

/**
 * @brief Set cpu mode to absolute
 * @return the number of additional clock cycles
 */
uint8_t ABS(void);

/**
 * @brief Set cpu mode to absolute, x
 * @return the number of additional clock cycles
 */
uint8_t ABX(void);

/**
 * @brief Set cpu mode to implicit, y
 * @return the number of additional clock cycles
 */
uint8_t ABY(void);

/**
 * @brief Set cpu mode to indirect
 * @return the number of additional clock cycles
 */
uint8_t IND(void);

/**
 * @brief Set cpu mode to indirect, x
 * @return the number of additional clock cycles
 */
uint8_t IZX(void);

/**
 * @brief Set cpu mode to indirect, y
 * @return the number of additional clock cycles
 */
uint8_t IZY(void);

/**************************************************************************************************
 * OPCode Function Prototypes
 *************************************************************************************************/

uint8_t NUL(void);
uint8_t ADC(void);
uint8_t AND(void);
uint8_t ASL(void);
uint8_t BCC(void);
uint8_t BCS(void);
uint8_t BEQ(void);
uint8_t BIT(void);
uint8_t BMI(void);
uint8_t BNE(void);
uint8_t BPL(void);
uint8_t BRK(void);
uint8_t BVC(void);
uint8_t BVS(void);
uint8_t CLC(void);

uint8_t CLD(void);
uint8_t CLI(void);
uint8_t CLV(void);
uint8_t CMP(void);
uint8_t CPX(void);
uint8_t CPY(void);
uint8_t DEC(void);
uint8_t DEX(void);
uint8_t DEY(void);
uint8_t EOR(void);
uint8_t INC(void);
uint8_t INX(void);
uint8_t INY(void);
uint8_t JMP(void);

uint8_t JSR(void);
uint8_t LDA(void);
uint8_t LDX(void);
uint8_t LDY(void);
uint8_t LSR(void);
uint8_t NOP(void);
uint8_t ORA(void);
uint8_t PHA(void);
uint8_t PHP(void);
uint8_t PLA(void);
uint8_t PLP(void);
uint8_t ROL(void);
uint8_t ROR(void);
uint8_t RTI(void);

uint8_t RTS(void);
uint8_t SBC(void);
uint8_t SEC(void);
uint8_t SED(void);
uint8_t SEI(void);
uint8_t STA(void);
uint8_t STX(void);
uint8_t STY(void);
uint8_t TAX(void);
uint8_t TAY(void);
uint8_t TSX(void);
uint8_t TXA(void);
uint8_t TXS(void);
uint8_t TYA(void);

#endif