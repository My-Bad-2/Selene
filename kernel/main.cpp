#include <klibc/stdio.h>

#include <kernel/arch/arch.hpp>
#include <kernel/memory/physical.hpp>
#include <log.hpp>

namespace {
PhysicalAllocator phys_allocator;
}

extern "C" void kmain() {
  log::set_level(LOG_TRACE);
  log::set_quiet(false);

  arch_initialize();
  phys_allocator.initialize();

  log_info("Hello, World!");

  arch_halt(true);
}