#ifndef KERNEL_MEMORY_PHYSICAL_HPP
#define KERNEL_MEMORY_PHYSICAL_HPP 1

#include <cstddef>
#include <cstdint>

#include <common/bitmap.hpp>

class PhysicalAllocator {
 public:
  explicit PhysicalAllocator() = default;

  template <typename T = void*>
  T allocate(size_t size) {
    return this->allocate(size);
  }

  uintptr_t allocate(size_t size);
  void free(auto ptr, size_t size);
  void initialize();
  void info() const;

 private:
  uintptr_t m_highest_phys_addr;    ///< Highest physical address in use.
  uintptr_t m_highest_usable_addr;  ///< Highest usable physical address.

  size_t m_total_pages;   ///< Total number of pages in physical memory.
  size_t m_usable_pages;  ///< Number of pages available for use.
  size_t m_used_pages;    ///< Number of pages currently allocated.
  size_t m_last_used_idx;

  Bitmap m_bitmap;
};

#endif  // KERNEL_MEMORY_PHYSICAL_HPP