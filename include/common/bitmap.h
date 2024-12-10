/**
 * @file
 * @brief Provides macros for efficient manipulation of individual bits in a bitmap.
 *
 * This file defines a set of macros that allow for common operations on a bitmap, which is
 * represented as an array of bytes. A bitmap is a compact data structure for tracking binary states
 * (set or unset) across a large set of elements, commonly used in systems programming for managing
 * resources like memory, file descriptors, or process IDs.
 *
 * Key features:
 * - Macros for setting (`BITMAP_SET_BIT`), clearing (`BITMAP_CLEAR_BIT`), toggling (`TOGGLE_BIT`),
 * and checking
 *   (`IS_BIT_SET`) individual bits in the bitmap.
 * - The bitmap is represented as a contiguous array of bytes, with each bit corresponding to a
 * unique state.
 * - Optimized for minimal memory usage and high performance.
 * - Portability and ease of use across different platforms, as it relies on standard C syntax.
 *
 * These macros are designed to provide a clean and efficient way to perform bit-level operations
 * without requiring additional function calls, making them ideal for low-level systems programming.
 *
 * @note Ensure that the bitmap array is properly allocated and sized to avoid out-of-bounds memory
 * access. The size of the bitmap should be at least `(num_bits / 8) + ((num_bits % 8) ? 1 : 0)`
 * bytes.
 */
#ifndef COMMON_BITMAP_H
#define COMMON_BITMAP_H 1

/**
 * @brief Sets a specific bit in the bitmap to 1.
 * @param bitmap Pointer to the byte array representing the bitmap.
 * @param idx Index of the bit to set.
 * @note This macro modifies the byte containing the target bit.
 */
#define BITMAP_SET_BIT(bitmap, idx)                 \
  ({                                                \
    size_t _idx = (idx);                            \
    ((bitmap)[(_idx) / 8] |= (1U << ((_idx) % 8))); \
  })

/**
 * @brief Clears a specific bit in the bitmap, setting it to 0.
 * @param bitmap Pointer to the byte array representing the bitmap.
 * @param idx Index of the bit to clear.
 * @note This macro modifies the byte containing the target bit.
 */
#define BITMAP_CLEAR_BIT(bitmap, idx)                \
  ({                                                 \
    size_t _idx = (idx);                             \
    ((bitmap)[(_idx) / 8] &= ~(1U << ((_idx) % 8))); \
  })

/**
 * @brief Toggles a specific bit in the bitmap.
 * @param bitmap Pointer to the byte array representing the bitmap.
 * @param idx Index of the bit to toggle.
 * @note This macro modifies the byte containing the target bit.
 */
#define TOGGLE_BIT(bitmap, idx)                     \
  ({                                                \
    size_t _idx = (idx);                            \
    ((bitmap)[(_idx) / 8] ^= (1U << ((_idx) % 8))); \
  })

/**
 * @brief Checks if a specific bit in the bitmap is set to 1.
 * @param bitmap Pointer to the byte array representing the bitmap.
 * @param idx Index of the bit to check.
 * @return Non-zero if the bit is set, otherwise 0.
 */
#define IS_BIT_SET(bitmap, idx)                           \
  ({                                                      \
    size_t _idx = (idx);                                  \
    (((bitmap)[(_idx) / 8] & (1U << ((_idx) % 8))) != 0); \
  })

#endif  // COMMON_BITMAP_H