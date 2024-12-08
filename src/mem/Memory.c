#include "Memory.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

/**************************************************************************************************
 * Global Variables
 *************************************************************************************************/
mem_t mem;

/**************************************************************************************************
 * Function Definitions
 *************************************************************************************************/
void mem_init(char* filename)
{
    /* Clear all the memory */
    memset(mem.zero_page, 0, sizeof(mem.zero_page));
    memset(mem.stack, 0, sizeof(mem.stack));
    memset(mem.ram, 0, sizeof(mem.ram));
    memset(mem.rom, 0, sizeof(mem.rom));
    memset(mem.nmi_vector, 0, sizeof(mem.nmi_vector));
    memset(mem.reset_vector, 0 , sizeof(mem.reset_vector));
    memset(mem.irq_brk_vector, 0, sizeof(mem.irq_brk_vector));


    if (filename == NULL) {
        fprintf(stderr, "No binary program was provided.\n");
        exit(1);
    } else {
        mem_load_program(filename);
    }
}

int mem_dump(void)
{
    /* Empty for now */
    return 1;
}

mem_t* mem_get_ptr(void)
{
    mem_t *temp = &mem;
    return temp;
}

void mem_load_program(char* filename)
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Error while loading provided file.\n");
        exit(1);
    }

    /* Get the file size */
    struct stat st;
    stat(filename,&st);
    int file_size = st.st_size;

    /* Move assembly instructions into ROM */
    int bytes_read = fread(mem.rom, sizeof(uint8_t), sizeof(mem.rom), fp);

    /* If we could not read all the information from the file */
    if (bytes_read != file_size) {
        fprintf(
            stderr,
            "[FAILED] Amount of bytes read doesn't match read file size.\n");
        exit(1);
    }

    fclose(fp);
}
