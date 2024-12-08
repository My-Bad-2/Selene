#include <klibc/string.h>
#include <lock.h>
#include <log.h>

#include <kernel/arch/x86_64/cpu/exceptions.h>
#include <kernel/arch/x86_64/cpu/idt.h>

#define KERNEL_CODE_SELECTOR (8)

#define TYPE_ATTRIBUTE_PRESENT (1 << 7)
#define TYPE_ATTRIBUTE_DPL(x) (x << 5)

extern uintptr_t isr_table[MAX_IDT_ENTRIES];

struct idt idt = {};
struct mutex idt_mutex = INITIALIZE_MUTEX;

static inline uint8_t get_idt_attribute(uint8_t type, uint8_t dpl) {
  uint8_t ret = TYPE_ATTRIBUTE_PRESENT;
  ret |= TYPE_ATTRIBUTE_DPL(dpl);
  ret |= type;

  return ret;
}

static void set_idt_segment(struct idt_segment* segment, uintptr_t base, uint8_t ist, uint8_t type,
                            uint8_t dpl, uint16_t selector) {
  segment->base_low = base & 0xffff;
  segment->selector = selector;
  segment->ist = ist;
  segment->type_attributes = get_idt_attribute(type, dpl);
  segment->base_mid = (base >> 16) & 0xffff;
  segment->base_high = (base >> 32) & 0xffffffff;
  segment->reserved = 0;
}

/**
 * @details This function sets up the IDT entries based on the ISR table, configures their
 * attributes, and loads the IDT using the assembly-defined `load_idt` function.
 */
void idt_initialize(void) {
  mutex_lock(&idt_mutex);

  uint8_t type = IDT_INTERRUPT_GATE;

  memset(&idt, 0, sizeof(struct idt));

  for (int i = 0; i < MAX_IDT_ENTRIES; i++) {
    set_idt_segment(&idt.entries[i], isr_table[i], 0, type,
                    (i == EXCEPTION_BREAKPOINT) ? IDT_DPL3 : IDT_DPL0, KERNEL_CODE_SELECTOR);
  }

  struct idt_register idtr = {
      .limit = sizeof(struct idt) - 1,
      .base = (uintptr_t)&idt,
  };

  load_idt(&idtr);

    log_info("Initialized Interrupt Description Table!");
  log_debug("Interrupt Description Table located @ %p", &idt);

  mutex_unlock(&idt_mutex);
}