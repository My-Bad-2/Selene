/**
 * @file
 * @brief This file defines the structures, enumerations, and macros used for handling interrupts
 * and exceptions within the operating system. It provides an implementation of the Interrupt
 * Descriptor Table (IDT) and various related exception and interrupt types.
 *
 * The file includes the following:
 * - Enumeration of exception types (e.g., divide-by-zero, breakpoint, page fault).
 * - Enumeration of interrupt types (e.g., system timer interrupt, keyboard interrupt,
 *   local APIC interrupts).
 * - The `iframe` structure used to save the CPU register state when an interrupt or exception
 * occurs.
 * - The `nmi_frame` structure used to handle the state during Non-Maskable Interrupts (NMIs).
 * - Macros for setting attributes for interrupt gates, including DPL (Descriptor Privilege Level)
 *   and interrupt type.
 * - Functionality to initialize and load the IDT, which manages the interrupt and exception
 * vectors.
 *
 * The code facilitates interrupt handling, context switching, and system-level exception management
 * by leveraging IDT-based mechanisms and provides essential tools for exception handling in the
 * kernel.
 *
 * @note This file is intended to be part of the kernel's interrupt handling subsystem and may
 * require assembly-level code to fully function (e.g., for loading the IDT).
 */
#ifndef KERNEL_ARCH_X86_64_CPU_EXCEPTIONS_H
#define KERNEL_ARCH_X86_64_CPU_EXCEPTIONS_H 1

#include <stdint.h>

/**
 * @brief Enumeration of Exception Types.
 *
 * This enumeration defines the various exception types that the processor
 * can generate during execution. These exceptions are mapped to specific vector
 * numbers, and they include both hardware and software exceptions.
 */
enum ExceptionType {
  EXCEPTION_DIVIDE_BY_0 = 0,                ///< Divide-by-zero exception (vector 0)
  EXCEPTION_DEBUG = 1,                      ///< Debug exception (vector 1)
  EXCEPTION_NON_MASKABLE_INTERRUPT = 2,     ///< Non-maskable interrupt (vector 2)
  EXCEPTION_BREAKPOINT = 3,                 ///< Breakpoint exception (vector 3)
  EXCEPTION_OVERFLOW = 4,                   ///< Overflow exception (vector 4)
  EXCEPTION_BOUND_RANGE = 5,                ///< Bound range exception (vector 5)
  EXCEPTION_INVALID_OPCODE = 6,             ///< Invalid opcode exception (vector 6)
  EXCEPTION_DEVICE_NA = 7,                  ///< Device not available exception (vector 7)
  EXCEPTION_DOUBLE_FAULT = 8,               ///< Double fault exception (vector 8)
  EXCEPTION_INVALID_TSS = 10,               ///< Invalid TSS exception (vector 10)
  EXCEPTION_SEGMENT_NOT_PRESENT = 11,       ///< Segment not present exception (vector 11)
  EXCEPTION_STACK_SEGMENT_FAULT = 12,       ///< Stack segment fault exception (vector 12)
  EXCEPTION_GENERAL_PROTECTION_FAULT = 13,  ///< General protection fault (vector 13)
  EXCEPTION_PAGE_FAULT = 14,                ///< Page fault exception (vector 14)
  EXCEPTION_x87_FLOATING_POINT = 16,        ///< x87 floating point exception (vector 16)
  EXCEPTION_ALIGNMENT_CHECK = 17,           ///< Alignment check exception (vector 17)
  EXCEPTION_MACHINE_CHECK = 18,             ///< Machine check exception (vector 18)
  EXCEPTION_SIMD_FLOATING_POINT = 19,       ///< SIMD floating point exception (vector 19)
  EXCEPTION_VIRTUALIZATION = 20,            ///< Virtualization exception (vector 20)
  EXCEPTION_CONTROL_PROTECTION = 21,        ///< Control protection exception (vector 21)
  EXCEPTION_HYPERVISOR_INJECTION = 28,      ///< Hypervisor injection exception (vector 28)
  EXCEPTION_VMM_COMMUNICATION = 29,         ///< VMM communication exception (vector 29)
  EXCEPTION_SECURITY = 30                   ///< Security exception (vector 30)
};

/**
 * @brief Enumeration of Interrupt Types.
 *
 * This enumeration defines the various interrupt types that the processor
 * can handle. It includes both platform interrupts (e.g., IRQs) and local
 * APIC (Advanced Programmable Interrupt Controller) interrupts.
 */
