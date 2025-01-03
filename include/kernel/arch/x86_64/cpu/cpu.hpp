/**
 * @file
 * @brief This file defines macros, constants, and functions for low-level CPU operations in the
 * x86_64 architecture.
 *
 * The file includes:
 * - Macros for defining memory types in the Page Attribute Table (PAT) and the Reverse Fault Bitmap
 * (RFBM).
 * - Function declarations for interacting with control registers (CR0-CR4), Model-Specific
 * Registers (MSRs), and performing Translation Lookaside Buffer (TLB) operations.
 * - Functions for saving and restoring CPU states using XSAVE and FXSAVE instructions.
 * - Functions for reading and setting base addresses for the FS, GS, and Kernel GS segment
 * registers.
 *
 * This header provides the essential tools for controlling and monitoring low-level processor
 * features, including memory caching behavior, CPU state management, and segment register
 * operations. These functions are critical for kernel development and managing CPU behavior at a
 * low level.
 *
 * @note This file is specific to x86_64 architecture and may include hardware-specific
 * implementations.
 */
#ifndef KERNEL_ARCH_CPU_CPU_HPP
#define KERNEL_ARCH_CPU_CPU_HPP 1

#include <cstdint>

#include <kernel/arch/x86_64/regs.h>

/**
 * @defgroup pat_macros PAT (Page Attribute Table) Macros
 * @brief Macros for defining memory types in the Page Attribute Table (PAT).
 * @{
 */
#define PAT_FORCE_UNCACHABLE 0ul  ///< Force uncacheable memory type.
#define PAT_WRITE_COMBINING 1ul   ///< Write-combining memory type.
#define PAT_WRITE_THROUGH 4ul     ///< Write-through memory type.
#define PAT_WRITE_PROTECT 5ul     ///< Write-protect memory type.
#define PAT_WRITE_BACK 6ul        ///< Write-back memory type.
#define PAT_UNCACHABLE 7ul        ///< Uncacheable memory type.

/**
 * @brief Default PAT configuration combining all defined PAT types.
 *
 * The configuration defines the order of memory types in the PAT MSR.
 */
#define DEFAULT_PAT                                                              \
  ((PAT_UNCACHABLE << 56) | (PAT_WRITE_BACK << 48) | (PAT_WRITE_PROTECT << 40) | \
   (PAT_WRITE_THROUGH << 32) | (PAT_WRITE_COMBINING << 24) | (PAT_FORCE_UNCACHABLE << 16))
/** @} */

/**
 * @defgroup rfbm_macros Reverse Fault Bitmap Macros
 * @brief Macros for Reverse Fault Bitmap (RFBM) manipulation.
 * @{
 */
#define RFBM (static_cast<uint64_t>(-1))               ///< Reverse Fault Bitmap set to all ones.
#define RFBM_LOW (static_cast<uint32_t>(RFBM))         ///< Lower 32 bits of the RFBM.
#define RFBM_HIGH (static_cast<uint32_t>(RFBM >> 32))  ///< Upper 32 bits of the RFBM.
/** @} */

/**
 * @defgroup tlb_operations TLB Operations
 * @brief Functions to operate on Translation Lookaside Buffer (TLB).
 * @{
 */
/**
 * @brief Invalidates a page in the TLB for a specific address.
 * @param address The address of the page to invalidate.
 */
void invalidate_page(uintptr_t address);
/** @} */

/**
 * @defgroup control_registers Control Register Operations
 * @brief Functions to read from and write to CPU control registers.
 * @{
 */
/** @brief Reads the value of CR0. */
uint64_t read_cr0();
/** @brief Reads the value of CR2. */
uint64_t read_cr2();
/** @brief Reads the value of CR3. */
uint64_t read_cr3();
/** @brief Reads the value of CR4. */
uint64_t read_cr4();

/** @brief Writes a value to CR0. */
void write_cr0(uint64_t value);
/** @brief Writes a value to CR2. */
void write_cr2(uint64_t value);
/** @brief Writes a value to CR3. */
void write_cr3(uint64_t value);
/** @brief Writes a value to CR4. */
void write_cr4(uint64_t value);
/** @} */

/**
 * @defgroup msr_operations Model-Specific Register (MSR) Operations
 * @brief Functions to read from and write to model-specific registers.
 * @{
 */
/**
 * @brief Reads the value of an MSR.
 * @param msr The MSR to read.
 * @return The value of the MSR.
 */
uint64_t read_msr(uint32_t msr);

/**
 * @brief Writes a value to an MSR.
 * @param msr The MSR to write to.
 * @param value The value to write.
 */
void write_msr(uint32_t msr, uint64_t value);

/**
 * @brief Enables the PAT (Page Attribute Table) feature.
 */
void enable_pat();
/** @} */

/**
 * @defgroup xsave_fxsave XSAVE and FXSAVE Operations
 * @brief Functions for saving and restoring CPU state using XSAVE and FXSAVE.
 * @{
 */
/**
 * @brief Saves the processor state to a specified memory region using FXSAVE.
 * @param region A pointer to the memory region.
 */
void fxsave(uint8_t const* region);

/**
 * @brief Saves the processor state to a specified memory region using XSAVE.
 * @param region A pointer to the memory region.
 */
void xsave(uint8_t const* region);

/**
 * @brief Saves the processor state to a specified memory region using XSAVEOPT.
 * @param region A pointer to the memory region.
 */
void xsaveopt(uint8_t const* region);

/**
 * @brief Restores the processor state from a specified memory region using XRSTOR.
 * @param region A pointer to the memory region.
 */
void xrstor(uint8_t const* region);

/**
 * @brief Restores the processor state from a specified memory region using FXRSTOR.
 * @param region A pointer to the memory region.
 */
void fxrstor(uint8_t const* region);
/** @} */

/**
 * @defgroup segment_registers Segment Register Operations
 * @brief Functions to read from and write to CPU segment registers.
 * @{
 */
/** @brief Gets the base address of the Kernel GS segment. */
uintptr_t kernel_gs_base();
/** @brief Gets the base address of the FS segment. */
uintptr_t fs_base();
/** @brief Gets the base address of the GS segment. */
uintptr_t gs_base();

/** @brief Sets the base address of the FS segment. */
void set_fs_base(uintptr_t addr);
/** @brief Sets the base address of the GS segment. */
void set_gs_base(uintptr_t addr);
/** @brief Sets the base address of the Kernel GS segment. */
void set_kernel_gs_base(uintptr_t addr);

/**
 * @brief Swaps the current GS base with the Kernel GS base.
 */
void swapgs();
/** @} */

#endif  // KERNEL_ARCH_X86_64_CPU_CPU_H