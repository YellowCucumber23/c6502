/*
 * Filename: Memory.h
 * Description: 6502 Memory header file
 * Author: Adrian Ho
 */

#ifndef MEMORY_H
#define MEMORY_H

/**************************************************************************************************
 * Include Files 
 *************************************************************************************************/
#include <stdint.h>
#include <stddef.h>

/**************************************************************************************************
 * Definitions
 *************************************************************************************************/
#define TOTAL_MEM 0x10000  /* Total 64 KB memory */

#define MEMDEMP_SUCCESS 0
#define MEMDUMP_FAIL    1

/**************************************************************************************************
 * Type Definitions
 *************************************************************************************************/

/* Memory Layout for reading the 6502 binary */
typedef struct mem {
    uint8_t zero_page[0x100];        /* 0x0000 – 0x00FF used for single byte addressing */
    uint8_t stack[0x100];            /* 0x0100 – 0x01FF stack for subroutine calls and interrupts */
    uint8_t ram[0x8000 - 0x0200];    /* 0x0200 – 0x7FFF: General RAM */
    uint8_t rom[0xFFF9 - 0x8000];    /* 0x8000 – 0xFFF9: ROM */

    /* Interrupt Vectors */
    uint8_t nmi_vector[2];           /* 0xFFFA – 0xFFFB: NMI Vector */
    uint8_t reset_vector[2];         /* 0xFFFC – 0xFFFD: Reset Vector */
    uint8_t irq_brk_vector[2];       /* 0xFFFE – 0xFFFF: IRQ/BRK Vector */
} mem_t;

/**************************************************************************************************
 * Function Prototypes
 *************************************************************************************************/

/**
 * @brief Initialize the memory struct from the 6502 bin file
 * @param[in] filename The path to the 6502 executable
 */
void mem_init(char* filename);

/**
 * @brief Dump memory in a file called memdump.bin
 * @return int 1 if failure and 0 if success
 */
int mem_dump(void);

/**
 * @brief Return a point to the memory struct
 * @return mem_t* The memory pointer
 */
mem_t* mem_get_ptr(void);

/**
 * @brief Load the binary into ROM
 * @param[in] filename the path/filename of the binary
 */
void mem_load_program(char* filename);

#endif