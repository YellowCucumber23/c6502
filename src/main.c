#include <ncurses.h>
#include <stdlib.h>
#include "Memory.h"
#include "Cpu.h"

int main(int argc, char* argv[])
{
    switch (argc) {
        case 1:
            mem_init("example.bin");
            break;

        case 2:
            mem_init(argv[1]);
            break;

        default:
            fprintf(stderr, "[FAILED] Too many arguments.\n");
            exit(1);
    }

    mem_t *mem = mem_get_ptr();

    for(int i = 0; i < 256; ++i)
    {
        if (i % 16 == 0){printf("\n");}
        mem->ram[i] = (uint8_t) rand();
        printf("%02x ", mem->ram[i]);
    }


    uint16_t addr = 0x020D;
    printf("\nReading from address %02x: %02x\n", addr, cpu_read_mem(addr));

    cpu_write_mem(addr, 0x23);
    printf("Read: %02x\n", cpu_read_mem(addr));

    for(int i = 0; i < 256; ++i)
    {
        if (i % 16 == 0){printf("\n");}
        printf("%02x ", mem->ram[i]);
    }
    return 0;
}