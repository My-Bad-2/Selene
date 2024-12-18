#include <klibc/stdio.h>
#include <log.h>

#include <kernel/arch/arch.h>
#include <kernel/memory/physical.h>

void kmain(void) {
  log_set_level(LOG_TRACE);
  log_set_quiet(false);

  arch_initialize();
  physical_initialize();

  log_info("Hello, World!");

  arch_halt(true);
}