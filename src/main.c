#include <ncurses.h>
#include <stdlib.h>
#include "Memory.h"

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

    for(int i = 0; i < 50; ++i)
    {
        if (i % 10 == 0){printf("\n");}
        printf("%02x ", mem->rom[i]);
    }

    return 0;
}