#include <kernel/arch/x86_64/cpu/gdt.hpp>
#include <kernel/arch/x86_64/cpu/idt.hpp>

#include <kernel/arch/x86_64/arch.hpp>
#include <kernel/arch/x86_64/drivers/uart.hpp>

#include <string_view>

namespace {
UartDriver uart_driver(PORT1);
Gdt gdt;
Idt idt;
}

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
 * @details Sets up the primary UART (COM1) for serial communication, initializes the Global
 * Descriptor Table, Interrupt Descriptor Table. This function is typically called during system
 * boot to prepare low-level architecture dependencies.
 */
void arch_initialize() {
  arch_disable_interrupts();

  uart_driver.initialize();
  gdt.initialize();
  idt.initialize();

  arch_enable_interrupts();
}

/**
 * @details This function sends each character in the buffer to the primary UART
 * (COM1) using the `uart_putc` function.
 */
int arch_write(const char* buffer, int length) {
  std::string_view str(buffer, length);

  for (const auto& c : str) {
    uart_driver.uart_putc(c);
  }

  return static_cast<int>(length);
}