#include <kernel/arch/arch.h>
#include <klibc/stdio.h>
#include <log.h>

void kmain()
{
    arch_initialize();

    log_set_level(DEBUG);
    log_set_quiet(false);

    log_info("Hello, World!");

    arch_halt(true);
}