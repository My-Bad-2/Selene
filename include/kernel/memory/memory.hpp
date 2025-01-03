/**
 * @file
 * @brief Provides functions and macros for memory alignment, manipulation, and address conversions.
 *
 * This header defines utility functions for performing memory-related operations such as alignment,
 * address conversions between higher-half and lower-half addresses, and basic arithmetic functions
 * like rounding, minimum, and maximum computations.
 *
 * These functions are commonly used in memory management implementations.
 */
#ifndef KERNEL_MEMORY_MEMORY_HPP
#define KERNEL_MEMORY_MEMORY_HPP 1

#include <concepts>
#include <cstddef>
#include <cstdint>

#include <kernel/kernel.h>

/**
 * @brief Defines the page size in bytes.
 */
enum page_size {
  PAGE_SIZE_4KiB = 0x1000u,      ///< Page size (4 KiB).
  PAGE_SIZE_2MiB = 0x200000u,    ///< Page size (2 MiB).
  PAGE_SIZE_1GiB = 0x40000000u,  ///< Page size (1 GiB).
};

constexpr uintptr_t invalid_address = static_cast<uintptr_t>(-1);

template <typename Type>
using RetType = std::conditional_t<
    std::integral<Type>,
    std::conditional_t<std::unsigned_integral<Type>, std::uintptr_t, std::intptr_t>, Type>;

constexpr bool is_higher_half(auto addr) {
  return uintptr_t(addr) >= hhdm_request.response->offset;
}

/**
 * @brief Converts a physical address to its higher-half address.
 *
 * @param x The physical address to convert.
 * @return The higher-half address corresponding to the given physical address.
 *
 * @note Requires `hhdm_request.response->offset` to be initialized.
 */
template <typename T, typename U = RetType<T>>
constexpr U to_higher_half(T addr) {
  if (is_higher_half(addr)) {
    return addr;
  }

  return U(hhdm_request.response->offset + uintptr_t(addr));
}

/**
 * @brief Converts a higher-half address to its physical address.
 *
 * @param x The higher-half address to convert.
 * @return The physical address corresponding to the given higher-half address.
 *
 * @note Requires `hhdm_request.response->offset` to be initialized.
 */
template <typename T, typename U = RetType<T>>
constexpr U from_higher_half(T addr) {
  if (!is_higher_half(addr)) {
    return addr;
  }

  return U(uintptr_t(addr) - hhdm_request.response->offset);
}

/**
 * @brief Aligns a value down to the nearest multiple of the given base.
 *
 * @param num The value to align.
 * @param base The alignment boundary.
 * @return The aligned value.
 */
constexpr auto align_down(std::unsigned_integral auto num, std::unsigned_integral auto base) {
  return (num & ~(base - 1));
}

/**
 * @brief Aligns a value up to the nearest multiple of the given base.
 *
 * @param num The value to align.
 * @param base The alignment boundary.
 * @return The aligned value.
 */
constexpr auto align_up(std::unsigned_integral auto num, std::unsigned_integral auto base) {
  return align_down(num + base - 1, base);
}

/**
 * @brief Checks whether the given value is base-aligned.
 *
 * @param num The value to align.
 * @param base The alignment boundary.
 * @return True, if the num is aligned. Otherwise False.
 */
constexpr bool is_aligned(std::unsigned_integral auto num, std::unsigned_integral auto base) {
  return align_down(num, base) == num;
}

/**
 * @brief Divides a value by the base, rounding up to the nearest integer.
 *
 * @param num The dividend.
 * @param base The divisor.
 * @return The quotient, rounded up.
 */
constexpr bool div_round_up(std::unsigned_integral auto num, std::unsigned_integral auto base) {
  return align_up(num, base) / base;
}

constexpr size_t to_KB(const size_t bytes) {
  return bytes / 1024;
}

constexpr size_t to_MB(const size_t bytes) {
  return to_KB(bytes) / 1024;
}

constexpr size_t to_GB(const size_t bytes) {
  return to_MB(bytes) / 1024;
}

#endif  // KERNEL_MEMORY_MEMORY_HPP