enum InterruptType {
  PLATFORM_INTERRUPT_BASE = 32,  ///< Base value for platform interrupts
  PLATFORM_MAX = 256,            ///< Maximum value for platform interrupts

  IRQ_SYSTEM_TIMER = 32,   ///< IRQ for the system timer
  IRQ_KEYBOARD = 33,       ///< IRQ for the keyboard
  IRQ_CASCADE = 34,        ///< IRQ for the cascade
  IRQ_SERIAL_PORT2 = 35,   ///< IRQ for serial port 2
  IRQ_SERIAL_PORT1 = 36,   ///< IRQ for serial port 1
  IRQ_SOUND = 37,          ///< IRQ for sound
  IRQ_FLOPPY = 38,         ///< IRQ for the floppy disk controller
  IRQ_PARALLEL_PORT = 39,  ///< IRQ for the parallel port
  IRQ_RTC = 40,            ///< IRQ for the real-time clock
  IRQ_ACPI = 41,           ///< IRQ for the Advanced Configuration and Power Interface (ACPI)
  IRQ_10 = 42,             ///< IRQ 10
  IRQ_11 = 43,             ///< IRQ 11
  IRQ_MOUSE = 44,          ///< IRQ for the mouse
  IRQ_CPU_COP = 45,        ///< IRQ for CPU coprocessor
  IRQ_PRIMARY_ATA = 46,    ///< IRQ for primary ATA (IDE) controller
  IRQ_SECONDARY_ATA = 47,  ///< IRQ for secondary ATA (IDE) controller

  INTERRUPT_LOCAL_APIC_BASE = 240,  ///< Base value for local APIC interrupts
  INTERRUPT_APIC_SPURIOUS = 240,    ///< APIC spurious interrupt
  INTERRUPT_APIC_TIMER = 241,       ///< APIC timer interrupt
  INTERRUPT_APIC_ERROR = 242,       ///< APIC error interrupt
  INTERRUPT_APIC_PMI = 243,         ///< APIC Performance Monitoring Interrupt
  INTERRUPT_IPI_GENERIC = 244,      ///< Interprocessor Interrupt for generic use
  INTERRUPT_IPI_RESCHEDULE = 245,   ///< Interprocessor Interrupt for rescheduling
  INTERRUPT_IPI_INTERRUPT = 246,    ///< Interprocessor Interrupt for specific interrupts
  INTERRUPT_IPI_HALT = 247          ///< Interprocessor Interrupt to halt the processor
};

/**
 * @brief Frame structure for handling interrupt and exception contexts.
 *
 * This structure is used to store the CPU register state when an interrupt or
 * exception occurs. It is used for context switching, exception handling, and
 * for saving and restoring the state of the CPU during interrupt handling.
 */
struct iframe {
  uint64_t rdi;       ///< Register rdi
  uint64_t rsi;       ///< Register rsi
  uint64_t rbp;       ///< Register rbp
  uint64_t rbx;       ///< Register rbx
  uint64_t rdx;       ///< Register rdx
  uint64_t rcx;       ///< Register rcx
  uint64_t rax;       ///< Register rax
  uint64_t r8;        ///< Register r8
  uint64_t r9;        ///< Register r9
  uint64_t r10;       ///< Register r10
  uint64_t r11;       ///< Register r11
  uint64_t r12;       ///< Register r12
  uint64_t r13;       ///< Register r13
  uint64_t r14;       ///< Register r14
  uint64_t r15;       ///< Register r15
  uint64_t vector;    ///< Interrupt/Exception vector number
  uint64_t err_code;  ///< Error code for certain exceptions
  uint64_t ip;        ///< Instruction pointer at the time of the interrupt
  uint64_t cs;        ///< Code segment at the time of the interrupt
  uint64_t flags;     ///< Flags register at the time of the interrupt
  uint64_t user_sp;   ///< User-space stack pointer (if applicable)
  uint64_t user_ss;   ///< User-space stack segment (if applicable)
};

/**
 * @brief Frame structure for handling Non-Maskable Interrupt (NMI) contexts.
 *
 * This structure is used for saving the register state during an NMI. In addition
 * to the registers, it also includes a field for the expected GS register value.
 * NMIs are high-priority interrupts that cannot be masked by software.
 */
struct nmi_frame {
  struct iframe regs;  ///< General register state during the NMI
  void* expected_gs;   ///< Expected value of the GS register during the NMI
};

#endif  // KERNEL_ARCH_X86_64_CPU_EXCEPTIONS_H