/**
 * @file
 * @brief Interface for the physical memory management system.
 * 
 * This header provides the declarations for a physical memory management system that uses 
 * a bitmap to track memory usage. It allows for allocation, deallocation, and retrieval of 
 * memory statistics.
 * 
 * Key Features:
 * - Page-level memory allocation and deallocation.
 * - Efficient bitmap representation of memory state.
 * - Thread-safe operations using a mutex.
 * - Integration with the bootloader's memory map for initialization.
 * 
 * @note All memory allocations and deallocations are performed at page-level granularity.
 */
#ifndef KERNEL_MEMORY_PHYSICAL_H
#define KERNEL_MEMORY_PHYSICAL_H 1

#include <lock.h>
#include <stddef.h>
#include <stdint.h>

#include <kernel/memory/memory.h>

/**
 * @struct physical_stats
 * @brief Represents statistics about the current state of physical memory.
 */
struct physical_stats {
  uintptr_t highest_phys_addr;    ///< Highest physical address in use.
  uintptr_t highest_usable_addr;  ///< Highest usable physical address.
  uintptr_t lowest_usable_addr;   ///< Lowest usable physical address.

  size_t total_pages;   ///< Total number of pages in physical memory.
  size_t usable_pages;  ///< Number of pages available for use.
  size_t used_pages;    ///< Number of pages currently allocated.
};

/**
 * @brief Initializes the physical memory management system.
 */
void physical_initialize();

/**
 * @brief Allocates a block of physical memory.
 * @param size Size of the memory block to allocate (in bytes).
 * @return Pointer to the allocated memory block, or `NULL` if allocation fails.
 */
void* physical_allocate(size_t size);

/**
 * @brief Frees a previously allocated block of physical memory.
 * @param ptr Pointer to the memory block to free.
 * @param size Size of the memory block being freed (in bytes).
 */
void physical_free(void* ptr, size_t size);

/**
 * @brief Retrieves statistics about the current state of physical memory.
 * @param stats Pointer to a `physical_stats` structure to populate with memory statistics.
 */
void physical_stats(struct physical_stats* stats);

#endif  // KERNEL_MEMORY_PHYSICAL_H