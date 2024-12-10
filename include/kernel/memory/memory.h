/**
 * @file
 * @brief Provides macros for memory alignment, manipulation, and address conversions.
 * 
 * This header defines utility macros for performing memory-related operations such as alignment,
 * address conversions between higher-half and lower-half addresses, and basic arithmetic functions
 * like rounding, minimum, and maximum computations.
 * 
 * These macros are commonly used in memory management implementations.
 */
#ifndef KERNEL_MEMORY_MEMORY_H
#define KERNEL_MEMORY_MEMORY_H 1

#include <kernel/kernel.h>

/**
 * @brief Defines the page size in bytes.
 */
#define PAGE_SIZE 0x1000ull ///< Page size (4 KiB).

/**
 * @brief Defines the page shift value for calculating page indices.
 */
#define PAGE_SHIFT 12 ///< Number of bits to shift for page alignment (log2(PAGE_SIZE)).

/**
 * @brief Converts a physical address to its higher-half address.
 * 
 * @param x The physical address to convert.
 * @return The higher-half address corresponding to the given physical address.
 */
#define TO_HIGHER_HALF(x) ((typeof(x))((uintptr_t)(x) + hhdm_request.response->offset))

/**
 * @brief Converts a higher-half address to its physical address.
 * 
 * @param x The higher-half address to convert.
 * @return The physical address corresponding to the given higher-half address.
 * 
 * @note Requires `hhdm_request.response->offset` to be initialized.
 */
#define FROM_HIGHER_HALF(x) ((typeof(x))((uintptr_t)(x) - hhdm_request.response->offset))

/**
 * @brief Aligns a value down to the nearest multiple of the given page size.
 * 
 * @param x The value to align.
 * @param page_size The alignment boundary.
 * @return The aligned value.
 */
#define ALIGN_DOWN(x, page_size) ((typeof(x))((uintptr_t)(x) & ~((size_t)(page_size) - 1)))

/**
 * @brief Aligns a value up to the nearest multiple of the given page size.
 * 
 * @param x The value to align.
 * @param page_size The alignment boundary.
 * @return The aligned value.
 */
#define ALIGN_UP(x, page_size) ALIGN_DOWN((x) + (page_size) - 1, page_size)

/**
 * @brief Divides a value by the page size, rounding up to the nearest integer.
 * 
 * @param x The dividend.
 * @param page_size The divisor.
 * @return The quotient, rounded up.
 */
#define DIV_ROUNDUP(x, page_size) (ALIGN_UP(x, page_size) / (page_size))

/**
 * @brief Computes the maximum of two values.
 * 
 * @param a The first value.
 * @param b The second value.
 * @return The larger of the two values.
 */
#define MAX(a, b)           \
  ({                        \
    typeof(a) _a = (a);     \
    typeof(b) _b = (b);     \
    _a > _b ? _a : _b;      \
  })

/**
 * @brief Computes the minimum of two values.
 * 
 * @param a The first value.
 * @param b The second value.
 * @return The smaller of the two values.
 */
#define MIN(a, b)           \
  ({                        \
    typeof(a) _a = (a);     \
    typeof(b) _b = (b);     \
    _a < _b ? _a : _b;      \
  })

#endif  // KERNEL_MEMORY_MEMORY_H