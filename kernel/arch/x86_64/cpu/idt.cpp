#include <klibc/string.h>
#include <log.hpp>

#include <kernel/arch/x86_64/cpu/exceptions.hpp>
#include <kernel/arch/x86_64/cpu/idt.hpp>

#define KERNEL_CODE_SELECTOR (8)

#define TYPE_ATTRIBUTE_PRESENT (1 << 7)
#define TYPE_ATTRIBUTE_DPL(x) (x << 5)

extern "C" uintptr_t isr_table[MAX_IDT_ENTRIES];

namespace {
constexpr uint8_t get_idt_attribute(uint8_t type, uint8_t dpl) {
  uint8_t ret = TYPE_ATTRIBUTE_PRESENT;
  ret |= TYPE_ATTRIBUTE_DPL(dpl);
  ret |= type;

  return ret;
}
}  // namespace

void IdtSegment::set(uintptr_t base, uint8_t ist, uint8_t type, uint8_t dpl, uint16_t selector) {
  this->base_low = base & 0xffff;
  this->selector = selector;
  this->ist = ist;
  this->type_attributes = get_idt_attribute(type, dpl);
  this->base_mid = (base >> 16) & 0xffff;
  this->base_high = (base >> 32) & 0xffffffff;
  this->reserved = 0;
}

/**
 * @details This function sets up the IDT entries based on the ISR table, configures their
 * attributes, and loads the IDT using the assembly-defined `load_idt` function.
 */
void Idt::initialize() {
  uint8_t type = IDT_INTERRUPT_GATE;

  memset(&this->m_table, 0, sizeof(IdtTable));

  for (int i = 0; i < MAX_IDT_ENTRIES; i++) {
    this->m_table.entries[i].set(isr_table[i], 0, type,
                                 (i == EXCEPTION_BREAKPOINT) ? IDT_DPL3 : IDT_DPL0,
                                 KERNEL_CODE_SELECTOR);
  }

  this->load();

  log_info("Initialized Interrupt Description Table!");
  log_debug("Interrupt Description Table located @ %p", &this->m_table);
}

void Idt::load() {
  IdtRegister idtr = {
      .limit = sizeof(IdtTable) - 1,
      .base = reinterpret_cast<uintptr_t>(&this->m_table),
  };

  asm volatile("lidt %0" ::"m"(idtr) : "memory");
}