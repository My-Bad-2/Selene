/**
 * @file arch.h
 * @brief Basic Architecture-related Operations
 */
#ifndef KERNEL_ARCH_H
#define KERNEL_ARCH_H 1

#include <compiler.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/// @brief Inserts a CPU pause instruction.
#define arch_pause() WRAP_MACRO(asm volatile("pause"))

/// @brief Enables hardware interrupts.
#define arch_enable_interrupts() WRAP_MACRO(asm volatile("sti"))

/// @brief Disables hardware interrupts.
#define arch_disable_interrupts() WRAP_MACRO(asm volatile("cli"))

/// @brief Halts the CPU until the next interrupt.
#define arch_hlt() WRAP_MACRO(asm volatile("hlt"))

/// @brief Writes a value to the specified port.
/// @details Automatically selects the correct implementation based on the value
/// type.
#define outp(port, val) _Generic((val), uint8_t: outpb, uint16_t: outpw, uint32_t: outpl)(port, val)

/// @brief Reads a value from the specified port.
/// @details Automatically selects the correct implementation based on the value
/// type.
#define inp(port, val) \
    _Generic((val), uint8_t *: inpb, uint16_t *: inpw, uint32_t *: inpl)(port, val)

/**
 * @brief Halts the system, optionally disabling interrupts beforehand.
 * @param interrupts Set to false to disable interrupts before halting.
 * @note This function does not return.
 */
__NO_RETURN void arch_halt(bool interrupts);

/**
 * @brief Writes an 8-bit value to the specified port.
 * @param port The I/O port to write to.
 * @param val The 8-bit value to write.
 */
void outpb(uint16_t port, uint8_t val);

/**
 * @brief Writes a 16-bit value to the specified port.
 * @param port The I/O port to write to.
 * @param val The 16-bit value to write.
 */
void outpw(uint16_t port, uint16_t val);

/**
 * @brief Writes a 32-bit value to the specified port.
 * @param port The I/O port to write to.
 * @param val The 32-bit value to write.
 */
void outpl(uint16_t port, uint32_t val);

/**
 * @brief Reads an 8-bit value from the specified port.
 * @param port The I/O port to read from.
 * @param val Pointer to store the 8-bit value read.
 */
void inpb(uint16_t port, uint8_t *val);

/**
 * @brief Reads a 16-bit value from the specified port.
 * @param port The I/O port to read from.
 * @param val Pointer to store the 16-bit value read.
 */
void inpw(uint16_t port, uint16_t *val);

/**
 * @brief Reads a 32-bit value from the specified port.
 * @param port The I/O port to read from.
 * @param val Pointer to store the 32-bit value read.
 */
void inpl(uint16_t port, uint32_t *val);

/**
 * @brief Initializes architecture-specific components.
 */
void arch_initialize();

/**
 * @brief Writes a buffer of characters to the output device.
 * @param buffer Pointer to the character buffer to write.
 * @param length Number of characters to write.
 * @return The number of characters written, or 0 if the buffer is `NULL` or the length is 0.
 */
int arch_write(const char *buffer, size_t length);

#endif// KERNEL_ARCH_H