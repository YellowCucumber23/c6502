#include <ncurses.h>
#include <stdlib.h>
#include "Memory.h"
#include "Cpu.h"
#include "Instructions.h"

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

    cpu_init();
    cpu_print();

    uint32_t cycles = 0;
    instruction_exec(0x00, &cycles);
    return 0;
}