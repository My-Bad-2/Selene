#include <kernel/arch/x86_64/arch.h>
#include <kernel/arch/x86_64/cpu/gdt.h>
#include <kernel/arch/x86_64/cpu/idt.h>
#include <kernel/arch/x86_64/drivers/uart.h>

/**
 * @note This function enters an infinite loop, either halting the CPU or
 * disabling interrupts and halting repeatedly.
 */
void arch_halt(bool interrupts) {
  if (interrupts) {
    while (true) {
      arch_hlt();
    }
  } else {
    while (true) {
      arch_disable_interrupts();
      arch_hlt();
    }
  }
}

/**
 * @details Uses the `outb` assembly instruction to write the value.
 */
void outpb(uint16_t port, uint8_t val) { asm volatile("outb %0, %1" ::"a"(val), "Nd"(port)); }

/**
 * @details Uses the `outw` assembly instruction to write the value.
 */
void outpw(uint16_t port, uint16_t val) { asm volatile("outw %0, %1" ::"a"(val), "Nd"(port)); }

/**
 * @details Uses the `outl` assembly instruction to write the value.
 */
void outpl(uint16_t port, uint32_t val) { asm volatile("outl %0, %1" ::"a"(val), "Nd"(port)); }

/**
 * @details Uses the `inb` assembly instruction to read the value.
 */
void inpb(uint16_t port, uint8_t *val) { asm volatile("inb %1, %0" : "=a"(*val) : "Nd"(port)); }

/**
 * @details Uses the `inw` assembly instruction to read the value.
 */
void inpw(uint16_t port, uint16_t *val) { asm volatile("inw %1, %0" : "=a"(*val) : "Nd"(port)); }

/**
 * @details Uses the `inl` assembly instruction to read the value.
 */
void inpl(uint16_t port, uint32_t *val) { asm volatile("inl %1, %0" : "=a"(*val) : "Nd"(port)); }

/**
 * @details Sets up the primary UART (COM1) for serial communication, initializes the Global
 * Descriptor Table, Interrupt Descriptor Table. This function is typically called during system
 * boot to prepare low-level architecture dependencies.
 */
void arch_initialize(void) {
  arch_disable_interrupts();

  uart_initialize(COM_PORT1);
  gdt_initialize();
  idt_initialize();

  arch_enable_interrupts();
}

/**
 * @details This function sends each character in the buffer to the primary UART
 * (COM1) using the `uart_putc` function.
 */
int arch_write(const char *buffer, size_t length) {
  if (length == 0) {
    return 0;
  }

  if (buffer == nullptr) {
    return 0;
  }

  for (size_t i = 0; i < length; i++) {
    uart_putc(buffer[i], COM_PORT1);
  }

  return (int)length;
}