/**
 * @file
 * @brief This file defines architecture-specific functions and macros for interacting with the CPU
 * and I/O ports.
 *
 * The file includes:
 * - Macros for controlling CPU operations such as pausing, enabling/disabling interrupts, and
 * halting the system.
 * - Functions for reading and writing data to I/O ports of various sizes (8-bit, 16-bit, 32-bit).
 * - A function to halt the system with the option to disable interrupts.
 * - Architecture initialization and output functions.
 *
 * These functions are essential for low-level hardware interaction and control, enabling the kernel
 * to manage CPU states, I/O operations, and system shutdown.
 *
 * @note The macros `arch_pause`, `arch_enable_interrupts`, `arch_disable_interrupts`, and
 * `arch_hlt` correspond to specific assembly instructions used in x86_64 architecture.
 */
#ifndef KERNEL_ARCH_HPP
#define KERNEL_ARCH_HPP 1

#include <compiler.h>

#include <concepts>
#include <cstddef>
#include <cstdint>

/// @brief Inserts a CPU pause instruction.
#define arch_pause() WRAP_MACRO(asm volatile("pause"))

/// @brief Enables hardware interrupts.
#define arch_enable_interrupts() WRAP_MACRO(asm volatile("sti"))

/// @brief Disables hardware interrupts.
#define arch_disable_interrupts() WRAP_MACRO(asm volatile("cli"))

/// @brief Halts the CPU until the next interrupt.
#define arch_hlt() WRAP_MACRO(asm volatile("hlt"))

/// @brief Writes a value to the specified port.
template <std::unsigned_integral T>
  requires(sizeof(T) <= sizeof(uint32_t))
void outp(uint16_t port, T val) {
  if constexpr (std::same_as<T, uint8_t>) {
    asm volatile("outb %0, %1" ::"a"(val), "Nd"(port));
  }

  if constexpr (std::same_as<T, uint16_t>) {
    asm volatile("outw %0, %1" ::"a"(val), "Nd"(port));
  }

  if constexpr (std::same_as<T, uint32_t>) {
    asm volatile("outl %0, %1" ::"a"(val), "Nd"(port));
  }
}

/// @brief Reads a value from the specified port.
template <std::unsigned_integral T>
  requires(sizeof(T) <= sizeof(uint32_t))
T inp(uint16_t port) {
  T ret = 0;

  if constexpr (std::same_as<T, uint8_t>) {
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
  }

  if constexpr (std::same_as<T, uint16_t>) {
    asm volatile("inw %1, %0" : "=a"(ret) : "Nd"(port));
  }

  if constexpr (std::same_as<T, uint32_t>) {
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
  }

  return ret;
}

/**
 * @brief Halts the system, optionally disabling interrupts beforehand.
 * @param interrupts Set to false to disable interrupts before halting.
 * @note This function does not return.
 */
__NO_RETURN void arch_halt(bool interrupts = true);

/**
 * @brief Initializes architecture-specific components.
 */
void arch_initialize();

/**
 * @brief Writes a buffer of characters to the output device.
 * @param buffer Pointer to the character buffer to write.
 * @param length Number of characters to write.
 * @return The number of characters written, or 0 if the buffer is `nullptr` or the length is 0.
 */
int arch_write(const char *buffer, int length);

#endif  // KERNEL_ARCH_HPP