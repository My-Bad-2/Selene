#include <kernel/arch/arch.h>

void kmain()
{
    arch_initialize();
    arch_halt(true);
}