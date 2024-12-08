/**
 * @file
 * @brief Interrupt Descriptor Table (IDT) management.
 *
 * This file contains the definition and initialization of the IDT, which is
 * used to handle hardware and software interrupts in a system. It includes
 * the structure definitions, macros, and functions to configure and load the IDT.
 *
 * Features:
 * - IDT segment and register definitions.
 * - Support for up to 256 interrupt service routines.
 * - Functions to initialize and set up the IDT entries.
 * - Integration with assembly-defined ISR table and `load_idt` function.
 */
#ifndef KERNEL_ARCH_X86_64_CPU_IDT_H
#define KERNEL_ARCH_X86_64_CPU_IDT_H 1

#include <compiler.h>
#include <stdint.h>

/** @brief Defines the type for an interrupt gate descriptor. */
#define IDT_INTERRUPT_GATE 0xe

/** @brief Defines the type for a trap gate descriptor. */
#define IDT_TRAP_GATE 0xf

/** @brief Descriptor Privilege Level 0 (highest privilege level). */
#define IDT_DPL0 (0)

/** @brief Descriptor Privilege Level 3 (lowest privilege level, user mode). */
#define IDT_DPL3 (3)

/** @brief Maximum number of entries in the Interrupt Descriptor Table. */
#define MAX_IDT_ENTRIES 256

/**
 * @brief Structure representing a single IDT entry (IDT segment).
 */
struct idt_segment {
  uint16_t base_low;        ///< Lower 16 bits of the ISR handler's address.
  uint16_t selector;        ///< Code segment selector in GDT or LDT.
  uint8_t ist;              ///< Interrupt Stack Table offset.
  uint8_t type_attributes;  ///< Type and attributes for the segment.
  uint16_t base_mid;        ///< Middle 16 bits of the ISR handler's address.
  uint32_t base_high;       ///< Higher 32 bits of the ISR handler's address.
  uint32_t reserved;        ///< Reserved, must be zero.
} __PACKED;

/**
 * @brief Structure representing the IDT itself.
 */
struct idt {
  struct idt_segment entries[MAX_IDT_ENTRIES];  ///< Array of IDT entries.
};

/**
 * @brief Structure representing the IDTR register.
 */
struct idt_register {
  uint16_t limit;  ///< Limit size of the IDT.
  uintptr_t base;  ///< Base address of the IDT.
} __PACKED;

/**
 * @brief Initializes and loads the Interrupt Descriptor Table (IDT).
 */
void idt_initialize(void);

/**
 * @brief Loads the Interrupt Descriptor Table (IDT) into the CPU.
 *
 * This function loads the IDT into the CPU by setting the IDT register (IDTR). It must be called
 * after the IDT has been initialized to ensure the processor uses the correct table for interrupt
 * handling. The IDT register is loaded using the `lidt` instruction, which requires the address of
 * the IDT and its size.
 *
 * @param idtr A pointer to an `idt_register` structure that contains the address and the size of
 * the IDT.
 */
extern void load_idt(struct idt_register* idtr);

#endif  // KERNEL_ARCH_X86_64_CPU_IDT_H