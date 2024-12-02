#include <kernel/arch/arch.h>
#include <klibc/stdio.h>

void kmain()
{
    arch_initialize();
    fprintf(stderr, "Hello, World!\n");
    arch_halt(true);
}