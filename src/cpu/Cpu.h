/*
 * Filename: Cpu.h
 * Description: 6502 cpu header file
 * Author: Adrian Ho
 */

#ifndef CPU_H
#define CPU_H

/**************************************************************************************************
 * Include Files 
 *************************************************************************************************/
#include <stdint.h>
#include <stddef.h>

/**************************************************************************************************
 * Definitions
 *************************************************************************************************/


/**************************************************************************************************
 * Type Definitions
 *************************************************************************************************/

typedef struct cpu {
    uint16_t pc;        /* Program counter */
    uint16_t sp;         /* Stack pointer (start at 0x1FF and grows down to 0x100) */
    uint8_t reg_a;      /* Accumulator register */
    uint8_t reg_x;      /* Index regiter x */
    uint8_t reg_y;      /* Index register y */
    uint8_t status;     /* Status register for the flags */
} cpu_t;

/**************************************************************************************************
 * Function Prototypes
 *************************************************************************************************/

/**
 * @brief Reset all the registers and status bits in the cpu
 */
void cpu_reset(void);

/**
 * @brief Initialize the memory pointer and registers
 */
void cpu_init(void);

/**
 * @brief Return a pointer to the cpu struct
 * @return cpu_t* The cpu struct
 */
cpu_t* cpu_get_ptr(void);

/**
 * @brief Read from an address in memory
 * @param[in] addr The address to read from
 * @return The value read from addr
 */
uint8_t cpu_read_mem(uint16_t addr);

/**
 * @brief Write a value into memory
 * @param[in] addr The address to read from
 * @param[in] val The value to write
 */
void cpu_write_mem(uint16_t addr, uint8_t val);

/**
 * @brief Print all the registers from the cpu struct
 */
void cpu_print(void);

/**
 * @brief Execute a cpu instruction
 */
void cpu_exec(void);
#endif