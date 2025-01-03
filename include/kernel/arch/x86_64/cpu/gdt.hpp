/**
 * @file
 * @brief This file defines the structures, constants, and functions for managing the Global
 * Descriptor Table (GDT) and Task State Segment (TSS) in the x86_64 architecture.
 *
 * The file includes:
 * - Definitions for the maximum number of GDT entries and the size of the I/O permission bitmap.
 * - Structures representing the Task State Segment (TSS), GDT entries, and the GDT register.
 * - Function declarations to initialize and load the GDT and TSS, which are crucial for segment
 * management and task switching in the operating system.
 *
 * The Task State Segment (TSS) stores information about the processor's state for context
 * switching, interrupt handling, and privileged operations. The Global Descriptor Table (GDT) holds
 * segment descriptors that define the memory segments and their access rights.
 *
 * @note This file is specific to x86_64 architecture and includes assembly-level code for loading
 * the GDT and TSS.
 */
#ifndef KERNEL_ARCH_CPU_GDT_HPP
#define KERNEL_ARCH_CPU_GDT_HPP 1

#include <compiler.h>

#include <array>
#include <cstdint>
#include <expected>

#include <lock.hpp>

/** @brief Maximum size of the I/O permission bitmap. */
#define MAX_IO_PERMISSION_BITMAP 8192

/** @brief Maximum number of entries in the Global Descriptor Table. */
#define MAX_GDT_ENTRIES 5

/**
 * @struct
 * @brief Task State Segment (TSS) structure for x86_64.
 * @details The TSS stores information about the stack pointers, interrupt stack table (IST), and
 * the I/O permission bitmap offset.
 */
struct Tss {
  uint32_t reserved_0;   ///< Reserved.
  uint64_t rsp[3];       ///< Stack pointers for privilege levels 0, 1, and 2.
  uint64_t reserved_1;   ///< Reserved.
  uint64_t ist[7];       ///< Interrupt Stack Table entries.
  uint64_t reserved_2;   ///< Reserved.
  uint16_t reserved_3;   ///< Reserved.
  uint16_t iopb_offset;  ///< Offset to the I/O permission bitmap.
} __PACKED;

/**
 * @struct
 * @brief Represents a single segment descriptor in the GDT.
 */
struct GdtSegment {
  uint16_t limit;           ///< Segment limit (low part).
  uint16_t base_low;        ///< Segment base address (low part).
  uint8_t base_mid;         ///< Segment base address (middle part).
  uint8_t access;           ///< Access rights and flags.
  uint8_t limit_high : 4;   ///< Segment limit (high part).
  uint8_t granularity : 4;  ///< Granularity and other flags.
  uint8_t base_high;        ///< Segment base address (high part).

  void set(uint32_t base, uint32_t limit, uint8_t granularity, uint8_t access);
} __PACKED;

/**
 * @struct
 * @brief Represents the descriptor for the TSS in the GDT.
 */
struct TssSegment {
  uint16_t limit;       ///< TSS segment limit.
  uint16_t base_low;    ///< TSS base address (low part).
  uint8_t base_mid;     ///< TSS base address (middle part).
  uint8_t flags_low;    ///< Flags (low part).
  uint8_t flags_high;   ///< Flags (high part).
  uint8_t base_high;    ///< TSS base address (high part).
  uint32_t base_upper;  ///< TSS base address (upper part).
  uint32_t reserved;    ///< Reserved.

  void set(Tss* tss);
} __PACKED;

/**
 * @struct
 * @brief Structure representing the GDT register (GDTR)..
 */
struct GdtRegister {
  uint16_t limit;  ///< Size of the GDT.
  uintptr_t base;  ///< Base address of the GDT.
} __PACKED;

/**
 * @struct
 * @brief Represents the Global Descriptor Table (GDT).
 */
struct GdtTable {
  std::array<GdtSegment, MAX_GDT_ENTRIES> segments;
  TssSegment tss_segment;
};

class Gdt {
 public:
  Gdt() = default;
  void initialize();
  void load();

 private:
  GdtTable m_table = {};
  Tss m_tss = {};
  TicketLock m_lock;
};

__CDECLS_BEGIN

/**
 * @brief Load the Global Descriptor Table (GDT).
 * @details This function uses the `lgdtq` instruction to load the GDT register (GDTR) with a new
 * GDT. It also reloads all segment registers with appropriate selectors and transitions to the
 * new code segment using a far return (`lretq`).
 * @param gdtr Pointer to the `gdt_register` structure containing the base and limit of the GDT.
 * @note This function is implemented in assembly.
 */
void load_gdt(GdtRegister* gdtr);

/**
 * @brief Load the Task State Segment (TSS).
 * @details This function uses the `ltr` (Load Task Register) instruction to load the Task State
 * Segment (TSS) selector into the task register. This initializes the CPU's task state.
 * @note This function is implemented in assembly.
 */
void load_tss();

__CDECLS_END

#endif  // KERNEL_ARCH_CPU_GDT_HPP