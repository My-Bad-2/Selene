#include <klibc/assert.h>
#include <klibc/string.h>
#include <log.h>

#include <common/bitmap.h>
#include <kernel/memory/physical.h>

struct mutex phys_lock = INITIALIZE_MUTEX;
struct physical_stats phys_stats = {};
uint8_t* phys_bitmap = nullptr;
// Tracks the last accessed index in the bitmap to optimize allocation searches.
size_t phys_bitmap_last_index = 0;

/**
 * @brief Internal function to allocate a block of memory up to a specified limit.
 *
 * @param count Number of pages to allocate.
 * @param limit Maximum index in the bitmap to search for free pages.
 * @return Pointer to the allocated memory block, or `NULL` if allocation fails.
 */
static void* physical_allocate_limit(size_t count, size_t limit) {
  size_t i = 0;

  while (phys_bitmap_last_index < limit) {
    if (!IS_BIT_SET(phys_bitmap, phys_bitmap_last_index++)) {
      if (++i == count) {
        size_t page = phys_bitmap_last_index - count;

        for (size_t j = page; j < phys_bitmap_last_index; j++) {
          BITMAP_SET_BIT(phys_bitmap, j);
        }

        return (void*)(page * PAGE_SIZE);
      }
    } else {
      i = 0;
    }
  }

  return nullptr;
}

void* physical_allocate(size_t size) {
  if (size == 0) {
    return nullptr;
  }

  mutex_lock(&phys_lock);

  const size_t count = DIV_ROUNDUP(size, PAGE_SIZE);
  size_t index = phys_bitmap_last_index;
  void* ret = physical_allocate_limit(count, phys_stats.highest_usable_addr / PAGE_SIZE);

  if (ret == nullptr) {
    phys_bitmap_last_index = 0;
    ret = physical_allocate_limit(count, index);

    if (ret == nullptr) {
      mutex_unlock(&phys_lock);

      log_panic("Out of Physical Memory.");
      return nullptr;
    }
  }

  memset(TO_HIGHER_HALF(ret), 0, count * PAGE_SIZE);
  phys_stats.used_pages += count;

  mutex_unlock(&phys_lock);
  return ret;
}

void physical_free(void* ptr, size_t size) {
  if (ptr == nullptr) {
    return;
  }

  mutex_lock(&phys_lock);

  size_t page = (uintptr_t)ptr / PAGE_SIZE;
  const size_t count = size / PAGE_SIZE;

  for (size_t i = page; i < (page + count); i++) {
    BITMAP_CLEAR_BIT(phys_bitmap, i);
  }

  phys_stats.used_pages -= count;

  mutex_unlock(&phys_lock);
}

void physical_stats(struct physical_stats* stats) {
  memcpy(stats, &phys_stats, sizeof(struct physical_stats));
}
/**
 * @details This function sets up the memory manager by analyzing the memory map and creating a
 * bitmap to track page allocations.
 * @note Must be called before any other physical memory operations.
 */
void physical_initialize(void) {
  const size_t memmap_count = memmap_request.response->entry_count;
  struct limine_memmap_entry** memmaps = memmap_request.response->entries;

  uintptr_t highest_phys_addr = 0;
  uintptr_t highest_usable_addr = 0;
  uintptr_t lowest_usable_addr = 0x1000;

  memset(&phys_stats, 0, sizeof(struct physical_stats));

  for (size_t i = 0; i < memmap_count; i++) {
    uintptr_t limit = memmaps[i]->base + memmaps[i]->length;

    highest_phys_addr = MAX(highest_phys_addr, limit);

    switch (memmaps[i]->type) {
      case LIMINE_MEMMAP_USABLE:
        highest_usable_addr = MAX(highest_usable_addr, limit);
        lowest_usable_addr = MIN(lowest_usable_addr, limit);

        phys_stats.usable_pages += (memmaps[i]->length / PAGE_SIZE);
        break;
      case LIMINE_MEMMAP_EXECUTABLE_AND_MODULES:
      case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
        break;
      default:
        continue;
    }

    phys_stats.total_pages += (memmaps[i]->length / PAGE_SIZE);
  }

  phys_stats.highest_usable_addr = highest_usable_addr;
  phys_stats.highest_phys_addr = highest_phys_addr;
  phys_stats.lowest_usable_addr = lowest_usable_addr;

  size_t bitmap_entries = highest_usable_addr / PAGE_SIZE;
  size_t bitmap_size = ALIGN_UP(bitmap_entries / 8, PAGE_SIZE);
  bitmap_entries = bitmap_size * 8;

  for (size_t i = 0; i < memmap_count; i++) {
    if (memmaps[i]->type != LIMINE_MEMMAP_USABLE) {
      continue;
    }

    if (memmaps[i]->length >= bitmap_size) {
      uint8_t* bitmap = (uint8_t*)TO_HIGHER_HALF(memmaps[i]->base);
      memset(bitmap, 0xff, bitmap_size);

      memmaps[i]->length -= bitmap_size;
      memmaps[i]->base += bitmap_size;

      phys_stats.used_pages += bitmap_size / PAGE_SIZE;
      phys_bitmap = bitmap;
      break;
    }
  }

  for (size_t i = 0; i < memmap_count; i++) {
    if (memmaps[i]->type != LIMINE_MEMMAP_USABLE) {
      continue;
    }

    for (uintptr_t j = 0; j < memmaps[i]->length; j += PAGE_SIZE) {
      BITMAP_CLEAR_BIT(phys_bitmap, (memmaps[i]->base + j) / PAGE_SIZE);
    }
  }

#ifdef DEBUG
  log_trace("Bitmap size = %lu (%lu KiB)", bitmap_size, bitmap_size / 1024);
  log_trace("Lowest usable address = %p", (void*)lowest_usable_addr);
  log_trace("Highest usable address = %p", (void*)highest_usable_addr);
  log_trace("Highest physical address = %p", (void*)highest_phys_addr);

  log_trace("Total usable memory = %lu pages.", phys_stats.usable_pages);
  log_trace("Used memory = %lu pages.", phys_stats.used_pages);
  log_trace("Free memory = %lu pages.", phys_stats.usable_pages - phys_stats.used_pages);
#endif 

  log_info("Initialized Physical Memory Manager!");
}