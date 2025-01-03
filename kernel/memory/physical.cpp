#include <log.hpp>
#include <string.h>

#include <span>

#include <kernel/kernel.h>

#include <kernel/memory/memory.hpp>
#include <kernel/memory/physical.hpp>

uintptr_t PhysicalAllocator::allocate(size_t size) {
  if (size == 0) {
    return 0;
  }

  size_t page_count = div_round_up(size, static_cast<size_t>(PAGE_SIZE_4KiB));

  auto allocate_page = [&](size_t upper_limit) -> uintptr_t {
    size_t idx = 0;

    while (this->m_last_used_idx > upper_limit) {
      if (!this->m_bitmap.get(this->m_last_used_idx++)) {
        if (++idx == page_count) {
          size_t page = this->m_last_used_idx - page_count;

          for (size_t i = page; i < page_count; i++) {
            this->m_bitmap.set(idx);
          }

          return (page * PAGE_SIZE_4KiB);
        }
      } else {
        idx = 0;
      }
    }

    return 0;
  };

  size_t index = this->m_last_used_idx;
  uintptr_t ret = allocate_page(this->m_highest_usable_addr / PAGE_SIZE_4KiB);

  if (!ret) {
    this->m_last_used_idx = 0;
    ret = allocate_page(index);

    if (!ret) {
      log_panic("Out of Physical Memory.");
      return 0;
    }
  }

  memset(reinterpret_cast<void*>(to_higher_half(ret)), 0, page_count * PAGE_SIZE_4KiB);
  this->m_used_pages += page_count;

  return ret;
}

void PhysicalAllocator::free(auto ptr, size_t size) {
  uintptr_t addr = uintptr_t(ptr);

  if (addr == 0) {
    return;
  }

  size_t page_count = div_round_up(size, static_cast<size_t>(PAGE_SIZE_4KiB));
  size_t page = addr / PAGE_SIZE_4KiB;

  for (size_t i = 0; i < page_count; i++) {
    this->m_bitmap.clear(page + i);
  }

  this->m_used_pages -= page_count;
}

void PhysicalAllocator::initialize() {
  std::span<limine_memmap_entry*> memmaps(memmap_request.response->entries,
                                          memmap_request.response->entry_count);

  for (const auto memmap : memmaps) {
    uintptr_t upper_bound = memmap->base + memmap->length;

    this->m_highest_phys_addr = std::max(this->m_highest_phys_addr, upper_bound);

    switch (memmap->type) {
      case LIMINE_MEMMAP_EXECUTABLE_AND_MODULES:
      case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
        this->m_used_pages += memmap->length / PAGE_SIZE_4KiB;
        __FALLTHROUGH;
      case LIMINE_MEMMAP_USABLE:
        this->m_usable_pages += memmap->length / PAGE_SIZE_4KiB;
        this->m_highest_usable_addr = std::max(this->m_highest_usable_addr, upper_bound);
        break;
      default:
        continue;
    }

    this->m_total_pages += memmap->length / PAGE_SIZE_4KiB;
  }

  size_t bitmap_entries = this->m_highest_usable_addr / PAGE_SIZE_4KiB;
  const size_t bitmap_size = align_up(bitmap_entries / 8, static_cast<size_t>(PAGE_SIZE_4KiB));
  bitmap_entries = bitmap_size * 8;

  for (auto memmap : memmaps) {
    if (memmap->type != LIMINE_MEMMAP_USABLE) {
      continue;
    }

    if (memmap->length >= bitmap_size) {
      uintptr_t base = to_higher_half(memmap->base);
      uint8_t* bitmap = reinterpret_cast<uint8_t*>(base);

      memset(bitmap, 0xff, bitmap_entries);
      this->m_bitmap.initialize(bitmap, bitmap_entries);

      log_debug("Initialized Bitmap at address: %p size: 0x%lx", bitmap, bitmap_entries);

      memmap->base += bitmap_size;
      memmap->length -= bitmap_size;

      this->m_used_pages += bitmap_size / PAGE_SIZE_4KiB;
      break;
    }
  }

  if (this->m_bitmap.data() == nullptr) {
    log_panic(
        "Unable to find a memory region with sufficiently enough"
        "contiguously usable memory space for physical memory bitmap.");
  }

  for (const auto& memmap : memmaps) {
    if (memmap->type != LIMINE_MEMMAP_USABLE) {
      continue;
    }

    for (size_t i = 0; i < memmap->length; i += PAGE_SIZE_4KiB) {
      const size_t idx = (memmap->base + i) / PAGE_SIZE_4KiB;
      this->m_bitmap.clear(idx);
    }
  }

  this->m_bitmap.set(0);

  this->info();
}

void PhysicalAllocator::info() const {
  log_debug("Total Physical Memory = %lu MB", to_MB(this->m_total_pages * PAGE_SIZE_4KiB));
  log_debug("Usable Physical Memory = %lu MB", to_MB(this->m_usable_pages * PAGE_SIZE_4KiB));
  log_debug("Used Physical Memory = %lu MB", to_MB(this->m_used_pages * PAGE_SIZE_4KiB));
  log_debug("Highest Physical Address = 0x%lx", this->m_highest_phys_addr);
  log_debug("Highest Usable Address = 0x%lx", this->m_highest_usable_addr);
}